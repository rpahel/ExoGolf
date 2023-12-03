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

UUMGSequencePlayer* UEGMainMenuButton::PlayButtonAnimation(const EMainMenuButtonAnimation& Animation)
{
	switch(Animation)
	{
		case EMainMenuButtonAnimation::Click:
		return PlayAnimation(ClickAnimation);

		case EMainMenuButtonAnimation::RightEnter:
		return PlayAnimation(RightEnterAnimation);

		case EMainMenuButtonAnimation::LeftEnter:
		return PlayAnimation(LeftEnterAnimation);

		case EMainMenuButtonAnimation::RightExit:
		return PlayAnimation(RightExitAnimation);

		case EMainMenuButtonAnimation::LeftExit:
		return PlayAnimation(LeftExitAnimation);
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
