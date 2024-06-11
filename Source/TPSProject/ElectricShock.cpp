#include "ElectricShock.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TempMan.h"
#include "Kismet/GameplayStatics.h"

AElectricShock::AElectricShock()
{
    PrimaryActorTick.bCanEverTick = true;

    // 충돌체 컴포넌트 생성 및 설정
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(50.0f);
    RootComponent = CollisionComponent;
    CollisionComponent->OnComponentHit.AddDynamic(this, &AElectricShock::OnHit); // 충돌 이벤트 바인딩

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

    // 프로젝타일 무브먼트 컴포넌트 생성 및 설정
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = ShockSpeed;
    ProjectileMovementComponent->MaxSpeed = ShockSpeed;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = false;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f; // 중력 비활성화

    // 수명 설정
    InitialLifeSpan = 15.0f; // 15초 후 삭제
}

void AElectricShock::BeginPlay()
{
    Super::BeginPlay();

    // 타겟 위치로 이동
    FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
    ProjectileMovementComponent->Velocity = Direction * ShockSpeed;
}

void AElectricShock::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // TempMan과의 거리 확인
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

void AElectricShock::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this && OtherActor != GetOwner())
    {
        if (OtherActor->IsA<ATempMan>())  // 템프맨과 충돌 시
        {
            HandleCollisionWithTempMan(OtherActor);
        }
    }
}

void AElectricShock::SetTargetLocation(FVector NewTargetLocation)
{
    TargetLocation = NewTargetLocation;

    // 타겟 위치로 이동 설정
    if (ProjectileMovementComponent)
    {
        FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
        ProjectileMovementComponent->Velocity = Direction * ShockSpeed;
    }
}

void AElectricShock::HandleCollisionWithTempMan(AActor* OtherActor)
{
    // 충돌체 컴포넌트를 제거하여 다른 오브젝트와의 충돌을 방지합니다.
    CollisionComponent->DestroyComponent();
    // 파티클 시스템 컴포넌트도 제거할 수 있습니다.
    ParticleSystemComponent->DestroyComponent();

    // 마지막으로, 자신을 삭제합니다.
    Destroy();
}
