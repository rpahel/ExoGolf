// Fill out your copyright notice in the Description page of Project Settings.


#include "EGHUD.h"

#include "Blueprint/UserWidget.h"
#include "ExoGolf/Datas/Data_Assets/LevelsData.h"
#include "Kismet/GameplayStatics.h"

//=======================================================================================|
//==== PUBLIC ===========================================================================|
//=======================================================================================|

void AEGHUD::LoadNextLevel() const
{
	LoadLevelByIndex(LevelsData->CurrentLevelIndex + 1);
}

void AEGHUD::LoadLevelByIndex(int32 LevelIndex) const
{
	if(LevelIndex < 0 || LevelIndex >= LevelsData->LevelsInGame.Num())
		return;

	LevelsData->CurrentLevelIndex = LevelIndex;
	UGameplayStatics::OpenLevel(GetWorld(), LevelsData->LevelsInGame[LevelIndex], false);
}

void AEGHUD::LoadLevelByName(FName LevelName) const
{
	const int32 LevelIndex = LevelsData->LevelsInGame.IndexOfByKey(LevelName);
	
	if(LevelIndex != INDEX_NONE)
	{
		LevelsData->CurrentLevelIndex = LevelIndex;
		UGameplayStatics::OpenLevel(GetWorld(), LevelName, false);
	}
}

void AEGHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if(!LevelsData)
		return;

	if(LevelsData->CurrentLevelIndex == 0)
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
