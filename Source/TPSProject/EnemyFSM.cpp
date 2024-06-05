#include "EnemyFSM.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include "TPSProject.h"
#include <Components/CapsuleComponent.h>
#include "MeteorBomb.h"
#include "WhiteSphere.h"
#include "TempMan.h"
#include "GameFramework/ProjectileMovementComponent.h" // 추가: UProjectileMovementComponent 포함

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
    PrimaryComponentTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
    if (SphereMesh.Succeeded())
    {
        WhiteSphereMesh = SphereMesh.Object;
    }

    bWhiteSphereExecuted = false; // WhiteSphere 상태 한 번 실행 여부 초기화
}

// Called when the game starts
void UEnemyFSM::BeginPlay()
{
    Super::BeginPlay();

    if (!target)
    {
        auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATempMan::StaticClass());
        target = Cast<ATempMan>(actor);
    }
    me = Cast<AEnemy>(GetOwner());
}

// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bWhiteSphereExecuted)
    {
        WhiteSphereState();
        bWhiteSphereExecuted = true;
        mState = EEnemyState::Idle;
        return;
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
        ChargeState();
        break;
    }
}

// 대기 상태
void UEnemyFSM::IdleState()
{
    currentTime += GetWorld()->DeltaTimeSeconds;
    if (currentTime > idleDelayTime)
    {
        mState = EEnemyState::Meteor;
        currentTime = 0;
    }

    // 메테오 스킬 사용 주기 체크
    if (currentTime > meteorInterval)
    {
        mState = EEnemyState::Meteor;
        currentTime = 0;
    }

    // WhiteSphere 스킬 사용 주기 체크
    if (currentTime > whiteSphereInterval)
    {
        mState = EEnemyState::WhiteSphere;
        currentTime = 0;
    }
}

// 이동 상태
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

// 공격 상태
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

// 피격 상태
void UEnemyFSM::DamageState()
{
    currentTime += GetWorld()->DeltaTimeSeconds;
    if (currentTime > damageDelayTime)
    {
        mState = EEnemyState::Idle;
        currentTime = 0;
    }
}

// 죽음 상태
void UEnemyFSM::DieState()
{
    FVector P0 = me->GetActorLocation();
    FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
    FVector P = P0 + vt;
    me->SetActorLocation(P);

    if (P.Z < -200.0f)
    {
        me->Destroy();
    }
}

// 메테오 상태
void UEnemyFSM::MeteorState()
{
    // 메테오를 발사하는 함수 호출
    LaunchMeteors();
    mState = EEnemyState::Idle;
}

// WhiteSphere 상태
void UEnemyFSM::WhiteSphereState()
{
    // WhiteSphere를 발사하는 함수 호출
    LaunchWhiteSphere();
    mState = EEnemyState::Idle;
}


void UEnemyFSM::ChargeState()
{
    // WhiteSphere를 발사하는 함수 호출
    ChargeWarning();
    mState = EEnemyState::Idle;
}

// 피격 알림 이벤트 함수
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
        for (int i = 0; i < 10; ++i)
        {
            FVector targetLocation = target->GetActorLocation(); // 템프맨의 위치로 설정
            FVector startLocation = me->GetActorLocation() + FMath::VRand() * 300.0f; // 적의 주위 랜덤 위치에서 시작
            FTransform transform;
            transform.SetLocation(startLocation);
            AMeteorBomb* meteor = GetWorld()->SpawnActor<AMeteorBomb>(AMeteorBomb::StaticClass(), transform);
            if (meteor)
            {
                meteor->TargetLocation = targetLocation;
                // 목표 위치로 향하는 속도 설정
                FVector Direction = (meteor->TargetLocation - startLocation).GetSafeNormal();
                meteor->ProjectileMovementComponent->Velocity = Direction * meteor->ProjectileMovementComponent->InitialSpeed;
            }
        }
    }
}
void UEnemyFSM::LaunchWhiteSphere()
{
    FVector spawnLocation = FVector(-100.0f, 100.0f, 200.0f); // 맵 밖의 초기 위치 설정
    FTransform transform;
    transform.SetLocation(spawnLocation);

    AWhiteSphere* whiteSphere = GetWorld()->SpawnActor<AWhiteSphere>(AWhiteSphere::StaticClass(), transform);
    if (whiteSphere)
    {
        whiteSphere->Initialize(me); // Initialize 함수에 하나의 인수만 전달
    }
}


void UEnemyFSM::ChargeWarning()
{
    //1.tageting tempman
    //2. show charging path that go through and 3second for prepaing
    //3. charge start if craSH with temp man? do nothing yet if crash with pillar     

}

