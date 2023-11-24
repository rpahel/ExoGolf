// Fill out your copyright notice in the Description page of Project Settings.


#include "EGOverlapExemple.h"

#include "Scalable.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"

//=======================================================================================|
//===================================== PUBLIC ==========================================|
//=======================================================================================|

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

//=======================================================================================|
//=================================== OVERRIDES =========================================|
//=======================================================================================|

void AEGOverlapExemple::BeginPlay()
{
	Super::BeginPlay();

	if(PointLightComponent)
		PointLightComponent->SetLightFColor(DefaultColor);
}

//=======================================================================================|
//================================ EVENT HANDLERS =======================================|
//=======================================================================================|

void AEGOverlapExemple::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GLog->Log("Overlap !");

	if(!PointLightComponent)
		return;

	PointLightComponent->SetLightFColor(DetectionColor);

	if(!OtherActor->Implements<UScalable>())
		return;
	
	OverlappedActor = OtherActor;

	GetWorldTimerManager().SetTimer(OverlapTimer, this, &AEGOverlapExemple::OverlapTimerHandler, OverlapTimerRate, true);
}

void AEGOverlapExemple::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor != OverlappedActor)
		return;
	
	GLog->Log("End overlap !");

	if(!PointLightComponent)
		return;

	OverlappedActor = nullptr;

	GetWorldTimerManager().ClearTimer(OverlapTimer);
	
	PointLightComponent->SetLightFColor(DefaultColor);
}

void AEGOverlapExemple::OverlapTimerHandler()
{
	if(!OverlappedActor)
		return;

	IScalable* Scalable = Cast<IScalable>(OverlappedActor);
	
	if(!Scalable)
	{
		GLog->Log(ELogVerbosity::Error, "EGOverlapExemple : OverlapTimerHandler() -> Scalable is nullptr !");
		return;
	}

	Scalable->UniformAddScale(0.1f);
}

#if WITH_EDITOR
//=======================================================================================|
//================================== EDITOR ONLY ========================================|
//=======================================================================================|

void AEGOverlapExemple::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.GetMemberPropertyName();

	if(PropertyName == GET_MEMBER_NAME_CHECKED(AEGOverlapExemple, DefaultColor))
		PointLightComponent->LightColor = DefaultColor;
	else if(PropertyName == GET_MEMBER_NAME_CHECKED(AEGOverlapExemple, DetectionColor))
		PointLightComponent->LightColor = DetectionColor;
}
#endif