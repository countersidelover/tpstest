#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

// 사용할 상태 정의
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
    Idle,
    Move,
    Attack,
    Damage,
    Die,
    Meteor,
    WhiteSphere, 
    Charge
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TPSPROJECT_API UEnemyFSM : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UEnemyFSM();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    // 상태 변수
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
    EEnemyState mState = EEnemyState::WhiteSphere; // 초기 상태를 WhiteSphere로 설정

    // 대기 상태
    void IdleState();
    // 이동 상태
    void MoveState();
    // 공격 상태
    void AttackState();
    // 피격 상태
    void DamageState();
    // 죽음 상태
    void DieState();
    // 메테오 상태
    void MeteorState();
    // WhiteSphere 상태
    void WhiteSphereState();
    void ChargeState();

    // 대기 시간
    UPROPERTY(EditDefaultsOnly, Category = FSM)
    float idleDelayTime = 2;
    // 경과 시간
    float currentTime = 0;
    // 메테오 스킬 사용 주기
    UPROPERTY(EditDefaultsOnly, Category = FSM)
    float meteorInterval = 10.0f;
    // WhiteSphere 스킬 사용 주기
    UPROPERTY(EditDefaultsOnly, Category = FSM)
    float whiteSphereInterval = 15.0f; // WhiteSphere 사용 주기
    // 메테오 스킬 경고 시간
    UPROPERTY(EditDefaultsOnly, Category = FSM)
    float meteorWarningTime = 3.0f;

    // 타깃
    UPROPERTY(EditAnywhere, Category = FSM)
    class ATempMan* target;

    // 소유 액터
    UPROPERTY()
    class AEnemy* me;

    // 공격 범위
    UPROPERTY(EditAnywhere, Category = FSM)
    float attackRange = 150.0f;

    // 공격 대기 시간
    UPROPERTY(EditAnywhere, Category = FSM)
    float attackDelayTime = 2.0f;

    // 피격 알림 이벤트 함수
    void OnDamageProcess();

    // 체력
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
    int32 hp = 3;
    // 피격 대기 시간
    UPROPERTY(EditAnywhere, Category = FSM)
    float damageDelayTime = 2.0f;

    // 아래로 사라지는 속도
    UPROPERTY(EditAnywhere, Category = FSM)
    float dieSpeed = 50.0f;

    // 메테오 스킬을 위한 함수들
    void LaunchMeteors();
    // WhiteSphere 스킬을 위한 함수들
    void LaunchWhiteSphere();
    void ChargeWarning();
    //화이트 스피어 
    void Initialize(class AEnemy* InEnemy);


private:
    UStaticMesh* WarningCircleMesh;
    UStaticMesh* MeteorMesh;
    UStaticMesh* WhiteSphereMesh; // WhiteSphere 메쉬


    bool bWhiteSphereExecuted; // WhiteSphere 상태 한 번 실행 여부
};
