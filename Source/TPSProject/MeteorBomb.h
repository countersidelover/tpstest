#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeteorBomb.generated.h"

UCLASS()
class TPSPROJECT_API AMeteorBomb : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AMeteorBomb();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    class USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    class UParticleSystemComponent* ParticleSystemComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    class UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
    FVector TargetLocation;

private:
    void HandleCollisionWithTempMan(AActor* OtherActor);

    // 템프맨과의 거리를 계산하여 충돌 처리
    void CheckProximityToTempMan();

    float ProximityThreshold; // 템프맨과의 거리 임계값
};
