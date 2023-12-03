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
	else if(CurrentLevel == Keys[Keys.Num() - 1])
	{
		KillCurrentWidget();
	}
	else
	{
		KillCurrentWidget();
		
		if(W_HUD)
		{
			HUD = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(),0), W_HUD);
			HUD->AddToViewport(0);
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
