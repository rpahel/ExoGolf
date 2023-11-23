// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EGOverlapExemple.generated.h"

class UBoxComponent;
class UPointLightComponent;

UCLASS()
class EXOGOLF_API AEGOverlapExemple : public AActor
{
	GENERATED_BODY()

private:
	//==== Exposed ====
	
	UPROPERTY(EditAnywhere, Category = "Light|Color")
	FColor DefaultColor;
	
	UPROPERTY(EditAnywhere, Category = "Light|Color")
	FColor DetectionColor;
	
	//==== Components ====
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPointLightComponent* PointLightComponent;
	
public:
	AEGOverlapExemple();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
