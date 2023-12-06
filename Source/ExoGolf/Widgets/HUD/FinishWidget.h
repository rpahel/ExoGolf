// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FinishWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DELEGATE(FFinishWidgetDelegate)
/**
 * 
 */
UCLASS()
class EXOGOLF_API UFinishWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FFinishWidgetDelegate OnNextLevelClickedDelegate;
	FFinishWidgetDelegate OnMainMenuClickedDelegate;
	FFinishWidgetDelegate OnQuitClickedDelegate;

private:
	//==== Widgets ====

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TimerText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* StrikesText;

	UPROPERTY(meta=(BindWidget))
	UButton* NextLevelButton;

	UPROPERTY(meta=(BindWidget))
	UButton* MainMenuButton;

	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton;

public:
	void SetUp(const uint8 Minutes, const uint8 Seconds, const uint16 Strikes, const bool bHideNextLevelButton = false);

private:
	//==== Overrides ====

	virtual void NativeConstruct() override;

	//==== Event Handlers ====

	UFUNCTION()
	void NextLevelClicked();

	UFUNCTION()
	void MainMenuClicked();

	UFUNCTION()
	void QuitClicked();
};
