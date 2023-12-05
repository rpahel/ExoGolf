// Fill out your copyright notice in the Description page of Project Settings.


#include "NoGoZone.h"

#include "Components/BoxComponent.h"
#include "ExoGolf/Actors/Pawns/Player/EGPlayer.h"
#include "Kismet/GameplayStatics.h"

ANoGoZone::ANoGoZone()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	
	if(BoxCollider)
	{
		BoxCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	}

	OnActorBeginOverlap.AddUniqueDynamic(this, &ANoGoZone::ActorOverlap);
}

void ANoGoZone::ActorOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if(AEGPlayer* Player = Cast<AEGPlayer>(OtherActor))
	{
		Player->ReturnToLastPosition();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
	}
	else
	{
		OtherActor->Destroy();
	}
}
