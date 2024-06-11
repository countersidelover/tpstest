// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Boss2FSM.h"
#include "Boss2Anim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UBoss2Anim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	EBoss2State animState;
};

