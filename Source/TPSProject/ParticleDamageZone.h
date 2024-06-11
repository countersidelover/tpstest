#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParticleDamageZone.generated.h"

UCLASS()
class TPSPROJECT_API AParticleDamageZone : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AParticleDamageZone();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // 영역의 좌표 설정
    void SetZoneCoordinates(FVector2D Start, FVector2D End);

    // 플레이어에게 주는 데미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float DamageAmount;

private:
    FVector2D ZoneStart;
    FVector2D ZoneEnd;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UParticleSystemComponent* ParticleSystemComponent;

    void CheckDamageToPlayer();

    UFUNCTION()
    void ApplyDamage(AActor* OtherActor);
};
