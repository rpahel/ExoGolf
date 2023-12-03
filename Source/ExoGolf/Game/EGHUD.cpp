// Fill out your copyright notice in the Description page of Project Settings.


#include "EGHUD.h"

#include "Blueprint/UserWidget.h"
#include "ExoGolf/Datas/Data_Assets/LevelsData.h"
#include "ExoGolf/Widgets/MainMenu/MainMenu.h"
#include "Kismet/GameplayStatics.h"

//=======================================================================================|
//==== PUBLIC ===========================================================================|
//=======================================================================================|

void AEGHUD::LoadLevel(FName LevelName) const
{
	if(!LevelsData->LevelsInGame.Contains(LevelName))
		return;
	
	UGameplayStatics::OpenLevel(GetWorld(), LevelName, false);
}

void AEGHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if(!LevelsData)
		return;

	const FName CurrentLevel = FName(UGameplayStatics::GetCurrentLevelName(GetWorld()));
	TArray<FName> Keys;
	if(LevelsData->LevelsInGame.GetKeys(Keys) <= 0)
		return;
	
	if(CurrentLevel == Keys[0])
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

		if(UMainMenu* CastMainMenu = Cast<UMainMenu>(MainMenu))
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
