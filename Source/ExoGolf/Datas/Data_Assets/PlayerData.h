// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ExoGolf/Actors/Others/EGForceGauge.h"
#include "PlayerData.generated.h"

/**
 * 
 */
UCLASS()
class EXOGOLF_API UPlayerData : public UDataAsset
{
	GENERATED_BODY()

public:
	
	// The force gauge to show the strike force of the player.
	UPROPERTY(EditDefaultsOnly, Category = "Player|Gameplay|Strike")
	TSubclassOf<AEGForceGauge> ForceGauge = nullptr;
	
	// The minimum distance the player has to pull back its cursor to be considered a strike.
	UPROPERTY(EditDefaultsOnly, Category = "Player|Gameplay|Strike")
	float MinimumStrikeDistance = 0.f;

	// The impulse force applied to the ball at minimum strike distance.
	UPROPERTY(EditDefaultsOnly, Category = "Player|Gameplay|Strike")
	float MinimumForce = 1.f;

	// The distance the player needs to pull back its cursor to strike the ball with maximum force.
	UPROPERTY(EditDefaultsOnly, Category = "Player|Gameplay|Strike")
	float MaximumStrikeDistance = 100.f;

	// The impulse force applied to the ball at maximum strike distance.
	UPROPERTY(EditDefaultsOnly, Category = "Player|Gameplay|Strike")
	float MaximumForce = 100.f;

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
};
