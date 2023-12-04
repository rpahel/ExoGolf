#include "PauseMenu.h"

#include "Components/Button.h"

//==============================================================
//==== OVERRIDES
//==============================================================

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if(ResumeButton)
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenu::ResumeClicked);

	if(LastStrikePositionButton)
		LastStrikePositionButton->OnClicked.AddDynamic(this, &UPauseMenu::LastStrikePositionClicked);

	if(LevelsButton)
		LevelsButton->OnClicked.AddDynamic(this, &UPauseMenu::LevelsClicked);

	if(HelpButton)
		HelpButton->OnClicked.AddDynamic(this, &UPauseMenu::HelpClicked);

	if(QuitButton)
		QuitButton->OnClicked.AddDynamic(this, &UPauseMenu::QuitClicked);
}

//==============================================================
//==== EVENT HANDLERS
//==============================================================

void UPauseMenu::ResumeClicked()
{
	OnResumeClicked.ExecuteIfBound();
}

void UPauseMenu::LastStrikePositionClicked()
{
	OnLastStrikePositionClicked.ExecuteIfBound();
}

void UPauseMenu::LevelsClicked()
{
	OnLevelsClicked.ExecuteIfBound();
}

void UPauseMenu::HelpClicked()
{
	OnHelpClicked.ExecuteIfBound();
}

void UPauseMenu::QuitClicked()
{
	OnQuitClicked.ExecuteIfBound();
}
