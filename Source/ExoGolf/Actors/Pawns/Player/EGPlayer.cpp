// Fill out your copyright notice in the Description page of Project Settings.


#include "EGPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "ExoGolf/Datas/Enums/MouseButtonPressed.h"
#include "GameFramework/SpringArmComponent.h"

//=======================================================================================|
//===================================== PUBLIC ==========================================|
//=======================================================================================|

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

void AEGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerController = Cast<APlayerController>(GetController());
	if(!PlayerController)
	{
		GLog->Log(ELogVerbosity::Error, "AEGPlayer : SetupPlayerInputComponent() -> PlayerController is nullptr !");
		return;
	}
	
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

	if(IA_MousePos)
	{
		EIC->BindAction(IA_MousePos, ETriggerEvent::Triggered, this, &AEGPlayer::SetCameraRotation);
	}

	if(IA_Scroll)
	{
		EIC->BindAction(IA_Scroll, ETriggerEvent::Triggered, this, &AEGPlayer::SetCameraDistance);
	}
}

//=======================================================================================|
//=========================== INTERFACE IMPLEMENTATIONS =================================|
//=======================================================================================|

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

//=======================================================================================|
//=================================== OVERRIDES =========================================|
//=======================================================================================|

void AEGPlayer::BeginPlay()
{
	Super::BeginPlay();

	if(SpringArmComponent)
		SpringArmComponent->TargetArmLength = CameraMaximumDistance;
}

//=======================================================================================|
//==================================== METHODS ==========================================|
//=======================================================================================|

void AEGPlayer::RotateCamera(const FVector2D& MouseDelta) const
{
	const FRotator DeltaRotation = FRotator(MouseDelta.Y, MouseDelta.X, 0) * CameraSensitivity;
	FRotator NewRotation = SpringArmComponent->GetRelativeRotation();
	NewRotation.Add(DeltaRotation.Pitch, DeltaRotation.Yaw, DeltaRotation.Roll);
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, CameraMinimumPitch, CameraMaximumPitch);
	SpringArmComponent->SetRelativeRotation(NewRotation);
}

void AEGPlayer::SetCursorVisibility(const bool IsVisible)
{
	if(!PlayerController)
		return;
	
	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if(!LocalPlayer || !LocalPlayer->ViewportClient)
		return;
		
	FViewport* Viewport = LocalPlayer->ViewportClient->Viewport;
	if(!Viewport)
		return;

	// If visible, set mouse cursor position to previous position
	if(IsVisible)
	{
		Viewport->SetMouse(MouseLastPos.X, MouseLastPos.Y);
	}
	else // Store mouse cursor position
	{
		Viewport->GetMousePos(MouseLastPos);
	}
	
	PlayerController->SetShowMouseCursor(IsVisible);
}

//=======================================================================================|
//================================= INPUT HANDLERS ======================================|
//=======================================================================================|

void AEGPlayer::LeftClickStarted(const FInputActionValue& Value)
{
	GLog->Log("AEGPlayer : LeftClickStarted().");
	MouseButtonPressed = LMB;
	SetCursorVisibility(false);
}

void AEGPlayer::LeftClickStopped(const FInputActionValue& Value)
{
	GLog->Log("AEGPlayer : LeftClickStopped().");
	if(MouseButtonPressed == LMB)
	{
		MouseButtonPressed = None;
		SetCursorVisibility(true);
	}
}

void AEGPlayer::RightClickStarted(const FInputActionValue& Value)
{
	GLog->Log("AEGPlayer : RightClickStarted().");
	MouseButtonPressed = RMB;
	SetCursorVisibility(false);
}

void AEGPlayer::RightClickStopped(const FInputActionValue& Value)
{
	GLog->Log("AEGPlayer : RightClickStopped().");
	if(MouseButtonPressed == RMB)
	{
		MouseButtonPressed = None;
		SetCursorVisibility(true);
	}
}

void AEGPlayer::SetCameraRotation(const FInputActionValue& Value)
{
	if(MouseButtonPressed == None)
		return;

	const FVector2D MouseDeltaPosition = Value.Get<FVector2D>();

	if(MouseButtonPressed == RMB)
	{
		RotateCamera(MouseDeltaPosition);
	}
	else if(MouseButtonPressed == LMB)
	{
		GLog->Log("AEGPlayer : SetCameraRotation() -> MouseDeltaPosition = " + MouseDeltaPosition.ToString());
	}
}

void AEGPlayer::SetCameraDistance(const FInputActionValue& Value)
{
	if(!SpringArmComponent)
		return;

	const float Delta = Value.Get<float>() * ScrollSensitivity;
	SpringArmComponent->TargetArmLength = FMath::Clamp(
		SpringArmComponent->TargetArmLength + Delta,
		CameraMinimumDistance,
		CameraMaximumDistance);
}
