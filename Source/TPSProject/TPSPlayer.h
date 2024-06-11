// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	// 카메라
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* tpsCamComp;

	// HP
	float HP = 100;


	// 이동
	// 이동 속도
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float Normal = 800;

	// 이동 방향
	FVector direction;

	// 기타
	// 총알 공장
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	// 총알 파편 효과 공장
	UPROPERTY(EditAnywhere, Category = BulletEffect)
	class UParticleSystem* bulletEffectFactory;


	// UI
	// 일반 조준 크로스헤어UI 위젯
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;
	// 크로스헤어 UI 위젯 인스턴스
	class UUserWidget* _crosshairUI;



	// 바인딩

	// 좌우 회전 입력 처리
	void Turn(float value);
	// 상하 회전 입력 처리
	void LookUp(float value);
	// 좌우 이동 입력 이벤트 처리 함수
	void InputHorizontal(float value);
	// 상하 이동 입력 이벤트 처리 함수
	void InputVertical(float value);
	// 점프 입력 이벤트 처리 함수
	void InputJump();
	// 플레이어 이동 처리
	void Move();
	// 총알 발사 처리 함수
	void InputFire();

	// 스킬 & 아이템
	// Q
	void Q1_Skill();
	void Q2_Skill();
	int32 Q = 0;
	// W
	void W1_Skill();
	void W2_Skill();
	int32 W = 0;
	// E
	void E1_Skill();
	void E2_Skill();
	int32 E = 0;
	//ATPSPlayer* PlayerController;
	// R
	void R1_Skill();
	void R2_Skill();
	int32 R = 0;
	int Rrestrict = 2.5f;
	// 아이템 1
	void item1();
	float i1 = 0;
	// 아이템 2
	void item2();
	float i2 = 0;

	// 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CooldTime")		// 쿨타임 bool
		float Q_CoolTime1 = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CooldTime")		// 쿨타임 시간
		float Q_CoolTime2 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CooldTime")		// 쿨타임 bool
		float W_CoolTime1 = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CooldTime")		// 쿨타임 시간
		float W_CoolTime2 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CooldTime")		// 쿨타임 bool
		float E_CoolTime1 = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CooldTime")		// 쿨타임 시간
		float E_CoolTime2 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CooldTime")		// 쿨타임 bool
	float R_CoolTime1 = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CooldTime")		// 쿨타임 시간
	float R_CoolTime2 = 0;


	// 파티클

	UPROPERTY(EditAnywhere, Category = "SkillEffects")
	UParticleSystem* Q_Particle;

	UPROPERTY(EditAnywhere, Category = "SkillEffects")
	UParticleSystem* W_Particle;

	UPROPERTY(EditAnywhere, Category = "SkillEffects")
	UParticleSystem* E_Particle;

	UPROPERTY(EditAnywhere, Category = "SkillEffects")
	UParticleSystem* R_Particle;


	// 애니메이션

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimationAsset* N_Ani;
	void Play_N_Ani();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimationAsset* Q_Ani;
	void Play_Q_Ani();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimationAsset* W1_Ani;
	void Play_W1_Ani();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimationAsset* W2_Ani;
	void Play_W2_Ani();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimationAsset* E_Ani;
	void Play_E_Ani();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimationAsset* R1_Ani;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimationAsset* R2_Ani;
	void Play_R1_Ani();
	void Play_R2_Ani();


};
