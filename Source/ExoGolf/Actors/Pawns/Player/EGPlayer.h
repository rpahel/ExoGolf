// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExoGolf/ExosDeJerome/Scalable.h"
#include "GameFramework/Pawn.h"
#include "EGPlayer.generated.h"

enum class EMouseButtonPressed;
struct FInputActionValue;

class UPlayerData;
class AEGForceGauge;
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

	UPROPERTY()
	UEnhancedInputComponent* EIC;
	
	//==== Fields ====

	float CurrentStrikeForce = 0;
	FVector LastPosition = FVector::Zero();
	FIntPoint MouseLastPos = FIntPoint::ZeroValue;
	EMouseButtonPressed MouseButtonPressed;

	UPROPERTY()
	UWorld* World = nullptr;
	
	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	AEGForceGauge* CurrentForceGauge = nullptr;
	
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

	void UpdateForceGauge();
	void RotateCamera(const FVector2D& MouseDelta) const;
	void SetCursorVisibility(const bool IsVisible);
	FVector GetProjectedMousePosition(const FVector& MousePosition, const FVector& MouseDirection) const;
	FRotator GetForceGaugeDesiredRotation(const FVector& ProjectedMousePosition) const;
	TTuple<FVector, FVector> GetWorldMousePositionAndDirection() const;
	AEGForceGauge* SpawnForceGauge();
	
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
};

UENUM()
enum class EMouseButtonPressed
{
	None,
	LMB,
	RMB
};
