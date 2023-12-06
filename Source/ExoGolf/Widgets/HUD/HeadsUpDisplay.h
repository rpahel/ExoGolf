// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExoGolf/Datas/Interfaces/GameMenuInterface.h"
#include "HeadsUpDisplay.generated.h"

class UFinishWidget;
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
	FTimerHandle TimerHandle;

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

	UPROPERTY(meta=(BindWidget))
	UFinishWidget* FinishWidget;

public:
	virtual void SetHUD(AEGHUD* Hud) override;

	UFUNCTION()
	void ShowPauseMenu();

	UFUNCTION()
	void HidePauseMenu();

private:
	//==== Methods ====

	void HideAllHidableWidgets();
	
	//==== Overrides ====

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	//==== Event Handlers ====

	UFUNCTION()
	void ReturnToLastStrikePosition();
	
	UFUNCTION()
	void ShowLevelSelector();

	UFUNCTION()
	void ShowHelp();

	UFUNCTION()
	void ReturnToMainMenu();

	UFUNCTION()
	void ShowFinishMenu();

	UFUNCTION()
	void Quit();

	UFUNCTION()
	void NextLevel();
};
