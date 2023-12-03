// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EGMainMenu.generated.h"

class AEGHUD;
class UEGMainMenuButton;
class UUMGSequencePlayer;
class UButton;
/**
 * 
 */
UCLASS()
class EXOGOLF_API UEGMainMenu : public UUserWidget
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

public:
	void SetHUD(AEGHUD* Hud);
	
private:
	//==== Overrides ====
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	//==== Methods ====

	void SetUpButtons();
	void Start(UUMGSequencePlayer& Sequence);
	void Quit(UUMGSequencePlayer& Sequence);
	UUMGSequencePlayer* PlayClickAnimationForButton(UEGMainMenuButton* Button);
	
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
