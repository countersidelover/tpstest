#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Boss2.generated.h"

UCLASS()
class TPSPROJECT_API ABoss2 : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ABoss2();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // FSM component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FSM")
    class UBoss2FSM* fsm;
};

