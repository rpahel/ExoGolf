// Fill out your copyright notice in the Description page of Project Settings.


#include "EGMainMenu.h"

#include "EGMainMenuButton.h"
#include "Components/Button.h"

//=======================================================================================|
//==== OVERRIDES
//=======================================================================================|

void UEGMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	SetUpButtons();
}

//=======================================================================================|
//==== METHODS
//=======================================================================================|

void UEGMainMenu::SetUpButtons()
{
	if(RightButton)
		RightButton->OnClicked.AddUniqueDynamic(this, &UEGMainMenu::RightButtonClicked);

	if(LeftButton)
		LeftButton->OnClicked.AddUniqueDynamic(this, &UEGMainMenu::LeftButtonClicked);

	Buttons = {PlayButton, LevelsButton, HelpButton, QuitButton};
	CurrentButtonIndex = 0;

	for (int i = 1; i < Buttons.Num(); i++)
	{
		if(Buttons[i])
			Buttons[i]->SetButtonRenderOpacity(0);
	}
}

//=======================================================================================|
//==== EVENT HANDLERS
//=======================================================================================|

void UEGMainMenu::RightButtonClicked()
{
	Buttons[CurrentButtonIndex]->PlayButtonAnimation(EMainMenuButtonAnimation::LeftExit);

	CurrentButtonIndex++;
	if(CurrentButtonIndex >= Buttons.Num())
		CurrentButtonIndex = 0;
	
	Buttons[CurrentButtonIndex]->PlayButtonAnimation(EMainMenuButtonAnimation::RightEnter);
}

void UEGMainMenu::LeftButtonClicked()
{
	Buttons[CurrentButtonIndex]->PlayButtonAnimation(EMainMenuButtonAnimation::RightExit);

	CurrentButtonIndex--;
	if(CurrentButtonIndex < 0)
		CurrentButtonIndex = Buttons.Num()-1;
	
	Buttons[CurrentButtonIndex]->PlayButtonAnimation(EMainMenuButtonAnimation::LeftEnter);
}
