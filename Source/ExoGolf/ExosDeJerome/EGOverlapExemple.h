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
	//==== Exposed Fields ====
	
	UPROPERTY(EditAnywhere, Category = "OverlapExemple|Light Color")
	FColor DefaultColor;
	
	UPROPERTY(EditAnywhere, Category = "OverlapExemple|Light Color")
	FColor DetectionColor;

	UPROPERTY(EditAnywhere, Category = "OverlapExemple")
	float OverlapTimerRate = 0.5f;
	
	//==== Components ====
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPointLightComponent* PointLightComponent;
	
	//==== Fields ====

	UPROPERTY()
	AActor* OverlappedActor;
	
	FTimerHandle OverlapTimer;
	
public:
	AEGOverlapExemple();
	virtual void Tick(float DeltaTime) override;

protected:
	//==== Overrides ====
	
	virtual void BeginPlay() override;

	//==== Event Handlers ====
	
	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OverlapTimerHandler();

#if WITH_EDITOR
	//==== Editor Only ====

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
