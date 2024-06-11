// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillWidget.generated.h"

/**
 *
 */
UCLASS()
class TPSPROJECT_API USkillWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// 프로그래스 바의 값을 설정하는 함수
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetProgressBarValue_HP(float Value);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetProgressBarValue_Q(float Value);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetProgressBarValue_W(float Value);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetProgressBarValue_E(float Value);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetProgressBarValue_R(float Value);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetProgressBarValue_I1(float Value);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetProgressBarValue_I2(float Value);

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HP_ProgressBar;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* Q_ProgressBar;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* W_ProgressBar;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* E_ProgressBar;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* R_ProgressBar;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* I1_ProgressBar;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* I2_ProgressBar;



	// 얘넨 전역 변수로 구현하는게 좋을듯
	float MaxHP = 100;
	float MaxQ = 3;
	float MaxW = 1.5;
	float MaxE = 8;
	float MaxR = 8;
	float Max1 = 3;
	float Max2 = 3;

	float Current_HP = 100;
	float Current_Q = 0;
	float Current_W = 0;
	float Current_E = 0;
	float Current_R = 0;
	float Current_I1;
	float Current_I2;

	float Q_CoolTime1;
	float W_CoolTime1;
	float E_CoolTime1;
	float R_CoolTime1;

	class ATPSPlayer* Player;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
