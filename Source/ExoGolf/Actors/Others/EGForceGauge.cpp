// Fill out your copyright notice in the Description page of Project Settings.


#include "EGForceGauge.h"

#include "Components/SplineMeshComponent.h"

//=======================================================================================|
//===================================== PUBLIC ==========================================|
//=======================================================================================|

AEGForceGauge::AEGForceGauge()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineMesh = CreateDefaultSubobject<USplineMeshComponent>(TEXT("Spline Mesh"));
	if(SplineMesh)
		SetRootComponent(SplineMesh);
}

void AEGForceGauge::SetForce(float Length)
{
}

void AEGForceGauge::SetRotation(const FRotator& Rotation)
{
}

