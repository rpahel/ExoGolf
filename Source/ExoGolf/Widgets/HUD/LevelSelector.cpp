#include "LevelSelector.h"

#include "HeadsUpDisplay.h"
#include "Components/ScrollBox.h"
#include "Components/Spacer.h"
#include "ExoGolf/Datas/Data_Assets/LevelsData.h"
#include "ExoGolf/Game/EGHUD.h"
#include "ExoGolf/Widgets/MainMenu/MainMenuButton.h"
#include "Kismet/GameplayStatics.h"

void ULevelSelector::CreateLevelButtons()
{
	if(!ScrollBox || !LevelsData)
		return;

	if(ScrollBox->GetChildrenCount() > 1) // Account for the dummy
		return;

	int i = 0;
	for (auto Level : LevelsData->LevelsInGame)
	{
		if(i == 0)
		{
			i++;
			continue;
		}

		
		UMainMenuButton* LevelButton = Cast<UMainMenuButton>(CreateWidget(this, W_MainMenuButton));

		if(!LevelButton)
			continue;

		LevelButton->SetButtonText(FText::FromName(Level.Get<1>().DisplayName));
		LevelButton->SetButtonIndex(i);
		LevelButton->OnClickedRefDelegate.BindUObject(this, &ULevelSelector::LoadLevel);
		ScrollBox->AddChild(LevelButton);

		if(i==(LevelsData->LevelsInGame.Num() - 1))
			break;

		if(USpacer* Spacer = NewObject<USpacer>())
		{
			Spacer->SetSize(FVector2D(20,0));
			ScrollBox->AddChild(Spacer);
		}
		
		i++;
	}
}

void ULevelSelector::LoadLevel(UMainMenuButton* Button)
{
	if(!HUD || !LevelsData)
		return;

	if(HUDisplay)
		HUDisplay->KillTimer();

	TArray<FName> Levels;
	LevelsData->LevelsInGame.GetKeys(Levels);
	HUD->LoadLevel(Levels[Button->GetButtonIndex()]);
}

void ULevelSelector::SetHUD(AEGHUD* Hud)
{
	HUD = Hud;
}

void ULevelSelector::SetHeadsUpDisplay(UHeadsUpDisplay* HeadsUpDisplay)
{
	HUDisplay = HeadsUpDisplay;
}
