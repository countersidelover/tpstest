// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "TPSPlayer.h"

void USkillWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Player 객체를 가져옴
    Player = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
    if (!Player)
    {
        UE_LOG(LogTemp, Error, TEXT("Player not found"));
        return;
    }

    Current_I1 = Player->i1;
    Current_I2 = Player->i2;
    Q_CoolTime1 = Player->Q_CoolTime1;
    W_CoolTime1 = Player->W_CoolTime1;
    E_CoolTime1 = Player->E_CoolTime1;
    R_CoolTime1 = Player->R_CoolTime1;
    Current_HP = Player->HP;

    if (HP_ProgressBar)
    {
        HP_ProgressBar->SetPercent(Current_Q / MaxQ);
    }
    if (Q_ProgressBar)
    {
        Q_ProgressBar->SetPercent(Current_Q / MaxQ);
    }
    if (W_ProgressBar)
    {
        W_ProgressBar->SetPercent(Current_W / MaxW);
    }
    if (E_ProgressBar)
    {
        E_ProgressBar->SetPercent(Current_E / MaxE);
    }
    if (R_ProgressBar)
    {
        R_ProgressBar->SetPercent(Current_R / MaxR);
    }
    if (I1_ProgressBar)
    {
        I1_ProgressBar->SetPercent(Current_I1 / Max1);
    }
    if (I2_ProgressBar)
    {
        I2_ProgressBar->SetPercent(Current_I2 / Max2);
    }
}

void USkillWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!Player)
    {
        UE_LOG(LogTemp, Error, TEXT("Player not found"));
        return;
    }

    Current_HP = Player->HP;
    Current_I1 = Player->i1;
    Current_I2 = Player->i2;
    Q_CoolTime1 = Player->Q_CoolTime1;
    W_CoolTime1 = Player->W_CoolTime1;
    E_CoolTime1 = Player->E_CoolTime1;
    R_CoolTime1 = Player->R_CoolTime1;
    

    if (Q_CoolTime1 == 0)		// Q_CoolTime1이 0일땐 사용 불가능
    {

        static float Timer = 0.0f;
        Timer += InDeltaTime;
        Current_Q = Timer;
        if (Timer >= MaxQ)
        {
            Timer = 0;
            Current_Q = Timer;
            Player->Q_CoolTime1 = 1;    // Q_CoolTime1이 1일땐 사용 가능
        }
    }

    if (W_CoolTime1 == 0)		// W_CoolTime1이 0일땐 사용 불가능
    {

        static float Timer = 0.0f;
        Timer += InDeltaTime;
        Current_W = Timer;
        if (Timer >= MaxW)
        {
            Timer = 0;
            Current_W = Timer;
            Player->W_CoolTime1 = 1;    // W_CoolTime1이 1일땐 사용 가능
        }
    }

    if (E_CoolTime1 == 0)		// E_CoolTime1이 0일땐 사용 불가능
    {

        static float Timer = 0.0f;
        Timer += InDeltaTime;
        Current_E = Timer;
        if (Timer >= MaxE)
        {
            Timer = 0;
            Current_E = Timer;
            Player->E_CoolTime1 = 1;    // E_CoolTime1이 1일땐 사용 가능
        }
    }

    if (R_CoolTime1 == 0)		// E_CoolTime1이 0일땐 사용 불가능
    {

        static float Timer = 0.0f;
        Timer += InDeltaTime;
        Current_R = Timer;
        if (Timer >= MaxR)
        {
            Timer = 0;
            Current_R = Timer;
            Player->R_CoolTime1 = 1;    // E_CoolTime1이 1일땐 사용 가능
        }
    }

    if (HP_ProgressBar)
    {
        HP_ProgressBar->SetPercent(Current_HP / MaxHP);
    }
    if (Q_ProgressBar)
    {
        Q_ProgressBar->SetPercent(Current_Q / MaxQ);
    }
    if (W_ProgressBar)
    {
        W_ProgressBar->SetPercent(Current_W / MaxW);
    }
    if (E_ProgressBar)
    {
        E_ProgressBar->SetPercent(Current_E / MaxE);
    }
    if (R_ProgressBar)
    {
        R_ProgressBar->SetPercent(Current_R / MaxR);
    }
    if (I1_ProgressBar)
    {
        I1_ProgressBar->SetPercent(Current_I1 / Max1);
    }
    if (I2_ProgressBar)
    {
        I2_ProgressBar->SetPercent(Current_I2 / Max2);
    }
}

void USkillWidget::SetProgressBarValue_HP(float Value)
{
    Current_HP = Value;
    if (Q_ProgressBar)
    {
        Q_ProgressBar->SetPercent(Current_HP / MaxQ);
    }
}

void USkillWidget::SetProgressBarValue_Q(float Value)
{
    Current_Q = Value;
    if (Q_ProgressBar)
    {
        Q_ProgressBar->SetPercent(Current_Q / MaxQ);
    }
}

void USkillWidget::SetProgressBarValue_W(float Value)
{
    Current_W = Value;
    if (W_ProgressBar)
    {
        W_ProgressBar->SetPercent(Current_W / MaxW);
    }
}

void USkillWidget::SetProgressBarValue_E(float Value)
{
    Current_E = Value;
    if (E_ProgressBar)
    {
        E_ProgressBar->SetPercent(Current_E / MaxE);
    }
}

void USkillWidget::SetProgressBarValue_R(float Value)
{
    Current_R = Value;
    if (R_ProgressBar)
    {
        R_ProgressBar->SetPercent(Current_R / MaxR);
    }
}

void USkillWidget::SetProgressBarValue_I1(float Value)
{
    Current_I1 = Value;
    if (I1_ProgressBar)
    {
        I1_ProgressBar->SetPercent(Current_I1 / Max1);
    }
}

void USkillWidget::SetProgressBarValue_I2(float Value)
{
    Current_I2 = Value;
    if (I2_ProgressBar)
    {
        I2_ProgressBar->SetPercent(Current_I2 / Max2);
    }
}
