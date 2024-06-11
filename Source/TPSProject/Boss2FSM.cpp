#include "Boss2FSM.h"
#include "Boss2.h"
#include <Kismet/GameplayStatics.h>
#include "TPSProject.h"
#include <Components/CapsuleComponent.h>
#include "ElectricShock.h"
#include "TempMan.h"
#include "Boss2Anim.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Pillar.h"

UBoss2FSM::UBoss2FSM()
{
    PrimaryComponentTick.bCanEverTick = true;

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
    mState = EBoss2State::Idle;
    hp = 100; // 적절한 초기화
    isExecutingPattern = false;
}

void UBoss2FSM::BeginPlay()
{
    Super::BeginPlay();

    if (!target)
    {
        auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATempMan::StaticClass());
        target = Cast<ATempMan>(actor);
    }
    me = Cast<ABoss2>(GetOwner());
    anim = Cast<UBoss2Anim>(me->GetMesh()->GetAnimInstance());
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

void UBoss2FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!me || !target) // 'me'와 'target'이 유효한지 확인
    {
        UE_LOG(LogTemp, Error, TEXT("me or target is nullptr"));
        return;
    }

    if (!isExecutingPattern)
    {
        timeSinceLastMeteor += DeltaTime;
        timeSinceLastCharge += DeltaTime;

        if (mState == EBoss2State::Idle)
        {
            if (timeSinceLastMeteor >= meteorInterval)
            {
                mState = EBoss2State::Meteor;
                timeSinceLastMeteor = 0.0f;
            }
            else if (timeSinceLastCharge >= chargeInterval)
            {
                mState = EBoss2State::Charge;
                timeSinceLastCharge = 0.0f;
            }
        }
    }

    switch (mState)
    {
    case EBoss2State::Idle:
        IdleState();
        break;
    case EBoss2State::Move:
        MoveState();
        break;
    case EBoss2State::Attack:
        AttackState();
        break;
    case EBoss2State::Damage:
        DamageState();
        break;
    case EBoss2State::Die:
        DieState();
        break;
    case EBoss2State::Meteor:
        MeteorState();
        break;
    case EBoss2State::Charge:
        ChargeState(chargeSpeed);
        break;
    case EBoss2State::Charging:
        // Charging 상태에서는 별도의 로직을 추가하지 않음
        break;
    case EBoss2State::Groggy:
        // Groggy 상태에서는 별도의 로직을 추가하지 않음
        break;
    }
}

void UBoss2FSM::IdleState()
{
    anim->animState = mState;
    currentTime += GetWorld()->DeltaTimeSeconds;
}

void UBoss2FSM::MoveState()
{
    if (target)
    {
        FVector destination = target->GetActorLocation();
        FVector dir = destination - me->GetActorLocation();
        me->AddMovementInput(dir.GetSafeNormal());

        if (dir.Size() < attackRange)
        {
            mState = EBoss2State::Attack;
        }
    }
}

void UBoss2FSM::AttackState()
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
            mState = EBoss2State::Move;
        }
    }
}

void UBoss2FSM::DamageState()
{
    currentTime += GetWorld()->DeltaTimeSeconds;
    if (currentTime > damageDelayTime)
    {
        mState = EBoss2State::Idle;
        currentTime = 0;
    }
}

void UBoss2FSM::DieState()
{
    mState = EBoss2State::Die;
    FVector P0 = me->GetActorLocation();
    FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
    FVector P = P0 + vt;
    me->SetActorLocation(P);

    if (P.Z < -200.0f)
    {
        me->Destroy();
    }
}

void UBoss2FSM::MeteorState()
{
    if (!isExecutingPattern)  // 이미 패턴이 실행 중인지 확인
    {
        anim->animState = EBoss2State::Meteor;  // 애니메이션 상태 설정
        isExecutingPattern = true;  // 패턴 실행 중으로 설정

        // 일정 시간 대기 후 전기 충격 생성
        FTimerHandle ElectricShockSpawnHandle;
        GetWorld()->GetTimerManager().SetTimer(ElectricShockSpawnHandle, [this]()
            {
                if (target)
                {
                    for (int i = 0; i < 20; ++i)
                    {
                        FVector targetLocation = target->GetActorLocation();
                        FVector2D offset2D = FMath::RandPointInCircle(500.0f); // 500 단위의 원형 범위 내 랜덤 포인트
                        FVector offset = FVector(offset2D.X, offset2D.Y, 0); // FVector2D를 FVector로 변환
                        FVector startLocation = targetLocation + offset + FVector(0, 0, 1000.0f); // 위쪽에서 내려오도록

                        FTransform transform;
                        transform.SetLocation(startLocation);
                        AElectricShock* electricShock = GetWorld()->SpawnActor<AElectricShock>(AElectricShock::StaticClass(), transform);
                        if (electricShock)
                        {
                            electricShock->SetTargetLocation(targetLocation + offset);
                        }
                    }
                }

                // 전기 충격 생성 후 상태를 Idle로 변경
                mState = EBoss2State::Idle;
                anim->animState = EBoss2State::Idle;  // 애니메이션 상태를 Idle로 변경
                isExecutingPattern = false;  // 패턴 실행 완료
            }, 0.5f, false);  // 0.5초 후 전기 충격 생성
    }
}


void UBoss2FSM::ChargeState(float newChargeSpeed)
{
    anim->animState = mState;
    ChargeWarning(newChargeSpeed);
}

void UBoss2FSM::OnDamageProcess()
{
    hp--;

    if (hp > 0)
    {
        mState = EBoss2State::Damage;
    }
    else
    {
        mState = EBoss2State::Die;
        me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void UBoss2FSM::ChargeWarning(float newChargeSpeed)
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
            mState = EBoss2State::Charging;

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
                        mState = EBoss2State::Idle;
                        isExecutingPattern = false;  // 패턴 실행 완료
                    }
                    else if (IsCollidingWithPillar())
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Charged into a Pillar!"));
                        GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle);
                        me->GetCharacterMovement()->Velocity = FVector::ZeroVector;
                        mState = EBoss2State::Idle;
                        isExecutingPattern = false;  // 패턴 실행 완료
                    }
                }, 0.01f, true);

        }, chargePrepareTime, false);
}

bool UBoss2FSM::IsCollidingWithPillar()
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

void UBoss2FSM::EnterGroggyState()
{
    mState = EBoss2State::Idle;
    UE_LOG(LogTemp, Warning, TEXT("Groggy state ended, entering Idle state."));
}
