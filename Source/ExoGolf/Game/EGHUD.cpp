// Fill out your copyright notice in the Description page of Project Settings.


#include "EGHUD.h"

#include "Blueprint/UserWidget.h"
#include "ExoGolf/Datas/Data_Assets/LevelsData.h"
#include "ExoGolf/Widgets/Menus/EGMainMenu.h"
#include "Kismet/GameplayStatics.h"

//=======================================================================================|
//==== PUBLIC ===========================================================================|
//=======================================================================================|

void AEGHUD::LoadLevel(FName LevelName) const
{
	const int32 LevelIndex = LevelsData->LevelsInGame.IndexOfByKey(LevelName);
	
	if(LevelIndex != INDEX_NONE)
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelName, false);
	}
}

void AEGHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if(!LevelsData)
		return;

	const FName CurrentLevel = FName(UGameplayStatics::GetCurrentLevelName(GetWorld()));
	const FName FirstLevel = LevelsData->LevelsInGame[0];
	
	if(CurrentLevel == FirstLevel)
	{
		if(MainMenu)
		{
			MainMenu->RemoveFromParent();
			MainMenu = nullptr;
		}

		if(W_MainMenu)
		{
			MainMenu = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(),0), W_MainMenu);
			MainMenu->AddToViewport(0);
		}

		if(UEGMainMenu* CastMainMenu = Cast<UEGMainMenu>(MainMenu))
		{
			CastMainMenu->SetHUD(this);
		}
		
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
	}
	else
	{
		if(HUD)
		{
			HUD->RemoveFromParent();
			HUD = nullptr;
		}

		if(W_HUD)
		{
			HUD = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(),0), W_HUD);
			HUD->AddToViewport(0);
		}
	}
}
