// Fill out your copyright notice in the Description page of Project Settings.


#include "Warning.h"

// Sets default values
AWarning::AWarning()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWarning::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWarning::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

