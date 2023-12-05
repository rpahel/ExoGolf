// Fill out your copyright notice in the Description page of Project Settings.


#include "EGPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "ExoGolf/Actors/Others/ForceGauge.h"
#include "ExoGolf/Datas/Data_Assets/PlayerData.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#if WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif

//=======================================================================================|
//===================================== PUBLIC ==========================================|
//=======================================================================================|

AEGPlayer::AEGPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

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

	if(IA_Pause)
	{
		EIC->BindAction(IA_Pause, ETriggerEvent::Started, this, &AEGPlayer::OpenPauseMenu);
	}
}

void AEGPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(CurrentForceGauge)
		CurrentForceGauge->SetActorLocation(GetActorLocation());
}

void AEGPlayer::ReturnToLastPosition()
{
	if(!SphereComponent)
		return;
	
	SphereComponent->SetPhysicsLinearVelocity(FVector::Zero());
	SphereComponent->SetPhysicsAngularVelocityInRadians(FVector::Zero());

	SetActorLocation(LastPosition);
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

	LastPosition = GetActorLocation();
}

//=======================================================================================|
//==================================== METHODS ==========================================|
//=======================================================================================|

bool AEGPlayer::IsGrounded() const
{
	if(!SphereComponent)
		return false;

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	const FVector Start = GetActorLocation();
	const FVector End = GetActorLocation() + (SphereComponent->GetUnscaledSphereRadius() + 1) * FVector::DownVector;
	return GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldStatic);
}

void AEGPlayer::UpdateForceGauge()
{
	if(!CurrentForceGauge)
		return;
	
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

AForceGauge* AEGPlayer::SpawnForceGauge()
{
	const FVector BallPosition = GetActorLocation();
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AForceGauge* ForceGaugePtr = World->SpawnActor<AForceGauge>(PlayerData->ForceGauge, BallPosition, FRotator::ZeroRotator, SpawnParams);

	if(ForceGaugePtr)
	{
		ForceGaugePtr->SetMinAndMaxLength(PlayerData->MinimumStrikeDistance, PlayerData->MaximumStrikeDistance);
		CurrentForceGauge = ForceGaugePtr;
		UpdateForceGauge();
	}
	
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
	const FVector BallLocation = GetActorLocation();
	const FPlane XYPlane = FPlane(BallLocation, FVector::UpVector);
	FVector ProjectedMousePosition;
	
	if(MouseDirection.Z >= 0) // Click direction is parallel or going away from the plane.
	{
		ProjectedMousePosition = MousePosition + 100000000 * MouseDirection;
		ProjectedMousePosition.Z = BallLocation.Z;
	}
	else
	{
		ProjectedMousePosition = FMath::RayPlaneIntersection(MousePosition, MouseDirection, XYPlane);
	}

#if WITH_EDITOR
	if(bDebugMode)
	{
		DrawDebugSphere(World, MousePosition, 2, 16, FColor::Green, false, 2);
		DrawDebugLine(World, MousePosition, MousePosition + MouseDirection * 10000, FColor::Blue, false, 2);
		DrawDebugSphere(World, ProjectedMousePosition, 2, 16, FColor::Red, false, 2);
	}
#endif

	return ProjectedMousePosition;
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
	if(!bAllowInputs)
		return;

	if(MouseButtonPressed == EMouseButtonPressed::RMB)
		MouseButtonPressed = EMouseButtonPressed::Both;
	else
		MouseButtonPressed = EMouseButtonPressed::LMB;
		
	SetCursorVisibility(false);

	if(!World || !PlayerData->ForceGauge)
		return;

	if(!CurrentForceGauge)
		CurrentForceGauge = SpawnForceGauge();

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), PlayerData->TimeDilationFactor);
}

void AEGPlayer::LeftClickStopped(const FInputActionValue& Value)
{
	if(MouseButtonPressed == EMouseButtonPressed::LMB)
	{
		MouseButtonPressed = EMouseButtonPressed::None;
		if(bAllowInputs)
			SetCursorVisibility(true);
	}
	else if(MouseButtonPressed == EMouseButtonPressed::Both)
	{
		MouseButtonPressed = EMouseButtonPressed::RMB;
		if(bAllowInputs)
			SetCursorVisibility(false);
	}

	if(!bAllowInputs)
		return;
	
	if(!CurrentForceGauge)
		return;
	
	if(IsGrounded())
	{
		LastPosition = GetActorLocation();
		GLog->Log("Last Position = " + LastPosition.ToString());
	}

	OnStrike.Broadcast();
	const FVector Direction = -CurrentStrikeForce * CurrentForceGauge->GetActorRotation().Vector();
	SphereComponent->AddImpulse(Direction, NAME_None, true);
	CurrentForceGauge->Destroy();
	CurrentForceGauge = nullptr;
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);	

	CurrentStrikeForce = 0;
}

void AEGPlayer::RightClickStarted(const FInputActionValue& Value)
{
	if(MouseButtonPressed == EMouseButtonPressed::LMB)
		MouseButtonPressed = EMouseButtonPressed::Both;
	else
		MouseButtonPressed = EMouseButtonPressed::RMB;
	
	if(!bAllowInputs)
		return;
	
	SetCursorVisibility(false);
	
	if(CurrentForceGauge)
	{
		CurrentForceGauge->Destroy();
		CurrentForceGauge = nullptr;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	}

	CurrentStrikeForce = 0;
}

void AEGPlayer::RightClickStopped(const FInputActionValue& Value)
{
	if(MouseButtonPressed == EMouseButtonPressed::RMB)
	{
		MouseButtonPressed = EMouseButtonPressed::None;
		if(bAllowInputs)
			SetCursorVisibility(true);
	}
	else if(MouseButtonPressed == EMouseButtonPressed::Both)
	{
		MouseButtonPressed = EMouseButtonPressed::LMB;
		if(bAllowInputs)
			SetCursorVisibility(false);
	}
}

void AEGPlayer::MousePositionChanged(const FInputActionValue& Value)
{
	if(!bAllowInputs)
		return;
	
	if(MouseButtonPressed == EMouseButtonPressed::None)
		return;

	const FVector2D MouseDeltaPosition = Value.Get<FVector2D>();

	if(MouseButtonPressed == EMouseButtonPressed::RMB)
	{
		RotateCamera(MouseDeltaPosition);
	}
	else if(MouseButtonPressed == EMouseButtonPressed::LMB)
	{
		UpdateForceGauge();
	}
	else
	{
		RotateCamera(MouseDeltaPosition);
		UpdateForceGauge();
	}
}

void AEGPlayer::SetCameraDistance(const FInputActionValue& Value)
{
	if(!bAllowInputs)
		return;
	
	if(!SpringArmComponent)
		return;

	const float Delta = Value.Get<float>() * PlayerData->ScrollSensitivity;
	SpringArmComponent->TargetArmLength = FMath::Clamp(
		SpringArmComponent->TargetArmLength + Delta,
		PlayerData->CameraMinimumDistance,
		PlayerData->CameraMaximumDistance);
}

void AEGPlayer::OpenPauseMenu(const FInputActionValue& Value)
{
	if(CurrentForceGauge)
	{
		CurrentForceGauge->Destroy();
		CurrentForceGauge = nullptr;
	}

	OnPause.Broadcast();

	bAllowInputs = UGameplayStatics::IsGamePaused(GetWorld()) ? false : true;
}
