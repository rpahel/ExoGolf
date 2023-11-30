// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EGForceGauge.generated.h"

class USplineMeshComponent;

UCLASS()
class EXOGOLF_API AEGForceGauge : public AActor
{
	GENERATED_BODY()

private:
	//==== Exposed Fields ====

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
	
	//==== Components ====

	UPROPERTY(VisibleAnywhere)
	USplineMeshComponent* SplineMesh;

	//==== Hidden Fields ===

	float MinimumLength = -1.f;
	float MaximumLength = -1.f;

	UPROPERTY()
	UMaterialInstanceDynamic* DynMat;
	
public:
	AEGForceGauge();
	void SetForce(float NormalizedLength);
	void SetMinAndMaxLength(float Minimum, float Maximum);

private:
	//==== Overrides ====
	
	virtual void BeginPlay() override;
};
