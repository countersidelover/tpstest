// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossWidget.generated.h"
/**
 * 
 */
UCLASS()
class TPSPROJECT_API UBossWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	// 프로그래스 바의 값을 설정하는 함수
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetProgressBarValue(float Value);

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar;


	// 얘넨 전역 변수로 구현하는게 좋을듯
	float MaxHp = 100;
	float CurrentHp = 70;

protected:
	virtual void NativeConstruct() override;
	
};
