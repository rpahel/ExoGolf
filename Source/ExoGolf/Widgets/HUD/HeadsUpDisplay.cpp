#include "HeadsUpDisplay.h"

#include "CountersDisplay.h"
#include "FinishWidget.h"
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
	if(!Player)
		return;
	
	if(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		PauseMenuWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Player->SetCursorVisibility(true);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeUIOnly());
	}
	else
	{
		HidePauseMenu();
	}
}

void UHeadsUpDisplay::HidePauseMenu()
{
	if(!Player)
		return;
	
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	PauseMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	HideAllHidableWidgets();
	Player->AllowInputs();
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
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

	if(FinishWidget)
	{
		FinishWidget->OnNextLevelClickedDelegate.BindUObject(this, &UHeadsUpDisplay::NextLevel);
		FinishWidget->OnMainMenuClickedDelegate.BindUObject(this, &UHeadsUpDisplay::ReturnToMainMenu);
		FinishWidget->OnQuitClickedDelegate.BindUObject(this, &UHeadsUpDisplay::Quit);
	}

	Player = Cast<AEGPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(Player)
	{
		Player->OnPause.AddUObject(this, &UHeadsUpDisplay::ShowPauseMenu);
		Player->OnFinish.AddUObject(this, &UHeadsUpDisplay::ShowFinishMenu);
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

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(
			Handle,
			[this]
			{
				StartTimerCounter();
			},
			1.f,
			false,
			1.f);
	}
}

void UHeadsUpDisplay::NativeDestruct()
{
	Super::NativeDestruct();

	TimerHandle.Invalidate();
	LevelTitleWidget = nullptr;
	PauseMenuWidget = nullptr;
	CountersWidget = nullptr;
	LevelSelectorWidget = nullptr;
	HelpWidget = nullptr;
	FinishWidget = nullptr;
	Player = nullptr;
	HUD = nullptr;
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
	LevelSelectorWidget->SetHeadsUpDisplay(this);
	LevelSelectorWidget->CreateLevelButtons();
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
	KillTimer();
	if(HUD)
		HUD->LoadLevel(FName("Level_MainMenu"));
}

void UHeadsUpDisplay::ShowFinishMenu()
{
	KillTimer();
	if(FinishWidget && CountersWidget)
	{
		FinishWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		TTuple<uint8, uint8, uint16> Counters = CountersWidget->GetCounters();

		const FName CurrentLevelName = FName(UGameplayStatics::GetCurrentLevelName(GetWorld()));

		if(!LevelsData)
			return;

		TArray<FName> Keys;
		LevelsData->LevelsInGame.GetKeys(Keys);
		const int32 Index = Keys.IndexOfByKey(CurrentLevelName);
		const bool bHasNextLevel = Index != LevelsData->LevelsInGame.Num() - 1;
		FinishWidget->SetUp(Counters.Get<0>(), Counters.Get<1>(), Counters.Get<2>(), !bHasNextLevel);

		UGameplayStatics::SetGamePaused(GetWorld(), true);

		if(Player)
			Player->SetCursorVisibility(true);
	}
}

void UHeadsUpDisplay::Quit()
{
	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		EQuitPreference::Quit,
		false
	);
}

void UHeadsUpDisplay::NextLevel()
{
	KillTimer();
	if(HUD)
		HUD->LoadNextLevel();
}

void UHeadsUpDisplay::StartTimerCounter()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	
	if(CountersWidget)
		CountersWidget->ResetTimer();
	
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
