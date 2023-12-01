// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EGForceGauge.generated.h"

class UForceGaugeData;
class USplineMeshComponent;

UCLASS()
class EXOGOLF_API AEGForceGauge : public AActor
{
	GENERATED_BODY()

private:
	//==== Exposed Fields ====

	UPROPERTY(EditAnywhere, Category = "ForceGauge|Data")
	UForceGaugeData* ForceGaugeData;
	
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
