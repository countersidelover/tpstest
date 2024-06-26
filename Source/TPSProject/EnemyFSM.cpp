#include "EnemyFSM.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include "TPSProject.h"
#include <Components/CapsuleComponent.h>
#include "MeteorBomb.h"
#include "WhiteSphere.h"
#include "TempMan.h"
#include "EnemyAnim.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Pillar.h"

UEnemyFSM::UEnemyFSM()
{
    PrimaryComponentTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
    if (SphereMesh.Succeeded())
    {
        WhiteSphereMesh = SphereMesh.Object;
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PathMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
    if (PathMesh.Succeeded())
    {
        ChargingPathStaticMesh = PathMesh.Object;
    }

    static ConstructorHelpers::FObjectFinder<UMaterial> PathMaterial(TEXT("Material'/Game/StarterContent/Materials/M_AssetPlatform.M_AssetPlatform'"));
    if (PathMaterial.Succeeded())
    {
        ChargingPathMaterial = PathMaterial.Object;
    }

    bWhiteSphereExecuted = false;
    mState = EEnemyState::Idle;
    hp = 100; // 적절한 초기화
    isExecutingPattern = false;
}

void UEnemyFSM::BeginPlay()
{
    Super::BeginPlay();

    if (!target)
    {
        auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATempMan::StaticClass());
        target = Cast<ATempMan>(actor);
    }
    me = Cast<AEnemy>(GetOwner());
    anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
    if (me && ChargingPathStaticMesh && ChargingPathMaterial)
    {
        ChargingPathMesh = NewObject<UStaticMeshComponent>(me);
        if (ChargingPathMesh)
        {
            ChargingPathMesh->SetStaticMesh(ChargingPathStaticMesh);
            ChargingPathMesh->SetMaterial(0, ChargingPathMaterial);
            ChargingPathMesh->SetVisibility(false);
            ChargingPathMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            ChargingPathMesh->AttachToComponent(me->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
            ChargingPathMesh->RegisterComponent();
        }
    }
}

void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!me || !target) // 'me'와 'target'이 유효한지 확인
    {
        UE_LOG(LogTemp, Error, TEXT("me or target is nullptr"));
        return;
    }

    if (!bWhiteSphereExecuted)
    {
        WhiteSphereState();
        bWhiteSphereExecuted = true;
        mState = EEnemyState::Idle;
        return;
    }

    if (!isExecutingPattern)
    {
        timeSinceLastMeteor += DeltaTime;
        timeSinceLastCharge += DeltaTime;

        if (mState == EEnemyState::Idle)
        {
            if (timeSinceLastMeteor >= meteorInterval)
            {
                mState = EEnemyState::Meteor;
                timeSinceLastMeteor = 0.0f;
            }
            else if (timeSinceLastCharge >= chargeInterval)
            {
                mState = EEnemyState::Charge;
                timeSinceLastCharge = 0.0f;
            }
        }
    }

    switch (mState)
    {
    case EEnemyState::Idle:
        IdleState();
        break;
    case EEnemyState::Move:
        MoveState();
        break;
    case EEnemyState::Attack:
        AttackState();
        break;
    case EEnemyState::Damage:
        DamageState();
        break;
    case EEnemyState::Die:
        DieState();
        break;
    case EEnemyState::Meteor:
        MeteorState();
        break;
    case EEnemyState::WhiteSphere:
        WhiteSphereState();
        break;
    case EEnemyState::Charge:
        ChargeState(chargeSpeed);
        break;
    case EEnemyState::Charging:
        // Charging 상태에서는 별도의 로직을 추가하지 않음
        break;
    case EEnemyState::Groggy:
        // Groggy 상태에서는 별도의 로직을 추가하지 않음
        break;
    }
}

void UEnemyFSM::IdleState()
{
    anim->animState = mState;
    currentTime += GetWorld()->DeltaTimeSeconds;

 
}

void UEnemyFSM::MoveState()
{
    if (target)
    {
        FVector destination = target->GetActorLocation();
        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        if (dir.Size() < attackRange)
        {
            mState = EEnemyState::Attack;
        }
    }
}

void UEnemyFSM::AttackState()
{
    currentTime += GetWorld()->DeltaTimeSeconds;
    if (currentTime > attackDelayTime)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attack!!!!!"));
        currentTime = 0;
    }

    if (target)
    {
        float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
        if (distance > attackRange)
        {
            mState = EEnemyState::Move;
        }
    }
}

void UEnemyFSM::DamageState()
{
    currentTime += GetWorld()->DeltaTimeSeconds;
    if (currentTime > damageDelayTime)
    {
        mState = EEnemyState::Idle;
        currentTime = 0;
    }
}

void UEnemyFSM::DieState()
{
    mState = EEnemyState::Die;
    FVector P0 = me->GetActorLocation();
    FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
    FVector P = P0 + vt;
    me->SetActorLocation(P);

    if (P.Z < -200.0f)
    {
        me->Destroy();
    }
}

void UEnemyFSM::MeteorState()
{
    if (!isExecutingPattern)  // 이미 패턴이 실행 중인지 확인
    {
        anim->animState = EEnemyState::Meteor;  // 애니메이션 상태 설정
        isExecutingPattern = true;  // 패턴 실행 중으로 설정

        // 일정 시간 대기 후 메테오 생성
        FTimerHandle MeteorSpawnHandle;
        GetWorld()->GetTimerManager().SetTimer(MeteorSpawnHandle, [this]()
            {
                if (target)
                {
                    for (int i = 0; i < 20; ++i)
                    {
                        FVector targetLocation = target->GetActorLocation();
                        FVector startLocation = me->GetActorLocation() + FMath::VRand() * 500.0f;
                        FTransform transform;
                        transform.SetLocation(startLocation);
                        AMeteorBomb* meteor = GetWorld()->SpawnActor<AMeteorBomb>(AMeteorBomb::StaticClass(), transform);
                        if (meteor)
                        {
                            meteor->TargetLocation = targetLocation;
                            FVector Direction = (meteor->TargetLocation - startLocation).GetSafeNormal();
                            meteor->ProjectileMovementComponent->Velocity = Direction * meteor->ProjectileMovementComponent->InitialSpeed;
                        }
                    }
                }

                // 메테오 생성 후 상태를 Idle로 변경
                mState = EEnemyState::Idle;
                anim->animState = EEnemyState::Idle;  // 애니메이션 상태를 Idle로 변경
                isExecutingPattern = false;  // 패턴 실행 완료
            }, 0.5f, false);  // 0.5초 후 메테오 생성
    }
}

void UEnemyFSM::WhiteSphereState()
{
    LaunchWhiteSphere();
    mState = EEnemyState::Idle;
    isExecutingPattern = false;  // 패턴 실행 완료
}

void UEnemyFSM::ChargeState(float newChargeSpeed)
{
    anim->animState = mState;
    ChargeWarning(newChargeSpeed);
}

void UEnemyFSM::OnDamageProcess()
{
    hp--;

    if (hp > 0)
    {
        mState = EEnemyState::Damage;
    }
    else
    {
        mState = EEnemyState::Die;
        me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void UEnemyFSM::LaunchMeteors()
{
    if (target)
    {
        anim->animState = EEnemyState::Meteor;  // 애니메이션 상태 설정

        // 일정 시간 대기 후 메테오 생성
        FTimerHandle MeteorSpawnHandle;
        GetWorld()->GetTimerManager().SetTimer(MeteorSpawnHandle, [this]()
            {
                if (target)
                {
                    for (int i = 0; i < 5; ++i)
                    {
                        FVector targetLocation = target->GetActorLocation();
                        FVector startLocation = me->GetActorLocation() + FMath::VRand() * 300.0f;
                        FTransform transform;
                        transform.SetLocation(startLocation);
                        AMeteorBomb* meteor = GetWorld()->SpawnActor<AMeteorBomb>(AMeteorBomb::StaticClass(), transform);
                        if (meteor)
                        {
                            meteor->TargetLocation = targetLocation;
                            FVector Direction = (meteor->TargetLocation - startLocation).GetSafeNormal();
                            meteor->ProjectileMovementComponent->Velocity = Direction * meteor->ProjectileMovementComponent->InitialSpeed;
                        }
                    }
                }

                // 메테오 생성 후 상태를 Idle로 변경
                mState = EEnemyState::Idle;
                anim->animState = EEnemyState::Idle;  // 애니메이션 상태를 Idle로 변경
                isExecutingPattern = false;  // 패턴 실행 완료
            }, 0.5f, false);  // 0.5초 후 메테오 생성
    }
}

void UEnemyFSM::LaunchWhiteSphere()
{
    FVector spawnLocation = FVector(-100.0f, 100.0f, 200.0f);
    FTransform transform;
    transform.SetLocation(spawnLocation);

    AWhiteSphere* whiteSphere = GetWorld()->SpawnActor<AWhiteSphere>(AWhiteSphere::StaticClass(), transform);
    if (whiteSphere)
    {
        whiteSphere->Initialize(me);
    }
}

void UEnemyFSM::ChargeWarning(float newChargeSpeed)
{
    if (!target || !me)
    {
        UE_LOG(LogTemp, Error, TEXT("Target or me is nullptr"));
        return;
    }

    FVector targetLocation = target->GetActorLocation();
    FVector startLocation = me->GetActorLocation();
    FVector direction = (targetLocation - startLocation).GetSafeNormal();

    FRotator newRotation = direction.Rotation();
    me->SetActorRotation(newRotation);

    FVector pathCenter = (startLocation + targetLocation) / 2.0f;
    float pathLength = FVector::Distance(startLocation, targetLocation);
    FVector pathScale = FVector(pathLength / 100.0f, 1.0f, 1.0f);
    ChargingPathMesh->SetWorldLocation(pathCenter);
    ChargingPathMesh->SetWorldRotation(direction.Rotation());
    ChargingPathMesh->SetWorldScale3D(pathScale);
    ChargingPathMesh->SetVisibility(true);

    FTimerHandle UnusedHandle;
    GetWorld()->GetTimerManager().SetTimer(UnusedHandle, [this, direction, newChargeSpeed]()
        {
            ChargingPathMesh->SetVisibility(false);
            mState = EEnemyState::Charging;

            GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, [this, direction, newChargeSpeed]()
                {
                    if (!me)
                    {
                        UE_LOG(LogTemp, Error, TEXT("me is nullptr inside timer"));
                        return;
                    }

                    FVector newLocation = me->GetActorLocation() + direction * newChargeSpeed;
                    me->SetActorLocation(newLocation);
                    UE_LOG(LogTemp, Warning, TEXT("Charging started to %s"), *newLocation.ToString());

                    if (FVector::Distance(me->GetActorLocation(), target->GetActorLocation()) < 700.0f)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Charged into TempMan!"));
                        GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle);
                        me->GetCharacterMovement()->Velocity = FVector::ZeroVector;
                        mState = EEnemyState::Idle;
                        isExecutingPattern = false;  // 패턴 실행 완료
                    }
                    else if (IsCollidingWithPillar())
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Charged into a Pillar!"));
                        GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle);
                        me->GetCharacterMovement()->Velocity = FVector::ZeroVector;
                        mState = EEnemyState::Idle;
                        isExecutingPattern = false;  // 패턴 실행 완료
                    }
                }, 0.01f, true);

        }, chargePrepareTime, false);
}

bool UEnemyFSM::IsCollidingWithPillar()
{
    if (!me) return false;

    TArray<AActor*> FoundPillars;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APillar::StaticClass(), FoundPillars);

    for (AActor* Pillar : FoundPillars)
    {
        if (FVector::Distance(me->GetActorLocation(), Pillar->GetActorLocation()) < 700.0f) // 적절한 거리로 조정
        {
            Pillar->Destroy(); // 기둥을 파괴
            return true;
        }
    }

    return false;
}

void UEnemyFSM::EnterGroggyState()
{
    mState = EEnemyState::Idle;
    UE_LOG(LogTemp, Warning, TEXT("Groggy state ended, entering Idle state."));
}
