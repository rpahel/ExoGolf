// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EGMainMenu.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class EXOGOLF_API UEGMainMenu : public UUserWidget
{
	GENERATED_BODY()


private:

    UPROPERTY(meta=(BindWidget))
    UButton* PlayButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* LevelsButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* HelpButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton;
};
