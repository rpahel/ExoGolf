// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "CreditsButton.generated.h"

/**
 * 
 */
UCLASS()
class EXOGOLF_API UCreditsButton : public UButton
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FString UrlToLaunch;

public:
	void SetUpButton();

private:
	UFUNCTION()
	void OpenUrl();
};
