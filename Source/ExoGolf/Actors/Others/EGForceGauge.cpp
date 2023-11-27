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

void AEGForceGauge::SetColor(const FColor& Color)
{
	if(!SplineMesh)
	{
		GLog->Log(ELogVerbosity::Error, "AForceGauge : SetColor() -> SplineMesh is nullptr !");
		return;
	}
}

void AEGForceGauge::SetLength(float Length)
{
	if(!SplineMesh)
	{
		GLog->Log(ELogVerbosity::Error, "AForceGauge : SetLength() -> SplineMesh is nullptr !");
		return;
	}
}

void AEGForceGauge::AddRotation(float Delta)
{
	if(!SplineMesh)
	{
		GLog->Log(ELogVerbosity::Error, "AForceGauge : AddRotation() -> SplineMesh is nullptr !");
		return;
	}
}

