// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

class UButton;
DECLARE_DELEGATE(FPauseMenuDelegate)

/**
 * 
 */
UCLASS()
class EXOGOLF_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	FPauseMenuDelegate OnResumeClicked;
	FPauseMenuDelegate OnLastStrikePositionClicked;
	FPauseMenuDelegate OnLevelsClicked;
	FPauseMenuDelegate OnHelpClicked;
	FPauseMenuDelegate OnQuitClicked;
	
private:
	//==== Widgets ====

	UPROPERTY(meta=(BindWidget))
	UButton* ResumeButton;

	UPROPERTY(meta=(BindWidget))
	UButton* LastStrikePositionButton;

	UPROPERTY(meta=(BindWidget))
	UButton* LevelsButton;

	UPROPERTY(meta=(BindWidget))
	UButton* HelpButton;

	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton;

private:
	//==== Overrides ====
	virtual void NativeConstruct() override;
	
	//==== Event Handlers ====
	
	UFUNCTION()
	void ResumeClicked();

	UFUNCTION()
	void LastStrikePositionClicked();

	UFUNCTION()
	void LevelsClicked();

	UFUNCTION()
	void HelpClicked();

	UFUNCTION()
	void QuitClicked();
};
