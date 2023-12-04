#include "LevelSelector.h"

#include "Components/ScrollBox.h"
#include "ExoGolf/Datas/Data_Assets/LevelsData.h"
#include "ExoGolf/Game/EGHUD.h"
#include "ExoGolf/Widgets/MainMenu/MainMenuButton.h"

void ULevelSelector::CreateLevelButtons()
{
	if(!ScrollBox || !LevelsData)
		return;

	int i = 0;
	for (auto Level : LevelsData->LevelsInGame)
	{
		if(i==0 || i==LevelsData->LevelsInGame.Num())
			continue;

		i++;
		UMainMenuButton* LevelButton = Cast<UMainMenuButton>(CreateWidget(this, W_MainMenuButton));

		if(!LevelButton)
			continue;

		LevelButton->SetButtonText(FText::FromName(Level.Get<1>().DisplayName));
		LevelButton->SetButtonIndex(i);
		LevelButton->OnClickedRefDelegate.BindUObject(this, &ULevelSelector::LoadLevel);
		ScrollBox->AddChild(LevelButton);
	}
}

void ULevelSelector::LoadLevel(UMainMenuButton* Button)
{
	if(!HUD || !LevelsData)
		return;

	TArray<FName> Levels;
	LevelsData->LevelsInGame.GetKeys(Levels);
	HUD->LoadLevel(Levels[Button->GetButtonIndex()]);
}

void ULevelSelector::SetHUD(AEGHUD* Hud)
{
	HUD = Hud;
}

void ULevelSelector::NativeConstruct()
{
	Super::NativeConstruct();

	CreateLevelButtons();
}
