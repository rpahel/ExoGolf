// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExoGolf/Datas/Interfaces/GameMenuInterface.h"
#include "LevelsMenu.generated.h"

class UButton;
class AEGHUD;
class UMainMenuButton;
class UScrollBox;
class ULevelsData;
/**
 * 
 */
UCLASS()
class EXOGOLF_API ULevelsMenu : public UUserWidget, public IGameMenuInterface
{
	GENERATED_BODY()

private:
	//==== Exposed Fields ====

	UPROPERTY(EditAnywhere)
	ULevelsData* LevelsData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> W_MainMenuButton;

	//==== Hidden Fields ====

	FName SelectedLevel;
	FTimerHandle PostCreationWaitTimerHandle;
	TArray<UMainMenuButton*> Buttons;
	TArray<FName> Levels;

	UPROPERTY()
	AEGHUD* HUD;
	
	//==== Widgets ====

	UPROPERTY(meta=(BindWidget))
	UScrollBox* ScrollBox;

	UPROPERTY(meta=(BindWidget))
	UButton* BackButton;

public:
	virtual void SetHUD(AEGHUD* Hud) override;
	
private:
	//==== Overrides ====
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	//==== Methods ====

	void SetUpButtons();
	void PlaySelectedLevel(UUMGSequencePlayer& Sequence);

	//==== Event Handlers

	UFUNCTION()
	void BackButtonClicked();

	UFUNCTION()
	void LevelButtonClicked(UMainMenuButton* Button);
};
