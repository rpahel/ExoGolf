// Fill out your copyright notice in the Description page of Project Settings.


#include "EGPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"

AEGPlayer::AEGPlayer()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	if(SphereComponent)
		SetRootComponent(SphereComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	if(SphereComponent && StaticMeshComponent)
		StaticMeshComponent->SetupAttachment(SphereComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	if(SphereComponent && SpringArmComponent)
		SpringArmComponent->SetupAttachment(SphereComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if(SpringArmComponent && CameraComponent)
		CameraComponent->SetupAttachment(SpringArmComponent);
}

void AEGPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if(!EIC)
	{
		GLog->Log(ELogVerbosity::Error, "AEGPlayer : SetupPlayerInputComponent() -> EIC is nullptr !");
		return;
	}

	if(IA_LMB)
	{
		EIC->BindAction(IA_LMB, ETriggerEvent::Started, this, &AEGPlayer::LeftClickStarted);
		EIC->BindAction(IA_LMB, ETriggerEvent::Completed, this, &AEGPlayer::LeftClickStopped);
		EIC->BindAction(IA_LMB, ETriggerEvent::Canceled, this, &AEGPlayer::LeftClickStopped);
	}

	if(IA_RMB)
	{
		EIC->BindAction(IA_RMB, ETriggerEvent::Started, this, &AEGPlayer::RightClickStarted);
		EIC->BindAction(IA_RMB, ETriggerEvent::Completed, this, &AEGPlayer::RightClickStopped);
		EIC->BindAction(IA_RMB, ETriggerEvent::Canceled, this, &AEGPlayer::RightClickStopped);
	}
}

void AEGPlayer::UniformAddScale(float Amount)
{
	const FVector NewScale = SphereComponent->GetComponentScale() + Amount;
	SphereComponent->SetWorldScale3D(NewScale);
}

void AEGPlayer::NonUniformAddScale(float X, float Y, float Z)
{
	const FVector OldScale = SphereComponent->GetComponentScale();
	const FVector NewScale = FVector(OldScale.X + X, OldScale.Y + Y, OldScale.Z + Z);
	SphereComponent->SetWorldScale3D(NewScale);
}

void AEGPlayer::UniformMultiplyScale(float Amount)
{
	const FVector NewScale = SphereComponent->GetComponentScale() * Amount;
	SphereComponent->SetWorldScale3D(NewScale);
}

void AEGPlayer::NonUniformMultiplyScale(float X, float Y, float Z)
{
	const FVector OldScale = SphereComponent->GetComponentScale();
	const FVector NewScale = FVector(OldScale.X * X, OldScale.Y * Y, OldScale.Z * Z);
	SphereComponent->SetWorldScale3D(NewScale);
}

void AEGPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AEGPlayer::LeftClickStarted(const FInputActionValue& Value)
{
	GLog->Log("AEGPlayer : LeftClickStarted().");
	
	if(!EIC)
	{
		GLog->Log(ELogVerbosity::Error, "AEGPlayer : LeftClickStarted() -> EIC is nullptr !");
		return;
	}
	
	LMBMousePosEventHandle = EIC->BindAction(IA_MousePos, ETriggerEvent::Triggered, this, &AEGPlayer::SetMousePos).GetHandle();
}

void AEGPlayer::LeftClickStopped(const FInputActionValue& Value)
{
	GLog->Log("AEGPlayer : LeftClickStopped().");
	
	if(!EIC)
	{
		GLog->Log(ELogVerbosity::Error, "AEGPlayer : LeftClickStarted() -> EIC is nullptr !");
		return;
	}

	EIC->RemoveBindingByHandle(LMBMousePosEventHandle);
	LMBMousePosEventHandle = 0;
}

void AEGPlayer::RightClickStarted(const FInputActionValue& Value)
{
	GLog->Log("AEGPlayer : RightClickStarted().");

	if(!EIC)
	{
		GLog->Log(ELogVerbosity::Error, "AEGPlayer : LeftClickStarted() -> EIC is nullptr !");
		return;
	}
	
	RMBMousePosEventHandle = EIC->BindAction(IA_MousePos, ETriggerEvent::Triggered, this, &AEGPlayer::SetMousePos).GetHandle();
}

void AEGPlayer::RightClickStopped(const FInputActionValue& Value)
{
	GLog->Log("AEGPlayer : RightClickStopped().");
	
	if(!EIC)
	{
		GLog->Log(ELogVerbosity::Error, "AEGPlayer : LeftClickStarted() -> EIC is nullptr !");
		return;
	}
	
	EIC->RemoveBindingByHandle(RMBMousePosEventHandle);
	RMBMousePosEventHandle = 0;
}

void AEGPlayer::SetMousePos(const FInputActionValue& Value)
{
	MousePos = Value.Get<FVector2d>();
	GLog->Log("AEGPlayer : SetMousePos() -> " + MousePos.ToString());
}
