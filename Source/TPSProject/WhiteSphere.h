#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WhiteSphere.generated.h"

UCLASS()
class TPSPROJECT_API AWhiteSphere : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AWhiteSphere();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // 충돌체 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USphereComponent* CollisionComponent;

    // 메쉬 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* MeshComponent;

    // 프로젝트 움직임 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    class UProjectileMovementComponent* ProjectileMovementComponent;

    // 초기화 함수
    void Initialize(class AEnemy* InEnemy);

private:
    AEnemy* Enemy;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
