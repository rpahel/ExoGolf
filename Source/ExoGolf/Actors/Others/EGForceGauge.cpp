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

void AEGForceGauge::SetForce(float NormalizedLength)
{
	if(!SplineMesh)
		return;

	if(MinimumLength == -1.f || MaximumLength == -1.f)
	{
		GLog->Log(ELogVerbosity::Error, "AEGForceGauge : SetForce() -> MinimumLenght and/or MaximumLength has not been set !");
		return;
	}

	// Set SplineMesh color
	const FLinearColor MeshColor = FLinearColor::LerpUsingHSV(MinimumForceColor, MaximumForceColor, NormalizedLength);
	DynMat->SetVectorParameterValue(FName(TEXT("Color")), MeshColor);

	// Set Spline length
	const float Length = FMath::Lerp(MinimumLength, MaximumLength, NormalizedLength);
	SplineMesh->SetEndPosition(FVector(Length, 0, 0));

	// Set Spline mesh end scale
	const FVector2D EndScale = FVector2D(
		FMath::Lerp(MinimumForceSplineMeshScale, MaximumForceSplineMeshScale, NormalizedLength),
		1);
	SplineMesh->SetEndScale(EndScale);
}

void AEGForceGauge::SetMinAndMaxLength(float Minimum, float Maximum)
{
	MinimumLength = Minimum;
	MaximumLength = Maximum;
}

//=======================================================================================|
//=================================== OVERRIDES =========================================|
//=======================================================================================|

void AEGForceGauge::BeginPlay()
{
	Super::BeginPlay();

	if(SplineMesh && SplineMesh->GetMaterial(0))
	{
		UMaterialInstance* Mat = Cast<UMaterialInstance>(SplineMesh->GetMaterial(0));
		DynMat = UMaterialInstanceDynamic::Create(Mat, SplineMesh);
		SplineMesh->SetMaterial(0, DynMat);
	}
}
