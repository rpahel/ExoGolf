// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExoGolf/Datas/Interfaces/GameMenuInterface.h"
#include "HeadsUpDisplay.generated.h"

class AEGPlayer;
class ULevelsData;
class ULevelSelector;
class ULevelTitle;
class AEGHUD;
/**
 * 
 */
UCLASS()
class EXOGOLF_API UHeadsUpDisplay : public UUserWidget, public IGameMenuInterface
{
	GENERATED_BODY()

private:
	//==== Exposed Fields ====

	UPROPERTY(EditAnywhere)
	ULevelsData* LevelsData;
	
	//==== Hidden Fields ====

	UPROPERTY()
	AEGPlayer* Player;
	
	UPROPERTY()
	AEGHUD* HUD;
	
	//==== Widgets ====
	
	UPROPERTY(meta=(BindWidget))
	ULevelTitle* LevelTitleWidget;

	UPROPERTY(meta=(BindWidget))
	ULevelSelector* LevelSelectorWidget;

public:
	virtual void SetHUD(AEGHUD* Hud) override;
	void SetUp();
	void ShowPauseMenu();
	void HidePauseMenu();

private:
	UFUNCTION()
	void ShowLevelSelector();

	UFUNCTION()
	void ShowHelp();
};
