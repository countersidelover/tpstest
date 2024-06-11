// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NextPageWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UNextPageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* button_Next;

	UFUNCTION()
	void Next();
	
	int32 CountMap = 0;

protected:
	virtual void NativeConstruct() override;

};
