#include "WhiteSphere.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy.h"

// Sets default values
AWhiteSphere::AWhiteSphere()
{
    PrimaryActorTick.bCanEverTick = true;

    // 충돌체 컴포넌트 생성 및 설정
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(50.0f);
    RootComponent = CollisionComponent;
    CollisionComponent->OnComponentHit.AddDynamic(this, &AWhiteSphere::OnHit); // 충돌 이벤트 바인딩

    // 충돌 설정
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
    CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

    // 메쉬 컴포넌트 생성 및 설정
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
    if (SphereMesh.Succeeded())
    {
        MeshComponent->SetStaticMesh(SphereMesh.Object);
    }

    // 프로젝트 움직임 컴포넌트 생성 및 설정
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);

    // 속도를 천천히 이동하도록 조절
    ProjectileMovementComponent->InitialSpeed = 50.0f;
    ProjectileMovementComponent->MaxSpeed = 50.0f;

    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = false;
    ProjectileMovementComponent->ProjectileGravityScale = 0.0f; // 중력 영향 없음
}

// Called when the game starts or when spawned
void AWhiteSphere::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AWhiteSphere::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Enemy)
    {
        FVector Direction = (Enemy->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
    }
}

void AWhiteSphere::Initialize(AEnemy* InEnemy)
{
    this->Enemy = InEnemy;
}

void AWhiteSphere::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        if (OtherActor == Enemy)
        {
            // 에네미와 충돌하면 템프맨 사망 처리 (주석 처리)
            // if (ATempMan* TempMan = Cast<ATempMan>(Target))
            // {
            //     TempMan->Die();
            // }
        }
        else if (OtherActor == Enemy)
        {
            // 에네미와 충돌하면 구슬이 사라짐
            Destroy();
        }
    }
}
