// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "EGPlayer.generated.h"

class UHeadsUpDisplay;
enum class EMouseButtonPressed;
struct FInputActionValue;

class UPlayerData;
class AForceGauge;
class UInputAction;
class UInputMappingContext;
class USphereComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

DECLARE_MULTICAST_DELEGATE(FPlayerDelegate);

UCLASS()
class EXOGOLF_API AEGPlayer : public APawn
{
	GENERATED_BODY()

public:
	FPlayerDelegate OnStrike;
	FPlayerDelegate OnPause;
	
private:
	//==== Exposed Fields ====

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bDebugMode = false;
#endif

	UPROPERTY(EditAnywhere, Category = "Player|Data")
	UPlayerData* PlayerData;

	//==== Components ====

	UPROPERTY(VisibleAnywhere, Category = "Player|Components")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Player|Components")
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Player|Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Player|Components")
	UCameraComponent* CameraComponent;
	
	//==== Inputs ====
	
	UPROPERTY(EditDefaultsOnly, Category = "Player|Inputs")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Inputs")
	UInputAction* IA_LMB;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Inputs")
	UInputAction* IA_RMB;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Inputs")
	UInputAction* IA_MousePos;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Inputs")
	UInputAction* IA_Scroll;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Inputs")
	UInputAction* IA_Pause;

	UPROPERTY()
	UEnhancedInputComponent* EIC;
	
	//==== Fields ====

	bool bAllowInputs = true;
	float CurrentStrikeForce = 0;
	FVector LastPosition = FVector::Zero();
	FIntPoint MouseLastPos = FIntPoint::ZeroValue;
	EMouseButtonPressed MouseButtonPressed;

	UPROPERTY()
	UWorld* World = nullptr;
	
	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	AForceGauge* CurrentForceGauge = nullptr;
	
public:
	AEGPlayer();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;
	void ReturnToLastPosition();
	void AllowInputs(){bAllowInputs = true;}
	void SetCursorVisibility(const bool IsVisible);
	
private:
	//==== Overrides ====
	
	virtual void BeginPlay() override;

	//==== Methods ====

	bool IsGrounded() const;
	void UpdateForceGauge();
	void RotateCamera(const FVector2D& MouseDelta) const;
	FVector GetProjectedMousePosition(const FVector& MousePosition, const FVector& MouseDirection) const;
	FRotator GetForceGaugeDesiredRotation(const FVector& ProjectedMousePosition) const;
	TTuple<FVector, FVector> GetWorldMousePositionAndDirection() const;
	AForceGauge* SpawnForceGauge();
	
	//==== Input Handlers ====
	
	UFUNCTION()
	void LeftClickStarted(const FInputActionValue& Value);

	UFUNCTION()
	void LeftClickStopped(const FInputActionValue& Value);

	UFUNCTION()
	void RightClickStarted(const FInputActionValue& Value);

	UFUNCTION()
	void RightClickStopped(const FInputActionValue& Value);

	UFUNCTION()
	void MousePositionChanged(const FInputActionValue& Value);

	UFUNCTION()
	void SetCameraDistance(const FInputActionValue& Value);

	UFUNCTION()
	void OpenPauseMenu(const FInputActionValue& Value);
};

UENUM()
enum class EMouseButtonPressed
{
	None,
	LMB,
	RMB,
	Both
};
