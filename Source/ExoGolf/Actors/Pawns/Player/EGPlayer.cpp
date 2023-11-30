// Fill out your copyright notice in the Description page of Project Settings.


#include "EGPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "ExoGolf/Actors/Others/EGForceGauge.h"
#include "ExoGolf/Datas/Enums.h"
#include "ExoGolf/Datas/Data_Assets/PlayerData.h"
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
		SpringArmComponent->TargetArmLength = PlayerData->CameraMaximumDistance;
}

//=======================================================================================|
//==================================== METHODS ==========================================|
//=======================================================================================|

void AEGPlayer::UpdateForceGauge()
{
	const TTuple<FVector, FVector> MousePosAndDir = GetWorldMousePositionAndDirection();
	const FVector ProjectedMousePosition = GetProjectedMousePosition(MousePosAndDir.Get<0>(), MousePosAndDir.Get<1>());
	const FRotator ForceGaugeRotation = GetForceGaugeDesiredRotation(ProjectedMousePosition);
	CurrentForceGauge->SetActorRotation(ForceGaugeRotation);
	float BallToMouseDistance = (ProjectedMousePosition - GetActorLocation()).Length();
	BallToMouseDistance = UKismetMathLibrary::NormalizeToRange(BallToMouseDistance, PlayerData->MinimumStrikeDistance, PlayerData->MaximumStrikeDistance);
	CurrentStrikeForce = FMath::Clamp(BallToMouseDistance * PlayerData->MaximumForce, PlayerData->MinimumForce, PlayerData->MaximumForce);
	CurrentForceGauge->SetForce(UKismetMathLibrary::NormalizeToRange(CurrentStrikeForce, PlayerData->MinimumForce, PlayerData->MaximumForce));
}

void AEGPlayer::RotateCamera(const FVector2D& MouseDelta) const
{
	const FRotator DeltaRotation = FRotator(MouseDelta.Y, MouseDelta.X, 0) * PlayerData->CameraSensitivity;
	FRotator NewRotation = SpringArmComponent->GetRelativeRotation();
	NewRotation.Add(DeltaRotation.Pitch, DeltaRotation.Yaw, DeltaRotation.Roll);
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, PlayerData->CameraMinimumPitch, PlayerData->CameraMaximumPitch);
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

AEGForceGauge* AEGPlayer::SpawnForceGauge()
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
	const float StrikeDistance = UKismetMathLibrary::NormalizeToRange(MousePositionDistance, PlayerData->MinimumStrikeDistance, PlayerData->MaximumStrikeDistance);
	CurrentStrikeForce = StrikeDistance * PlayerData->MaximumForce;
	CurrentStrikeForce = FMath::Clamp(CurrentStrikeForce, PlayerData->MinimumForce, PlayerData->MaximumForce);
	
#if WITH_EDITOR
	if(bDebugMode)
		DrawDebugSphere(World, ProjectedMousePosition, 50, 32, FColor::Red, false, 3);
#endif

	AEGForceGauge* ForceGaugePtr = World->SpawnActor<AEGForceGauge>(PlayerData->ForceGauge, BallPosition, ForceGaugeDesiredRotation, SpawnParams);
	ForceGaugePtr->SetMinAndMaxLength(PlayerData->MinimumStrikeDistance, PlayerData->MaximumStrikeDistance);

	const float NormalizedForce = UKismetMathLibrary::NormalizeToRange(CurrentStrikeForce, PlayerData->MinimumStrikeDistance, PlayerData->MaximumStrikeDistance);

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
	MouseButtonPressed = EMouseButtonPressed::LMB;
	SetCursorVisibility(false);

	if(World == nullptr || PlayerData->ForceGauge == nullptr)
		return;

	if(!PlayerData->ForceGauge)
		return;

	if(!CurrentForceGauge)
		CurrentForceGauge = SpawnForceGauge();
}

void AEGPlayer::LeftClickStopped(const FInputActionValue& Value)
{
	if(MouseButtonPressed == EMouseButtonPressed::LMB)
	{
		MouseButtonPressed = EMouseButtonPressed::None;
		SetCursorVisibility(true);
	}

	if(CurrentForceGauge)
	{
		const FVector Direction = -CurrentStrikeForce * CurrentForceGauge->GetActorRotation().Vector();
		SphereComponent->AddImpulse(Direction, NAME_None, true);
		CurrentForceGauge->Destroy();
		CurrentForceGauge = nullptr;
	}

	CurrentStrikeForce = 0;
}

void AEGPlayer::RightClickStarted(const FInputActionValue& Value)
{
	MouseButtonPressed = EMouseButtonPressed::RMB;
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
	if(MouseButtonPressed == EMouseButtonPressed::RMB)
	{
		MouseButtonPressed = EMouseButtonPressed::None;
		SetCursorVisibility(true);
	}
}

void AEGPlayer::MousePositionChanged(const FInputActionValue& Value)
{
	if(MouseButtonPressed == EMouseButtonPressed::None)
		return;

	const FVector2D MouseDeltaPosition = Value.Get<FVector2D>();

	if(MouseButtonPressed == EMouseButtonPressed::RMB)
	{
		RotateCamera(MouseDeltaPosition);
	}
	else if(MouseButtonPressed == EMouseButtonPressed::LMB && CurrentForceGauge)
	{
		UpdateForceGauge();
	}
}

void AEGPlayer::SetCameraDistance(const FInputActionValue& Value)
{
	if(!SpringArmComponent)
		return;

	const float Delta = Value.Get<float>() * PlayerData->ScrollSensitivity;
	SpringArmComponent->TargetArmLength = FMath::Clamp(
		SpringArmComponent->TargetArmLength + Delta,
		PlayerData->CameraMinimumDistance,
		PlayerData->CameraMaximumDistance);
}