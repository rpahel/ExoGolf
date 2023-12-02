// Fill out your copyright notice in the Description page of Project Settings.


#include "EGMainMenuButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

//=======================================================================================|
//==== PUBLIC ===========================================================================|
//=======================================================================================|

void UEGMainMenuButton::SetButtonRenderOpacity(float Opacity) const
{
	if(Button)
		Button->SetRenderOpacity(Opacity);
}

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

	if(ButtonTextBlock)
		ButtonTextBlock->SetText(ButtonText);
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
