#include "MeteorBomb.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TempMan.h"
#include "Kismet/GameplayStatics.h" // GameplayStatics.h 헤더 파일 추가

// Sets default values
AMeteorBomb::AMeteorBomb()
{
    PrimaryActorTick.bCanEverTick = true;

    // 충돌체 컴포넌트 생성 및 설정
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(50.0f);
    RootComponent = CollisionComponent;
    CollisionComponent->OnComponentHit.AddDynamic(this, &AMeteorBomb::OnHit); // 충돌 이벤트 바인딩

    // 충돌 설정
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    // 파티클 시스템 컴포넌트 생성 및 설정
    ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
    ParticleSystemComponent->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));
    if (ParticleAsset.Succeeded())
    {
        ParticleSystemComponent->SetTemplate(ParticleAsset.Object);
    }

    // 프로젝트 움직임 컴포넌트 생성 및 설정
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = 600.0f;  // 속도 조절
    ProjectileMovementComponent->MaxSpeed = 600.0f;  // 속도 조절
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = false;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f; // 중력 영향 없음

    // 수명 설정
    InitialLifeSpan = 20.0f; // 20초 후 삭제

    // 거리 임계값 설정 (예: 100.0f)
    ProximityThreshold = 100.0f;
}

// Called when the game starts or when spawned
void AMeteorBomb::BeginPlay()
{
    Super::BeginPlay();

    // 목표 위치로 향하는 속도 설정
    FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
    ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
}

// Called every frame
void AMeteorBomb::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 템프맨과의 거리 검사
    CheckProximityToTempMan();
}

void AMeteorBomb::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this && OtherActor != GetOwner())
    {
        if (OtherActor->IsA<ATempMan>())  // 템프맨과 충돌 시
        {
            HandleCollisionWithTempMan(OtherActor);
        }
    }
}

void AMeteorBomb::HandleCollisionWithTempMan(AActor* OtherActor)
{
    // 특정 처리 (주석 처리 해놓음)
    // OtherActor->TakeDamage(DamageAmount, DamageEvent, GetInstigatorController(), this);

    // 충돌체 컴포넌트를 제거하여 다른 오브젝트와의 충돌을 방지합니다.
    CollisionComponent->DestroyComponent();
    // 파티클 시스템 컴포넌트도 제거할 수 있습니다.
    ParticleSystemComponent->DestroyComponent();

    // 마지막으로, 자신을 삭제합니다.
    Destroy();
}

void AMeteorBomb::CheckProximityToTempMan()
{
    // 월드에서 템프맨을 찾습니다.
    ATempMan* TempMan = Cast<ATempMan>(UGameplayStatics::GetActorOfClass(GetWorld(), ATempMan::StaticClass()));
    if (TempMan)
    {
        float DistanceToTempMan = FVector::Distance(GetActorLocation(), TempMan->GetActorLocation());
        if (DistanceToTempMan < ProximityThreshold)
        {
            HandleCollisionWithTempMan(TempMan);
        }
    }
}
