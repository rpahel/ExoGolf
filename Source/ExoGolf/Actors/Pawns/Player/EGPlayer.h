// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExoGolf/ExosDeJerome/Scalable.h"
#include "GameFramework/Pawn.h"
#include "EGPlayer.generated.h"

enum EMouseButtonPressed : int;
struct FInputActionValue;

class UInputAction;
class UInputMappingContext;
class USphereComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class EXOGOLF_API AEGPlayer : public APawn, public IScalable
{
	GENERATED_BODY()

private:
	//==== Exposed Fields ====

	// The minimum distance the player has to pull back its cursor to be considered a strike.
	UPROPERTY(EditDefaultsOnly, Category = "Player|Gameplay|Strike")
	float MinimumStrikeDistance = 0.f;

	// The impulse force applied to the ball at minimum strike distance.
	UPROPERTY(EditDefaultsOnly, Category = "Player|Gameplay|Strike")
	float MinimumForce = 1.f;

	// Color representing minimum force.
	UPROPERTY(EditDefaultsOnly, Category = "Player|Gameplay|Strike")
	FColor MinimumForceColor = FColor::Green;

	// The distance the player needs to pull back its cursor to strike the ball with maximum force.
	UPROPERTY(EditDefaultsOnly, Category = "Player|Gameplay|Strike")
	float MaximumStrikeDistance = 100.f;

	// The impulse force applied to the ball at maximum strike distance.
	UPROPERTY(EditDefaultsOnly, Category = "Player|Gameplay|Strike")
	float MaximumForce = 100.f;

	// Color representing maximum force.
	UPROPERTY(EditDefaultsOnly, Category = "Player|Gameplay|Strike")
	FColor MaximumForceColor = FColor::Red;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Camera", meta=(UIMin = 0.f, UIMax = 1.f))
	float CameraSensitivity = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Camera", meta=(UIMin = 0.f, UIMax = 100.f))
	float ScrollSensitivity = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Camera")
	float CameraMinimumDistance = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Camera")
	float CameraMaximumDistance = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Camera")
	float CameraMinimumPitch = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Player|Camera")
	float CameraMaximumPitch = 90.f;

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

	UPROPERTY()
	UEnhancedInputComponent* EIC;
	
	//==== Fields ====

	FIntPoint MouseLastPos = FIntPoint::ZeroValue;
	TEnumAsByte<EMouseButtonPressed> MouseButtonPressed;

	UPROPERTY()
	APlayerController* PlayerController;
	
public:
	AEGPlayer();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	//==== Interface Implementations ====
	
	virtual void UniformAddScale(float Amount) override;
	virtual void NonUniformAddScale(float X, float Y, float Z) override;
	virtual void UniformMultiplyScale(float Amount) override;
	virtual void NonUniformMultiplyScale(float X, float Y, float Z) override;
	
private:
	//==== Overrides ====
	
	virtual void BeginPlay() override;

	//==== Methods ====

	void RotateCamera(const FVector2D& MouseDelta) const;
	void SetCursorVisibility(const bool IsVisible);
	
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
	void SetCameraRotation(const FInputActionValue& Value);

	UFUNCTION()
	void SetCameraDistance(const FInputActionValue& Value);
};
