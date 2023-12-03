// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExoGolf/Datas/Interfaces/GameMenuInterface.h"
#include "MainMenu.generated.h"

class AEGHUD;
class UMainMenuButton;
class UUMGSequencePlayer;
class UButton;
/**
 * 
 */
UCLASS()
class EXOGOLF_API UMainMenu : public UUserWidget, public IGameMenuInterface
{
	GENERATED_BODY()

private:
	//==== Hidden Fields ====
	bool bCanClick;
	int32 CurrentButtonIndex;
	FTimerHandle ClickCooldownHandle;

	UPROPERTY()
	AEGHUD* HUD;
	
	UPROPERTY()
	TArray<UMainMenuButton*> Buttons;
	
	//==== Widgets ====

	UPROPERTY(meta=(BindWidget))
	UButton* RightButton;

	UPROPERTY(meta=(BindWidget))
	UButton* LeftButton;
	
    UPROPERTY(meta=(BindWidget))
    UMainMenuButton* PlayButton;
	
	UPROPERTY(meta=(BindWidget))
	UMainMenuButton* LevelsButton;
	
	UPROPERTY(meta=(BindWidget))
	UMainMenuButton* HelpButton;
	
	UPROPERTY(meta=(BindWidget))
	UMainMenuButton* QuitButton;

public:
	virtual void SetHUD(AEGHUD* Hud) override;
	
private:
	//==== Overrides ====
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	//==== Methods ====

	void SetUpButtons();
	void Start(UUMGSequencePlayer& Sequence);
	void OpenLevelsMenu(UUMGSequencePlayer& Sequence);
	void Quit(UUMGSequencePlayer& Sequence);
	void DisableLeftAndRightButtons() const;
	UUMGSequencePlayer* PlayClickAnimationForButton(UMainMenuButton* Button);
	
	//==== Event Handlers ====

	UFUNCTION()
	void RightButtonClicked();

	UFUNCTION()
	void LeftButtonClicked();

	UFUNCTION()
	void PlayButtonClicked();
	
	UFUNCTION()
	void LevelsButtonClicked();

	UFUNCTION()
	void HelpButtonClicked();

	UFUNCTION()
	void QuitButtonClicked();
};
