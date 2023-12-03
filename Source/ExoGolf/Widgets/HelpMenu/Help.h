// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExoGolf/Datas/Interfaces/GameMenuInterface.h"
#include "Help.generated.h"

class AEGHUD;
class UButton;
/**
 * 
 */
UCLASS()
class EXOGOLF_API UHelp : public UUserWidget, public IGameMenuInterface
{
	GENERATED_BODY()

private:
	//==== Hidden Fields ====

	UPROPERTY()
	AEGHUD* HUD;
	
	//==== Widgets ====
	
	UPROPERTY(meta=(BindWidget))
	UButton* BackButton;

public:
	virtual void SetHUD(AEGHUD* Hud) override;
	
private:
	//==== Overrides ====

	virtual void NativeConstruct() override;
	
	//==== Event Handlers ====

	UFUNCTION()
	void BackButtonClicked();
};
