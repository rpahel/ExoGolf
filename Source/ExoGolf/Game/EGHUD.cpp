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

void AEGHUD::LoadNextLevel() const
{
	const FName CurrentLevelName = FName(UGameplayStatics::GetCurrentLevelName(GetWorld()));
	if(!LevelsData)
		return;

	TArray<FName> Keys;
	LevelsData->LevelsInGame.GetKeys(Keys);
	const int32 Index = Keys.IndexOfByKey(CurrentLevelName);

	if(Index == LevelsData->LevelsInGame.Num() - 1)
		return;

	LoadLevel(Keys[Index+1]);
}

void AEGHUD::LoadMenu(const EMenu Menu)
{
	KillCurrentWidget();
	
	switch(Menu)
	{
	case EMenu::MainMenu:
		if(W_MainMenu)
		{
			CurrentWidget = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(),0), W_MainMenu);
			CurrentWidget->AddToViewport(0);
		}
		break;

	case EMenu::LevelsMenu:
		if(W_LevelsMenu)
		{
			CurrentWidget = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(),0), W_LevelsMenu);
			CurrentWidget->AddToViewport(0);
		}
		break;

	case EMenu::HelpMenu:
		if(W_HelpMenu)
		{
			CurrentWidget = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(),0), W_HelpMenu);
			CurrentWidget->AddToViewport(0);
		}
		break;
	}

	SetHUDInCurrentWidget();
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
		KillCurrentWidget();
		
		if(W_MainMenu)
		{
			CurrentWidget = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(),0), W_MainMenu);
			CurrentWidget->AddToViewport(0);
		}
		
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
	}
	else
	{
		KillCurrentWidget();
		
		if(W_HUD)
		{
			CurrentWidget = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(),0), W_HUD);
			CurrentWidget->AddToViewport(0);
		}
	}

	SetHUDInCurrentWidget();
}

void AEGHUD::KillCurrentWidget()
{
	if(CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
}

void AEGHUD::SetHUDInCurrentWidget()
{
	if(IGameMenuInterface* GameMenu = Cast<IGameMenuInterface>(CurrentWidget))
	{
		GameMenu->SetHUD(this);
	}
}
