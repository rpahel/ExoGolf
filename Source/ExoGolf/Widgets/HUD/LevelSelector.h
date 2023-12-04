// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExoGolf/Datas/Interfaces/GameMenuInterface.h"
#include "LevelSelector.generated.h"

class UMainMenuButton;
class ULevelsData;
class UScrollBox;
class AEGHUD;
/**
 * 
 */
UCLASS()
class EXOGOLF_API ULevelSelector : public UUserWidget, public IGameMenuInterface
{
	GENERATED_BODY()

private:
	//==== Exposed Fields ====

	UPROPERTY(EditAnywhere)
	ULevelsData* LevelsData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainMenuButton> W_MainMenuButton;

	//==== Hidden Fields ====

	UPROPERTY()
	AEGHUD* HUD;
	
	//==== Widgets ====
	
	UPROPERTY(meta=(BindWidget))
	UScrollBox* ScrollBox; 

public:
	virtual void SetHUD(AEGHUD* Hud) override;
	
private:
	virtual void NativeConstruct() override;
	void CreateLevelButtons();
	void LoadLevel(UMainMenuButton* Button);
};
