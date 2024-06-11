// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "Bullet.h"
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "EnemyFSM.h"
#include "TPSProjectGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Particles/ParticleSystemComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimationAsset.h"
#include "GameFramework/Actor.h"
#include "SkillWidget.h"

ATPSPlayer::ATPSPlayer()
{

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	// TPS 카메라
	// SpringArm 컴포넌트 붙이기
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = true;
	// Camera 컴포넌트 붙이기
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;
	// 2단 점프
	JumpMaxCount = 2;
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// 일반 조준 UI
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	_crosshairUI->AddToViewport();



	
	ATPSProjectGameModeBase* currentGameMode = Cast<ATPSProjectGameModeBase>(GetWorld()->GetAuthGameMode());
	if (currentGameMode != nullptr)
	{
		currentGameMode->ShowStart();
	}
	
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 움직임

	if (Q == 0 && W == 0 && E == 0 && R == 0)		// 스킬이 아무것도 눌러지지 않았을 때
		Move();

	if (Q == 0 && W == 0 && E == 1 && E_CoolTime1 == 0 && R == 0)	//  e가 눌러졌지만 쿨타임일 때
		Move();
	if (Q == 0 && W == 0 && E == 0 && R == 1 && R_CoolTime1 == 0)	//  r가 눌러졌지만 쿨타임일 때
		Move();
	if (Q == 0 && W == 1 && W_CoolTime1 == 0 && E == 0 && R == 0)	//  w가 눌러졌지만 쿨타임일 때
		Move();
	if (Q == 1 && Q_CoolTime1 == 0 && W == 0 && E == 0 && R == 0)	//  q가 눌러졌지만 쿨타임일 때
		Move();


	// 스킬

	if (Q == 1 && Q_CoolTime1 == 1)
	{
		InputFire();
		Play_Q_Ani();
		Q_CoolTime1 = 0;
		Q = 0;
	}

	if (W == 1 && W_CoolTime1 == 1)		// W를 눌렀을 때 3.16초 정신 집중에 성공하면 공격
	{

		static float Timer = 0.0f;
		Timer += DeltaTime;
		if (Timer >= 3.16f && E == 0 && R == 0)
		{
			// Attack!
			Timer = 0;

			InputFire();
			W = 0;	// W가 더이상 시행되지 않도록
			W_CoolTime1 = 0;

		}

		// 다른 스킬을 쓰면 끊어지게 만듬
		if (Q==1 && Q_CoolTime1 == 1|| E == 1 && E_CoolTime1 == 1 || R == 1 && R_CoolTime1 == 1)
		{
			W2_Skill();
		}
	}

	if (E == 1 && E_CoolTime1 == 1)
	{
		// 1. 방향
		FVector dir = FTransform(GetControlRotation()).TransformVector(direction);
		// 2. 정규화
		dir.Normalize();

		// 파티클
		// 이동하기 전 위치에 파티클이 생성된다.
		FVector nowLocation = GetActorLocation();
		// 파티클 인스턴스 생성
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), E_Particle, nowLocation);

		// 3. 이동할 위치 좌표를 구한다(p = p0 + vt).
		FVector newLocation = GetActorLocation() + dir * 1000;

		//파티클 인스턴스 생성
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), E_Particle, newLocation);

		// 4. 현재 액터의 위치 좌표를 앞에서 구한 새 좌표로 갱신한다.
		SetActorLocation(newLocation, true);
		E = 0;
		// 애니메이션
		Play_E_Ani();
		// 쿨타임
		E_CoolTime1 = 0;	// E_CoolTime1이 0이면 사용 불가능
	}

	if (R == 1 && R_CoolTime1 == 1 )
	{
		// R의 지속 시간 설정
		static float Timer = 0.0f;
		Timer += DeltaTime;
		if (Timer >= Rrestrict)	// Rrestrict만큼 지속한다.
		{
			Timer = 0;
			R_CoolTime1 = 0;	// R_CoolTime1이 0이면 사용 불가능
		}

		// 1. 방향
		FVector dir = FTransform(GetControlRotation()).TransformVector(direction);
		// 2. 정규화
		dir.Normalize();

		// 파티클
		// 이동하기 전 위치에 파티클이 생성된다.
		FVector nowLocation = GetActorLocation();
		// 파티클 인스턴스 생성
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), R_Particle, nowLocation);

		// 3. 이동할 위치 좌표를 구한다(p = p0 + vt).
		FVector newLocation = GetActorLocation() + dir * 70;

		//파티클 인스턴스 생성
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), R_Particle, newLocation);

		// 4. 현재 액터의 위치 좌표를 앞에서 구한 새 좌표로 갱신한다.
		SetActorLocation(newLocation, true);

	}


	static float Timer = 0.0f;
	Timer += DeltaTime;
	if (Timer >= 1.0f)
	{
		class ATPSProjectGameModeBase* currentGameMode = Cast<ATPSProjectGameModeBase>(GetWorld()->GetAuthGameMode());
		if (currentGameMode != nullptr)
		{
			//currentGameMode->ShowNext();
		}
		Timer = 0.0f; // 타이머 초기화
	}
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATPSPlayer::LookUp);
	// 좌우 입력 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATPSPlayer::InputHorizontal);
	// 상하 입력 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATPSPlayer::InputVertical);
	// 점프 입력 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATPSPlayer::InputJump);
	// 총알 발사 이벤트 처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::InputFire);


	// Q
	PlayerInputComponent->BindAction(TEXT("Q_Skill"), IE_Pressed, this, &ATPSPlayer::Q1_Skill);
	PlayerInputComponent->BindAction(TEXT("Q_Skill"), IE_Released, this, &ATPSPlayer::Q2_Skill);
	// W(정신 집중)
	PlayerInputComponent->BindAction(TEXT("W_Skill"), IE_Pressed, this, &ATPSPlayer::W1_Skill);
	PlayerInputComponent->BindAction(TEXT("W_Skill"), IE_Released, this, &ATPSPlayer::W2_Skill);
	// E
	PlayerInputComponent->BindAction(TEXT("E_Skill"), IE_Pressed, this, &ATPSPlayer::E1_Skill);
	PlayerInputComponent->BindAction(TEXT("E_Skill"), IE_Released, this, &ATPSPlayer::E2_Skill);
	// R
	PlayerInputComponent->BindAction(TEXT("R_Skill"), IE_Pressed, this, &ATPSPlayer::R1_Skill);
	PlayerInputComponent->BindAction(TEXT("R_Skill"), IE_Released, this, &ATPSPlayer::R2_Skill);

	// 아이템
	PlayerInputComponent->BindAction(TEXT("item1"), IE_Pressed, this, &ATPSPlayer::item1);
	PlayerInputComponent->BindAction(TEXT("item2"), IE_Pressed, this, &ATPSPlayer::item2);
}

void ATPSPlayer::Turn(float value)
{
	AddControllerYawInput(value);
}
void ATPSPlayer::LookUp(float value)
{
	AddControllerPitchInput(value);
}

// 좌우
void ATPSPlayer::InputHorizontal(float value)
{
	direction.Y = value;
}
// 상하
void ATPSPlayer::InputVertical(float value)
{
	direction.X = value;
}

void ATPSPlayer::InputJump()
{
	Jump();
}

// 이동
void ATPSPlayer::Move()
{
	direction = FTransform(GetControlRotation()).TransformVector(direction);
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void ATPSPlayer::InputFire()
{

	// LineTrace 의 시작 위치
	FVector startPos = tpsCamComp->GetComponentLocation();
	// LineTrace 의 종료 위치
	FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000;
	// LineTrace 의 충돌 정보를 담을 변수
	FHitResult hitInfo;
	// 충돌 옵션 설정 변수
	FCollisionQueryParams params;
	// 자기 자신(플레이어) 는 충돌에서 제외
	params.AddIgnoredActor(this);
	// Channel 필터를 이용한 LineTrace 충돌 검출(충돌 정보, 시작 위치, 종료 위치, 검출 채널, 충돌 옵션)
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

	float SphereRadius = 50.0f;
	bool bbHit = GetWorld()->SweepSingleByChannel(hitInfo, startPos, endPos, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(SphereRadius), params);

	// Q 스킬
	if ( Q == 1 && Q_CoolTime1 == 1)
	{
		UE_LOG(LogTemp, Display, TEXT("success"));

		if (bbHit) {
			// 총알 파편 효과 트랜스폼
			FTransform bT;
			// 부딪힌 위치 할당
			bT.SetLocation(hitInfo.ImpactPoint);
			// 총알 파편 효과 인스턴스 생성
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Q_Particle, bT);

			// 부딪힌 대상이 적인지 판단하기
			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (enemy)
			{
				UE_LOG(LogTemp, Display, TEXT("In Area, enemy hp - 10"));

				//auto enemyFSM = Cast<UEnemyFSM>(enemy);
				//enemyFSM->OnDamageProcess();
			}
		}
	}
	// W 스킬
	if (W == 1 && W_CoolTime1 == 1)
	{
		if (bbHit) {
			UE_LOG(LogTemp, Display, TEXT("success"));
			// 총알 파편 효과 트랜스폼
			FTransform bT;
			// 부딪힌 위치 할당
			bT.SetLocation(hitInfo.ImpactPoint);
			// 총알 파편 효과 인스턴스 생성
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), W_Particle, bT);

			// 부딪힌 대상이 적인지 판단하기
			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (enemy)
			{
				//auto enemyFSM = Cast<UEnemyFSM>(enemy);
				//enemyFSM->OnDamageProcess();
			}
		}
	}

	// 평타 사용 시
	else
	{
		Play_N_Ani();
		// LineTrace가 부딪혔을 때
		if (bHit)
		{
			// 충돌 처리 -> 총알 파편 효과 재생
			// 총알 파편 효과 트랜스폼
			FTransform bulletTrans;
			// 부딪힌 위치 할당
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			// 총알 파편 효과 인스턴스 생성
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);
		}
	}
}

// 스킬


void ATPSPlayer::Q1_Skill()
{
	Q_CoolTime2 = 3.0f;
	Q = 1;
}
void ATPSPlayer::Q2_Skill()
{
	Q = 0;
}
void ATPSPlayer::W1_Skill()
{
	// 애니메이션
	if (W_CoolTime1 == 1)
		Play_W1_Ani();

	W_CoolTime2 = 1.5f;
	W = 1;	// W가 1이면 Tick함수의 Move()가 발동 X

}
void ATPSPlayer::W2_Skill()
{
	W = 0;	// Move()를 다시 발동되게 만듬

	W_CoolTime1 = 0;
	Play_W2_Ani();

}
void ATPSPlayer::E1_Skill()
{
	E_CoolTime2 = 8.0f;
	E = 1;
}
void ATPSPlayer::E2_Skill()
{
	E = 0;
}

void ATPSPlayer::R1_Skill()
{
	R = 1;
	R_CoolTime2 = 8.0f;
	// 애니메이션
	if (R_CoolTime1 == 1)
		Play_R1_Ani();
}
void ATPSPlayer::R2_Skill()
{
	R = 0;

	// 애니메이션
	Play_R2_Ani();
	R_CoolTime1 = 0;
}
void ATPSPlayer::item1()
{
	if (i1 < 3) {
		HP += 10;
		i1++;
	}
}
void ATPSPlayer::item2()
{
	if (i2 < 3) {
		UE_LOG(LogTemp, Display, TEXT("Enemy Stun!"));
		i2++;
	}
}

// 애니메이션
void ATPSPlayer::Play_N_Ani()
{
	GetMesh()->PlayAnimation(N_Ani, false);
}
void ATPSPlayer::Play_Q_Ani()
{
	GetMesh()->PlayAnimation(Q_Ani, false);
}
void ATPSPlayer::Play_E_Ani()
{
	GetMesh()->PlayAnimation(E_Ani, false);
}
void ATPSPlayer::Play_W1_Ani()
{
	GetMesh()->PlayAnimation(W1_Ani, false);
}
void ATPSPlayer::Play_W2_Ani()
{
	GetMesh()->PlayAnimation(W2_Ani, false);
}
void ATPSPlayer::Play_R1_Ani()
{
	GetMesh()->PlayAnimation(R1_Ani, true);
}
void ATPSPlayer::Play_R2_Ani()
{
	GetMesh()->PlayAnimation(R2_Ani, false);
}
