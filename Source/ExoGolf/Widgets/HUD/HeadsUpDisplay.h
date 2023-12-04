// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExoGolf/Datas/Interfaces/GameMenuInterface.h"
#include "HeadsUpDisplay.generated.h"

class AEGHUD;
class AEGPlayer;
class ULevelsData;
class UCountersDisplay;
class UPauseMenu;
class ULevelSelector;
class ULevelTitle;
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
	UPauseMenu* PauseMenuWidget;

	UPROPERTY(meta=(BindWidget))
	UCountersDisplay* CountersWidget;
	
	UPROPERTY(meta=(BindWidget))
	ULevelSelector* LevelSelectorWidget;

	UPROPERTY(meta=(BindWidget))
	UUserWidget* HelpWidget;

public:
	virtual void SetHUD(AEGHUD* Hud) override;
	void ShowPauseMenu();

	UFUNCTION()
	void HidePauseMenu();

private:
	//==== Methods ====

	void HideAllHidableWidgets();
	
	//==== Overrides ====

	virtual void NativeConstruct() override;
	
	//==== Event Handlers ====

	UFUNCTION()
	void ReturnToLastStrikePosition();
	
	UFUNCTION()
	void ShowLevelSelector();

	UFUNCTION()
	void ShowHelp();

	UFUNCTION()
	void ReturnToMainMenu();
};
