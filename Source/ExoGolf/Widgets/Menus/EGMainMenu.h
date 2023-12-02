// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EGMainMenu.generated.h"

class UEGMainMenuButton;
class UButton;
/**
 * 
 */
UCLASS()
class EXOGOLF_API UEGMainMenu : public UUserWidget
{
	GENERATED_BODY()

private:
	//==== Exposed Fields ====
	int32 CurrentButtonIndex;
	
	UPROPERTY()
	TArray<UEGMainMenuButton*> Buttons;
	
	//==== Widgets ====

	UPROPERTY(meta=(BindWidget))
	UButton* RightButton;

	UPROPERTY(meta=(BindWidget))
	UButton* LeftButton;
	
    UPROPERTY(meta=(BindWidget))
    UEGMainMenuButton* PlayButton;
	
	UPROPERTY(meta=(BindWidget))
	UEGMainMenuButton* LevelsButton;
	
	UPROPERTY(meta=(BindWidget))
	UEGMainMenuButton* HelpButton;
	
	UPROPERTY(meta=(BindWidget))
	UEGMainMenuButton* QuitButton;

private:
	//==== Overrides ====
	
	virtual void NativeConstruct() override;

	//==== Methods ====

	void SetUpButtons();
	
	//==== Event Handlers ====

	UFUNCTION()
	void RightButtonClicked();

	UFUNCTION()
	void LeftButtonClicked();
};
