#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElectricShock.generated.h"

UCLASS()
class TPSPROJECT_API AElectricShock : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AElectricShock();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // TargetLocation�� �����ϱ� ���� setter �Լ�
    void SetTargetLocation(FVector NewTargetLocation);

private:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    class USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UParticleSystemComponent* ParticleSystemComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UProjectileMovementComponent* ProjectileMovementComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Shock")
    float ShockSpeed = 120.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Shock")
    float ProximityThreshold = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shock", meta = (AllowPrivateAccess = "true"))
    FVector TargetLocation;

    FTimerHandle LifeSpanTimerHandle;

    void DestroyElectricShock();

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    void HandleCollisionWithTempMan(AActor* OtherActor); // ����� �Լ�


};
