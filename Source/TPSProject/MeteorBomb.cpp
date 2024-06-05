#include "MeteorBomb.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

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
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
    CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

    // 메쉬 컴포넌트 생성 및 설정
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeteorMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
    if (MeteorMesh.Succeeded())
    {
        MeshComponent->SetStaticMesh(MeteorMesh.Object);
    }

    // 프로젝트 움직임 컴포넌트 생성 및 설정
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = 600.0f;  // 속도 조절
    ProjectileMovementComponent->MaxSpeed = 600.0f;  // 속도 조절
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = false;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f; // 중력 영향 없음
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
}

UStaticMeshComponent* AMeteorBomb::GetMeshComponent() const
{
    return MeshComponent;
}

void AMeteorBomb::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // 충돌 시 사라지도록 처리
    if (OtherActor && OtherActor != this && OtherActor != GetOwner())
    {
        Destroy();
    }
}
