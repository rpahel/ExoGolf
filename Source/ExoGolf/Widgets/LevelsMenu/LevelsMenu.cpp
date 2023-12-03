// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelsMenu.h"

#include "Animation/UMGSequencePlayer.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "ExoGolf/Datas/Data_Assets/LevelsData.h"
#include "ExoGolf/Game/EGHUD.h"
#include "ExoGolf/Widgets/MainMenu/MainMenuButton.h"

//=========================================================================================
//==== PUBLIC
//=========================================================================================

void ULevelsMenu::SetHUD(AEGHUD* Hud)
{
	HUD = Hud;
}

//=========================================================================================
//==== OVERRIDES
//=========================================================================================

void ULevelsMenu::NativeConstruct()
{
	Super::NativeConstruct();

	SetUpButtons();
}

void ULevelsMenu::NativeDestruct()
{
	Super::NativeDestruct();

	PostCreationWaitTimerHandle.Invalidate();
	Buttons.Empty();
	HUD = nullptr;
}

//=========================================================================================
//==== METHODS
//=========================================================================================

void ULevelsMenu::SetUpButtons()
{
	if(BackButton)
		BackButton->OnClicked.AddUniqueDynamic(this, &ULevelsMenu::BackButtonClicked);
	
	if(!LevelsData)
		return;

	Levels.Empty();
	LevelsData->LevelsInGame.GetKeys(Levels);
	
	for(int32 i = 1; i < Levels.Num() - 1; i++)
	{
		UUserWidget* LevelButton = CreateWidget(this, W_MainMenuButton);

		if(!LevelButton)
			continue;

		if(UMainMenuButton* CastButton = Cast<UMainMenuButton>(LevelButton))
		{
			CastButton->SetButtonText(FText::FromName(LevelsData->LevelsInGame[Levels[i]].DisplayName));
			CastButton->SetButtonIndex(i);
			CastButton->SetButtonRenderOpacity(0);
			CastButton->OnClickedRefDelegate.BindUObject(this, &ULevelsMenu::LevelButtonClicked);

			if(UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(ScrollBox->AddChild(CastButton)))
			{
				ScrollBoxSlot->SetPadding(FMargin(10.f));
			}
			
			Buttons.Add(CastButton);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(
		PostCreationWaitTimerHandle,
		[this]
		{
			if(Buttons.IsEmpty())
				return;
			
			uint32 FlipFlop = 0;
			for (UMainMenuButton* Button : Buttons)
			{
				if(!Button)
					continue;
				
				Button->PlayButtonAnimation(FlipFlop % 2 == 0 ? EMainMenuButtonAnimation::LeftEnter : EMainMenuButtonAnimation::RightEnter);
				FlipFlop++;
			}
		},
		1.f,
		false);
}

void ULevelsMenu::PlaySelectedLevel(UUMGSequencePlayer& Sequence)
{
	if(!HUD || SelectedLevel == FName(""))
		return;

	HUD->LoadLevel(SelectedLevel);
}

//=========================================================================================
//==== EVENT HANDLERS
//=========================================================================================

void ULevelsMenu::BackButtonClicked()
{
	if(HUD)
		HUD->LoadMenu(EMenu::MainMenu);
}

void ULevelsMenu::LevelButtonClicked(UMainMenuButton* Button)
{
	if(!Button || Levels.IsEmpty())
		return;

	int32 Index = Button->GetButtonIndex();
	if(Index <= 0 || Index >= Levels.Num() - 1)
		return;
	
	SelectedLevel = Levels[Index];
	UUMGSequencePlayer* Sequence = Button->PlayButtonAnimation(EMainMenuButtonAnimation::Click);
	Sequence->OnSequenceFinishedPlaying().AddUObject(this, &ULevelsMenu::PlaySelectedLevel);

	uint32 FlipFlop = 0;
	for (UMainMenuButton* Element : Buttons)
	{
		if(Element == Button)
			continue;

		Element->PlayButtonAnimation(FlipFlop % 2 == 0 ? EMainMenuButtonAnimation::LeftExit : EMainMenuButtonAnimation::RightExit);
		FlipFlop++;
	}
}
