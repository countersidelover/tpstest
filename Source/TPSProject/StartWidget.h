// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartWidget.generated.h"
/**
 * 
 */
UCLASS()
class TPSPROJECT_API UStartWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* button_Start;

	UFUNCTION()
	void Start();

protected:
	virtual void NativeConstruct() override;
};
