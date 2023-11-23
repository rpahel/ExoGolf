// Fill out your copyright notice in the Description page of Project Settings.


#include "EGPlayer.h"

// Sets default values
AEGPlayer::AEGPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEGPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEGPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

