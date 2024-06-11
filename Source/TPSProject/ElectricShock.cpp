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

    // �浹ü ������Ʈ ���� �� ����
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(50.0f);
    RootComponent = CollisionComponent;
    CollisionComponent->OnComponentHit.AddDynamic(this, &AElectricShock::OnHit); // �浹 �̺�Ʈ ���ε�

    // �浹 ����
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    // ��ƼŬ �ý��� ������Ʈ ���� �� ����
    ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
    ParticleSystemComponent->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));
    if (ParticleAsset.Succeeded())
    {
        ParticleSystemComponent->SetTemplate(ParticleAsset.Object);
    }

    // ������Ÿ�� �����Ʈ ������Ʈ ���� �� ����
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = ShockSpeed;
    ProjectileMovementComponent->MaxSpeed = ShockSpeed;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = false;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f; // �߷� ��Ȱ��ȭ

    // ���� ����
    InitialLifeSpan = 15.0f; // 15�� �� ����
}

void AElectricShock::BeginPlay()
{
    Super::BeginPlay();

    // Ÿ�� ��ġ�� �̵�
    FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
    ProjectileMovementComponent->Velocity = Direction * ShockSpeed;
}

void AElectricShock::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // TempMan���� �Ÿ� Ȯ��
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
        if (OtherActor->IsA<ATempMan>())  // �����ǰ� �浹 ��
        {
            HandleCollisionWithTempMan(OtherActor);
        }
    }
}

void AElectricShock::SetTargetLocation(FVector NewTargetLocation)
{
    TargetLocation = NewTargetLocation;

    // Ÿ�� ��ġ�� �̵� ����
    if (ProjectileMovementComponent)
    {
        FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
        ProjectileMovementComponent->Velocity = Direction * ShockSpeed;
    }
}

void AElectricShock::HandleCollisionWithTempMan(AActor* OtherActor)
{
    // �浹ü ������Ʈ�� �����Ͽ� �ٸ� ������Ʈ���� �浹�� �����մϴ�.
    CollisionComponent->DestroyComponent();
    // ��ƼŬ �ý��� ������Ʈ�� ������ �� �ֽ��ϴ�.
    ParticleSystemComponent->DestroyComponent();

    // ����������, �ڽ��� �����մϴ�.
    Destroy();
}
