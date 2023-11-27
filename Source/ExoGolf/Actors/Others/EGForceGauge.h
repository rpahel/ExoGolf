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
	//==== Components ====

	UPROPERTY()
	USplineMeshComponent* SplineMesh;
	
public:
	AEGForceGauge();
	void SetColor(const FColor& Color);
	void SetLength(float Length);
	void AddRotation(float Delta);
};
