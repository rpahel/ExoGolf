// Fill out your copyright notice in the Description page of Project Settings.


#include "EGMainMenuButton.h"

#include "Components/Button.h"
#include "ExoGolf/Datas/Enums.h"

//=======================================================================================|
//==== PUBLIC ===========================================================================|
//=======================================================================================|

UWidgetAnimation* UEGMainMenuButton::PlayButtonAnimation(const EMainMenuButtonAnimation& Animation)
{
	switch(Animation)
	{
		case EMainMenuButtonAnimation::Click:
		PlayAnimation(ClickAnimation);
		return ClickAnimation;

		case EMainMenuButtonAnimation::RightEnter:
		PlayAnimation(RightEnterAnimation);
		return RightEnterAnimation;

		case EMainMenuButtonAnimation::LeftEnter:
		PlayAnimation(LeftEnterAnimation);
		return LeftEnterAnimation;

		case EMainMenuButtonAnimation::RightExit:
		PlayAnimation(RightExitAnimation);
		return RightExitAnimation;

		case EMainMenuButtonAnimation::LeftExit:
		PlayAnimation(LeftExitAnimation);
		return LeftExitAnimation;
	}

	return nullptr;
}

//=======================================================================================|
//==== OVERRIDES ========================================================================|
//=======================================================================================|

void UEGMainMenuButton::NativeConstruct()
{
	Super::NativeConstruct();

	if(Button)
		Button->OnClicked.AddUniqueDynamic(this, &UEGMainMenuButton::ButtonClicked);
}

void UEGMainMenuButton::NativeDestruct()
{
	Super::NativeDestruct();

	OnClickedDelegate.Unbind();
}

//=======================================================================================|
//==== EVENT HANDLERS ===================================================================|
//=======================================================================================|

void UEGMainMenuButton::ButtonClicked()
{
	OnClickedDelegate.ExecuteIfBound();
}
