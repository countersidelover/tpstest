#include "Pillar.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h" // UBoxComponent를 포함합니다.
#include "UObject/ConstructorHelpers.h"

// Sets default values
APillar::APillar()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and set up the static mesh component
	PillarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarMesh"));
	RootComponent = PillarMesh;

	// Set a default mesh (you can replace the path with your actual asset path)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Architecture/Pillar_50x500.Pillar_50x500'"));
	if (MeshAsset.Succeeded())
	{
		PillarMesh->SetStaticMesh(MeshAsset.Object);
	}

	// Create and set up the collision box component
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetBoxExtent(FVector(50.0f, 50.0f, 100.0f)); // Adjust the size as needed
	CollisionBox->SetCollisionProfileName(TEXT("BlockAll"));

	// Optionally, configure the collision box to generate overlap events
	// CollisionBox->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void APillar::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
