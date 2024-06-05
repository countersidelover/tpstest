#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyFSM.h"  // UEnemyFSM 포함
#include "Enemy.generated.h"

UCLASS()
class TPSPROJECT_API AEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AEnemy();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
    UEnemyFSM* fsm;  // UEnemyFSM 컴포넌트 선언
};