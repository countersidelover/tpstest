// Fill out your copyright notice in the Description page of Project Settings.


#include "StartWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"
#include "TPSProjectGameModeBase.h"

void UStartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	button_Start->OnClicked.AddDynamic(this, &UStartWidget::Start);

}


void UStartWidget::Start()
{
	ATPSProjectGameModeBase* GameMode = Cast<ATPSProjectGameModeBase>(UGameplayStatics::GetGameMode(this));
	GameMode->startUI->RemoveFromViewport();
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);

	GameMode->ShowSkill();


}
