#include "ParticleDamageZone.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TempMan.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AParticleDamageZone::AParticleDamageZone()
{
    PrimaryActorTick.bCanEverTick = true;

    ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
    RootComponent = ParticleSystemComponent;
    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Fire.P_Fire'"));
    if (ParticleAsset.Succeeded())
    {
        ParticleSystemComponent->SetTemplate(ParticleAsset.Object);
    }

    DamageAmount = 10.0f;
}

// Called when the game starts or when spawned
void AParticleDamageZone::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AParticleDamageZone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    CheckDamageToPlayer();
}

void AParticleDamageZone::SetZoneCoordinates(FVector2D Start, FVector2D End)
{
    ZoneStart = Start;
    ZoneEnd = End;
}

void AParticleDamageZone::CheckDamageToPlayer()
{
    ATempMan* TempMan = Cast<ATempMan>(UGameplayStatics::GetActorOfClass(GetWorld(), ATempMan::StaticClass()));
    if (TempMan)
    {
        FVector PlayerLocation = TempMan->GetActorLocation();
        FVector2D PlayerLocation2D(PlayerLocation.X, PlayerLocation.Y);

        if (PlayerLocation2D.X >= ZoneStart.X && PlayerLocation2D.X <= ZoneEnd.X &&
            PlayerLocation2D.Y >= ZoneStart.Y && PlayerLocation2D.Y <= ZoneEnd.Y)
        {
            ApplyDamage(TempMan);
        }
    }
}

void AParticleDamageZone::ApplyDamage(AActor* OtherActor)
{
    UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, nullptr, this, nullptr);
}
