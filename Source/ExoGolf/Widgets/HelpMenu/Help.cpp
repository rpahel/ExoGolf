// Fill out your copyright notice in the Description page of Project Settings.


#include "Help.h"

#include "Components/Button.h"
#include "ExoGolf/Game/EGHUD.h"

void UHelp::SetHUD(AEGHUD* Hud)
{
	HUD = Hud;
}

void UHelp::NativeConstruct()
{
	Super::NativeConstruct();

	if(BackButton)
		BackButton->OnClicked.AddUniqueDynamic(this, &UHelp::BackButtonClicked);
}

void UHelp::BackButtonClicked()
{
	if(HUD)
		HUD->LoadLevel(FName("Level_MainMenu"));
}
