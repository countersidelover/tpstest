#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Boss2FSM.generated.h"

UENUM(BlueprintType)
enum class EBoss2State : uint8
{
    Idle,
    Move,
    Attack,
    Damage,
    Die,
    Meteor,
    Charge,
    Charging,
    Groggy
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TPSPROJECT_API UBoss2FSM : public UActorComponent
{
    GENERATED_BODY()

public:
    UBoss2FSM();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    void IdleState();
    void MoveState();
    void AttackState();
    void DamageState();
    void DieState();
    void MeteorState();
    void ChargeState(float newChargeSpeed);
    void LaunchElectricShocks();
    void ChargeWarning(float newChargeSpeed);
    bool IsCollidingWithPillar();
    void EnterGroggyState();
    void OnDamageProcess();

    UPROPERTY()
    class ABoss2* me;
    UPROPERTY()
    class ATempMan* target;
    UPROPERTY()
    class UBoss2Anim* anim;
    UPROPERTY()
    class UStaticMeshComponent* ChargingPathMesh;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
    float meteorInterval = 3.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
    float chargeInterval = 8.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
    float chargeSpeed = 30.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
    float chargePrepareTime = 2.0f;

private:
    float timeSinceLastMeteor = 0.0f;
    float timeSinceLastCharge = 0.0f;
    float currentTime = 0.0f;
    float attackDelayTime = 2.0f;
    float damageDelayTime = 1.0f;
    float dieSpeed = 100.0f;
    float attackRange = 200.0f;

    bool bWhiteSphereExecuted = false;
    bool isExecutingPattern = false;

    EBoss2State mState = EBoss2State::Idle;

    UPROPERTY()
    class UStaticMesh* ChargingPathStaticMesh;
    UPROPERTY()
    class UMaterial* ChargingPathMaterial;

    FTimerHandle ChargeTimerHandle;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 hp = 100;  // 'hp' 변수 초기화
};
