// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"


void UBossWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (ProgressBar)
    {
        ProgressBar->SetPercent(CurrentHp / MaxHp);
    }
}


void UBossWidget::SetProgressBarValue(float Value)
{
    if (ProgressBar)
    {
        ProgressBar->SetPercent(CurrentHp/MaxHp);
    }
}