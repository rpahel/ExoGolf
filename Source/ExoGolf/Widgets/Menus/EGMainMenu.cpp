// Fill out your copyright notice in the Description page of Project Settings.


#include "EGMainMenu.h"

#include "EGMainMenuButton.h"
#include "Animation/UMGSequencePlayer.h"
#include "Components/Button.h"
#include "ExoGolf/Game/EGHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

//=======================================================================================|
//==== PUBLIC
//=======================================================================================|

void UEGMainMenu::SetHUD(AEGHUD* Hud)
{
	HUD = Hud;
}

//=======================================================================================|
//==== OVERRIDES
//=======================================================================================|

void UEGMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	bCanClick = true;
	SetUpButtons();
}

void UEGMainMenu::NativeDestruct()
{
	Super::NativeDestruct();

	ClickCooldownHandle.Invalidate();
	Buttons.Empty();
	HUD = nullptr;
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

	if(PlayButton)
		PlayButton->OnClickedDelegate.BindUObject(this, &UEGMainMenu::PlayButtonClicked);

	if(LevelsButton)
		LevelsButton->OnClickedDelegate.BindUObject(this, &UEGMainMenu::LevelsButtonClicked);
	
	if(HelpButton)
		HelpButton->OnClickedDelegate.BindUObject(this, &UEGMainMenu::HelpButtonClicked);

	if(QuitButton)
		QuitButton->OnClickedDelegate.BindUObject(this, &UEGMainMenu::QuitButtonClicked);

	Buttons = {PlayButton, LevelsButton, HelpButton, QuitButton};
	CurrentButtonIndex = 0;

	for (int i = 1; i < Buttons.Num(); i++)
	{
		if(Buttons[i])
			Buttons[i]->SetButtonRenderOpacity(0);
	}
}

UUMGSequencePlayer* UEGMainMenu::PlayClickAnimationForButton(UEGMainMenuButton* Button)
{
	if(!bCanClick || !Button)
		return nullptr;

	bCanClick = false;
	return Button->PlayButtonAnimation(EMainMenuButtonAnimation::Click);
}

//=======================================================================================|
//==== EVENT HANDLERS
//=======================================================================================|

void UEGMainMenu::RightButtonClicked()
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

void UEGMainMenu::LeftButtonClicked()
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

void UEGMainMenu::PlayButtonClicked()
{
	UUMGSequencePlayer* Animation = PlayClickAnimationForButton(PlayButton);
	Animation->OnSequenceFinishedPlaying().AddUObject(this, &UEGMainMenu::Start);
}

void UEGMainMenu::LevelsButtonClicked()
{
	UUMGSequencePlayer* Animation = PlayClickAnimationForButton(LevelsButton);

	//AnimationFinishedDelegate.BindDynamic(this, &UEGMainMenu::Start);
	//Animation->BindToAnimationFinished(this, AnimationFinishedDelegate);
}

void UEGMainMenu::HelpButtonClicked()
{
	UUMGSequencePlayer* Animation = PlayClickAnimationForButton(HelpButton);

	//AnimationFinishedDelegate.BindDynamic(this, &UEGMainMenu::Start);
	//Animation->BindToAnimationFinished(this, AnimationFinishedDelegate);
}

void UEGMainMenu::QuitButtonClicked()
{
	UUMGSequencePlayer* Animation = PlayClickAnimationForButton(QuitButton);
	Animation->OnSequenceFinishedPlaying().AddUObject(this, &UEGMainMenu::Quit);
}

void UEGMainMenu::Start(UUMGSequencePlayer& Sequence)
{
	if(HUD)
		HUD->LoadLevel(TEXT("Level_001"));
}

void UEGMainMenu::Quit(UUMGSequencePlayer& Sequence)
{
	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		EQuitPreference::Quit,
		false
	);
}
