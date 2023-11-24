// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ExoGolf/ExosDeJerome/Scalable.h"
#include "EGPlayer.generated.h"

struct FEnhancedInputActionEventBinding;
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

	UPROPERTY()
	UEnhancedInputComponent* EIC;
	
	//==== Fields ====
	
	FVector2D MousePos = FVector2d::Zero();
	uint32 LMBMousePosEventHandle = 0;
	uint32 RMBMousePosEventHandle = 0;
	
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
	void SetMousePos(const FInputActionValue& Value);
};
