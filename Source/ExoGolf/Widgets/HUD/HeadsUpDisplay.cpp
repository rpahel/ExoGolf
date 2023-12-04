#include "HeadsUpDisplay.h"

#include "CountersDisplay.h"
#include "LevelSelector.h"
#include "LevelTitle.h"
#include "PauseMenu.h"
#include "ExoGolf/Actors/Pawns/Player/EGPlayer.h"
#include "ExoGolf/Datas/Data_Assets/LevelsData.h"
#include "ExoGolf/Game/EGHUD.h"
#include "Kismet/GameplayStatics.h"

//===============================================================================================
//==== PUBLIC
//===============================================================================================

void UHeadsUpDisplay::SetHUD(AEGHUD* Hud)
{
	HUD = Hud;
}

void UHeadsUpDisplay::ShowPauseMenu()
{
	if(PauseMenuWidget->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0);
		PauseMenuWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		HidePauseMenu();
	}
}

void UHeadsUpDisplay::HidePauseMenu()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
	PauseMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	HideAllHidableWidgets();
	Player->AllowInputs();
}

//===============================================================================================
//==== METHODS
//===============================================================================================

void UHeadsUpDisplay::HideAllHidableWidgets()
{
	if(PauseMenuWidget)
		PauseMenuWidget->SetVisibility(ESlateVisibility::Collapsed);

	if(LevelSelectorWidget)
		LevelSelectorWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	if(HelpWidget)
		HelpWidget->SetVisibility(ESlateVisibility::Collapsed);
}

//===============================================================================================
//==== OVERRIDES
//===============================================================================================

void UHeadsUpDisplay::NativeConstruct()
{
	Super::NativeConstruct();

	if(PauseMenuWidget)
	{
		PauseMenuWidget->OnResumeClicked.BindUObject(this, &UHeadsUpDisplay::HidePauseMenu);
		PauseMenuWidget->OnLastStrikePositionClicked.BindUObject(this, &UHeadsUpDisplay::ReturnToLastStrikePosition);
		PauseMenuWidget->OnLevelsClicked.BindUObject(this, &UHeadsUpDisplay::ShowLevelSelector);
		PauseMenuWidget->OnHelpClicked.BindUObject(this, &UHeadsUpDisplay::ShowHelp);
		PauseMenuWidget->OnQuitClicked.BindUObject(this, &UHeadsUpDisplay::ReturnToMainMenu);
	}

	Player = Cast<AEGPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(Player)
	{
		Player->OnPause.AddUObject(this, &UHeadsUpDisplay::ShowPauseMenu);
		Player->OnStrike.AddLambda(
			[this]
			{
				if(CountersWidget)
					CountersWidget->IncrementStrikes();
			});
	}
	
	const FName CurrentLevel = FName(UGameplayStatics::GetCurrentLevelName(GetWorld()));

	if(!LevelsData)
		return;
	
	if(LevelTitleWidget)
	{
		LevelTitleWidget->SetLevelName(LevelsData->LevelsInGame[CurrentLevel].DisplayName);
	}

	if(CountersWidget)
	{
		CountersWidget->ResetStrikes();
		CountersWidget->ResetTimer();
		CountersWidget->SetPar(LevelsData->LevelsInGame[CurrentLevel].ParStrikeAmount);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			[this]
			{
				if(CountersWidget)
					CountersWidget->IncrementSeconds();
			},
			1.f,
			true,
			1.f);
	}
}

//===============================================================================================
//==== EVENT HANDLERS
//===============================================================================================

void UHeadsUpDisplay::ReturnToLastStrikePosition()
{
	if(Player)
		Player->ReturnToLastPosition();

	ShowPauseMenu();
}

void UHeadsUpDisplay::ShowLevelSelector()
{
	if(!LevelSelectorWidget)
		return;

	LevelSelectorWidget->SetHUD(HUD);
	LevelSelectorWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UHeadsUpDisplay::ShowHelp()
{
	if(!HelpWidget)
		return;

	HelpWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UHeadsUpDisplay::ReturnToMainMenu()
{
	if(HUD)
		HUD->LoadLevel(FName("Level_MainMenu"));
}
