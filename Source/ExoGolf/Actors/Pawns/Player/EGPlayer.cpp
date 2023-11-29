// Fill out your copyright notice in the Description page of Project Settings.


#include "EGPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "ExoGolf/Actors/Others/EGForceGauge.h"
#include "ExoGolf/Datas/Enums.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif

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

	PlayerController->SetInputMode(FInputModeGameAndUI());
	
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
		EIC->BindAction(IA_MousePos, ETriggerEvent::Triggered, this, &AEGPlayer::MousePositionChanged);
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

	World = GetWorld();

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
		PlayerController->SetInputMode(FInputModeGameAndUI());
	}
	else // Store mouse cursor position
	{
		Viewport->GetMousePos(MouseLastPos);
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
	
	PlayerController->SetShowMouseCursor(IsVisible);
}

TObjectPtr<AEGForceGauge> AEGPlayer::SpawnForceGauge()
{
	const FVector BallPosition = GetActorLocation();
	const TTuple<FVector, FVector> WorldMousePositionAndDirection = GetWorldMousePositionAndDirection();
	const FVector ProjectedMousePosition = GetProjectedMousePosition(WorldMousePositionAndDirection.Get<0>(), WorldMousePositionAndDirection.Get<1>());

#if WITH_EDITOR
	if(bDebugMode)
		DrawDebugSphere(World, ProjectedMousePosition, 50, 32, FColor::Red, false, 3);
#endif
	
	const FRotator ForceGaugeDesiredRotation = GetForceGaugeDesiredRotation(ProjectedMousePosition);

	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Set CurrentStrikeForce and ForceGaugeLength;
	const float MousePositionDistance = ProjectedMousePosition.Length();
	const float StrikeDistance = UKismetMathLibrary::NormalizeToRange(MousePositionDistance, MinimumStrikeDistance, MaximumStrikeDistance);
	CurrentStrikeForce = StrikeDistance * MaximumForce;
	CurrentStrikeForce = FMath::Clamp(CurrentStrikeForce, MinimumForce, MaximumForce);
	
#if WITH_EDITOR
	if(bDebugMode)
		DrawDebugSphere(World, ProjectedMousePosition, 50, 32, FColor::Red, false, 3);
#endif

	AEGForceGauge* ForceGaugePtr = World->SpawnActor<AEGForceGauge>(ForceGauge, BallPosition, ForceGaugeDesiredRotation, SpawnParams);
	ForceGaugePtr->SetMinAndMaxLength(MinimumStrikeDistance, MaximumStrikeDistance);

	const float NormalizedForce = UKismetMathLibrary::NormalizeToRange(CurrentStrikeForce, MinimumStrikeDistance, MaximumStrikeDistance);

	ForceGaugePtr->SetForce(NormalizedForce);
	
	return ForceGaugePtr;
}

FRotator AEGPlayer::GetForceGaugeDesiredRotation(const FVector& ProjectedMousePosition) const
{
	const FVector BallToMousePos = ProjectedMousePosition - GetActorLocation();
	FRotator ToMousePosRotation = BallToMousePos.Rotation();
	ToMousePosRotation.Pitch = 0;
	return ToMousePosRotation;
}

FVector AEGPlayer::GetProjectedMousePosition(const FVector& MousePosition, const FVector& MouseDirection) const
{
	FHitResult HitResult;
	const float BallZPos = GetActorLocation().Z;
	const FVector MissLocationRaw = MousePosition + 100000000 * MouseDirection;
	
	PlayerController->GetHitResultUnderCursor(ECC_WorldStatic, false, HitResult);

	// Bring cursor hit position to the same plane as the ball.
	const FVector HitLocation = FVector(
		HitResult.Location.X,
		HitResult.Location.Y,
		BallZPos);
	
	const FVector MissLocation = FVector(
		MissLocationRaw.X,
		MissLocationRaw.Y,
		BallZPos);
	
	return HitResult.bBlockingHit ? HitLocation : MissLocation;
}

TTuple<FVector, FVector> AEGPlayer::GetWorldMousePositionAndDirection() const
{
	FVector Location;
	FVector Direction;
	PlayerController->DeprojectMousePositionToWorld(Location, Direction);
	return TTuple<FVector, FVector>(Location, Direction);
}

//=======================================================================================|
//================================= INPUT HANDLERS ======================================|
//=======================================================================================|

void AEGPlayer::LeftClickStarted(const FInputActionValue& Value)
{
	MouseButtonPressed = LMB;
	SetCursorVisibility(false);

	if(World == nullptr || ForceGauge == nullptr)
		return;

	if(!ForceGauge)
		return;

	if(!CurrentForceGauge)
		CurrentForceGauge = SpawnForceGauge();
}

void AEGPlayer::LeftClickStopped(const FInputActionValue& Value)
{
	if(MouseButtonPressed == LMB)
	{
		MouseButtonPressed = None;
		SetCursorVisibility(true);
	}

	if(CurrentForceGauge)
	{
		CurrentForceGauge->Destroy();
		CurrentForceGauge = nullptr;
	}

	CurrentStrikeForce = 0;
}

void AEGPlayer::RightClickStarted(const FInputActionValue& Value)
{
	MouseButtonPressed = RMB;
	SetCursorVisibility(false);

	if(CurrentForceGauge)
	{
		CurrentForceGauge->Destroy();
		CurrentForceGauge = nullptr;
	}

	CurrentStrikeForce = 0;
}

void AEGPlayer::RightClickStopped(const FInputActionValue& Value)
{
	if(MouseButtonPressed == RMB)
	{
		MouseButtonPressed = None;
		SetCursorVisibility(true);
	}
}

void AEGPlayer::MousePositionChanged(const FInputActionValue& Value)
{
	if(MouseButtonPressed == None)
		return;

	const FVector2D MouseDeltaPosition = Value.Get<FVector2D>();

	if(MouseButtonPressed == RMB)
	{
		RotateCamera(MouseDeltaPosition);
	}
	else if(MouseButtonPressed == LMB && CurrentForceGauge)
	{
		const FRotator DeltaRot = FRotator(0, -MouseDeltaPosition.X, 0);
		const float DeltaForce = -MouseDeltaPosition.Y;
		CurrentForceGauge->AddActorWorldRotation(DeltaRot);
		CurrentStrikeForce = FMath::Clamp(CurrentStrikeForce + DeltaForce, MinimumForce, MaximumForce);
		CurrentForceGauge->SetForce(UKismetMathLibrary::NormalizeToRange(CurrentStrikeForce, MinimumForce, MaximumForce));
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
