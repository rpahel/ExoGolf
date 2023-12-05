// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishLine.h"

#include "Components/BoxComponent.h"

AFinishLine::AFinishLine()
{
	PrimaryActorTick.bCanEverTick = false;

	PrimaryActorTick.bCanEverTick = false;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	
	if(BoxCollider)
	{
		BoxCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	}

	OnActorBeginOverlap.AddUniqueDynamic(this, &AFinishLine::ActorOverlap);
}

void AFinishLine::ActorOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
}
