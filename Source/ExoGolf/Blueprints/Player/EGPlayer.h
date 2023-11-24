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

protected:
	//==== Components ====

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComponent;
	
	//==== Inputs ====
	
	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* IA_LMB;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* IA_RMB;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* IA_MousePos;

	UPROPERTY()
	UEnhancedInputComponent* EIC;

private:
	FVector2D MousePos = FVector2d::Zero();
	int LMBMousePosEventHandle = 0;
	int RMBMousePosEventHandle = 0;
	
public:
	AEGPlayer();
	virtual void Tick(float DeltaTime) override; // I keep it in case I need it in the future. It's ok because bCanEverTick == false.
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void UniformAddScale(float Amount) override;
	virtual void NonUniformAddScale(float X, float Y, float Z) override;
	virtual void UniformMultiplyScale(float Amount) override;
	virtual void NonUniformMultiplyScale(float X, float Y, float Z) override;

protected:
	virtual void BeginPlay() override;

private:
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
