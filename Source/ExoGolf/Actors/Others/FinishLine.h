// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FinishLine.generated.h"

class UBoxComponent;

UCLASS()
class EXOGOLF_API AFinishLine : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollider;
	
public:
	AFinishLine();

private:
	UFUNCTION()
	void ActorOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
