// Fill out your copyright notice in the Description page of Project Settings.


#include "NextPageWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"
#include "TPSProjectGameModeBase.h"


void UNextPageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	button_Next->OnClicked.AddDynamic(this, &UNextPageWidget::Next);

}


void UNextPageWidget::Next()
{

	// 여기서 글로벌 변수 CountMap을 제안한다.
	// 한 페이지가 끝날 때마다 CountMap++;해주면 끝이다.
	// if (CountMap == 3) UGameplayStatics::OpenLevel(GetWorld(), "Boss3"); 이런 식으로

	CountMap++;

	if(CountMap == 1)
		UGameplayStatics::OpenLevel(GetWorld(), "Map1");
	if(CountMap == 2)
		UGameplayStatics::OpenLevel(GetWorld(), "Map2");
	
	// 안된다. 이유: 버튼은 1회용이였다...... 재활용할 순 없나? 다음 시간에
	


	UGameplayStatics::SetGamePaused(GetWorld(), false);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);

}
