// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ForceGaugeData.generated.h"

/**
 * 
 */
UCLASS()
class EXOGOLF_API UForceGaugeData : public UDataAsset
{
	GENERATED_BODY()

public:
	
	// Spline mesh's end scale at minimum force.
	UPROPERTY(EditDefaultsOnly, Category = "Appearance|Strike")
	float MinimumForceSplineMeshScale = 0.1f;

	// Color representing minimum force.
	UPROPERTY(EditDefaultsOnly, Category = "Appearance|Strike")
	FColor MinimumForceColor = FColor::Green;

	// Spline mesh's end scale at maximum force.
	UPROPERTY(EditDefaultsOnly, Category = "Appearance|Strike")
	float MaximumForceSplineMeshScale = 1.f;
	
	// Color representing maximum force.
	UPROPERTY(EditDefaultsOnly, Category = "Appearance|Strike")
	FColor MaximumForceColor = FColor::Red;
};
