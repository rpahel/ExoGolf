// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "CreditsButton.h"
#include "MainMenuButton.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/Button.h"
#include "ExoGolf/Game/EGHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

//=======================================================================================|
//==== PUBLIC
//=======================================================================================|

void UMainMenu::SetHUD(AEGHUD* Hud)
{
	HUD = Hud;
}

//=======================================================================================|
//==== OVERRIDES
//=======================================================================================|

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	bCanClick = true;
	SetUpButtons();
}

void UMainMenu::NativeDestruct()
{
	Super::NativeDestruct();

	ClickCooldownHandle.Invalidate();
	Buttons.Empty();
	HUD = nullptr;
}

//=======================================================================================|
//==== METHODS
//=======================================================================================|

void UMainMenu::SetUpButtons()
{
	if(RightButton)
		RightButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::RightButtonClicked);

	if(LeftButton)
		LeftButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::LeftButtonClicked);

	if(PlayButton)
		PlayButton->OnClickedDelegate.BindUObject(this, &UMainMenu::PlayButtonClicked);

	if(LevelsButton)
		LevelsButton->OnClickedDelegate.BindUObject(this, &UMainMenu::LevelsButtonClicked);
	
	if(HelpButton)
		HelpButton->OnClickedDelegate.BindUObject(this, &UMainMenu::HelpButtonClicked);

	if(QuitButton)
		QuitButton->OnClickedDelegate.BindUObject(this, &UMainMenu::QuitButtonClicked);

	Buttons = {PlayButton, LevelsButton, HelpButton, QuitButton};
	CurrentButtonIndex = 0;

	for (int i = 1; i < Buttons.Num(); i++)
	{
		if(Buttons[i])
			Buttons[i]->SetButtonRenderOpacity(0);
	}

	if(RpahelButton)
		RpahelButton->SetUpButton();

	if(KikiButton)
		KikiButton->SetUpButton();
}

void UMainMenu::DisableLeftAndRightButtons() const
{
	if(!RightButton || !LeftButton)
		return;

	RightButton->SetVisibility(ESlateVisibility::HitTestInvisible);
	LeftButton->SetVisibility(ESlateVisibility::HitTestInvisible);
}

UUMGSequencePlayer* UMainMenu::PlayClickAnimationForButton(UMainMenuButton* Button)
{
	if(!bCanClick || !Button)
		return nullptr;

	bCanClick = false;
	return Button->PlayButtonAnimation(EMainMenuButtonAnimation::Click);
}

//=======================================================================================|
//==== EVENT HANDLERS
//=======================================================================================|

void UMainMenu::RightButtonClicked()
{
	if(!bCanClick)
		return;
	
	Buttons[CurrentButtonIndex]->PlayButtonAnimation(EMainMenuButtonAnimation::LeftExit);

	CurrentButtonIndex++;
	if(CurrentButtonIndex >= Buttons.Num())
		CurrentButtonIndex = 0;
	
	Buttons[CurrentButtonIndex]->PlayButtonAnimation(EMainMenuButtonAnimation::RightEnter);

	bCanClick = false;
	GetWorld()->GetTimerManager().SetTimer(
		ClickCooldownHandle,
		[this]{bCanClick = true;},
		0.75f,
		false);
}

void UMainMenu::LeftButtonClicked()
{
	if(!bCanClick)
		return;
	
	Buttons[CurrentButtonIndex]->PlayButtonAnimation(EMainMenuButtonAnimation::RightExit);

	CurrentButtonIndex--;
	if(CurrentButtonIndex < 0)
		CurrentButtonIndex = Buttons.Num()-1;

	Buttons[CurrentButtonIndex]->PlayButtonAnimation(EMainMenuButtonAnimation::LeftEnter);

	bCanClick = false;
	GetWorld()->GetTimerManager().SetTimer(
		ClickCooldownHandle,
		[this]{bCanClick = true;},
		0.75f,
		false);
}

void UMainMenu::PlayButtonClicked()
{
	if(!bCanClick)
		return;
	
	UUMGSequencePlayer* Animation = PlayClickAnimationForButton(PlayButton);
	if(!Animation)
		return;
	
	Animation->OnSequenceFinishedPlaying().AddUObject(this, &UMainMenu::Start);
	DisableLeftAndRightButtons();
}

void UMainMenu::LevelsButtonClicked()
{
	if(!bCanClick)
		return;
	
	UUMGSequencePlayer* Animation = PlayClickAnimationForButton(LevelsButton);
	if(!Animation)
		return;
	
	Animation->OnSequenceFinishedPlaying().AddUObject(this, &UMainMenu::OpenLevelsMenu);
	DisableLeftAndRightButtons();
}

void UMainMenu::HelpButtonClicked()
{
	if(!bCanClick)
		return;
	
	UUMGSequencePlayer* Animation = PlayClickAnimationForButton(HelpButton);
	if(!Animation)
		return;
	
	Animation->OnSequenceFinishedPlaying().AddUObject(this, &UMainMenu::OpenHelpMenu);
	DisableLeftAndRightButtons();
}

void UMainMenu::QuitButtonClicked()
{
	if(!bCanClick)
		return;
	
	UUMGSequencePlayer* Animation = PlayClickAnimationForButton(QuitButton);
	
	if(!Animation)
		return;
	
	Animation->OnSequenceFinishedPlaying().AddUObject(this, &UMainMenu::Quit);
	DisableLeftAndRightButtons();
}

void UMainMenu::Start(UUMGSequencePlayer& Sequence)
{
	if(HUD)
		HUD->LoadLevel(TEXT("Level_001"));
}

void UMainMenu::OpenLevelsMenu(UUMGSequencePlayer& Sequence)
{
	if(HUD)
		HUD->LoadMenu(EMenu::LevelsMenu);
}

void UMainMenu::OpenHelpMenu(UUMGSequencePlayer& Sequence)
{
	if(HUD)
		HUD->LoadMenu(EMenu::HelpMenu);
}

void UMainMenu::Quit(UUMGSequencePlayer& Sequence)
{
	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		EQuitPreference::Quit,
		false
	);
}
