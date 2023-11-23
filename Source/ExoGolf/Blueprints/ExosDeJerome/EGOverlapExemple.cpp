// Fill out your copyright notice in the Description page of Project Settings.


#include "EGOverlapExemple.h"

#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"

AEGOverlapExemple::AEGOverlapExemple()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	if(BoxComponent)
		SetRootComponent(BoxComponent);

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
	if(BoxComponent && PointLightComponent)
	{
		PointLightComponent->SetupAttachment(BoxComponent);
		PointLightComponent->SetLightFColor(DefaultColor);
	}

	BoxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AEGOverlapExemple::OnComponentBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &AEGOverlapExemple::OnComponentEndOverlap);
}

void AEGOverlapExemple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEGOverlapExemple::BeginPlay()
{
	Super::BeginPlay();

	if(PointLightComponent)
		PointLightComponent->SetLightFColor(DefaultColor);
}

void AEGOverlapExemple::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GLog->Log("Overlap !");

	if(!PointLightComponent)
		return;

	PointLightComponent->SetLightFColor(DetectionColor);
}

void AEGOverlapExemple::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GLog->Log("End overlap !");

	if(!PointLightComponent)
		return;

	PointLightComponent->SetLightFColor(DefaultColor);
}


