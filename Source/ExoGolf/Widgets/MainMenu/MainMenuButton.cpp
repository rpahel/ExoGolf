// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

//=======================================================================================|
//==== PUBLIC ===========================================================================|
//=======================================================================================|

void UMainMenuButton::SetButtonRenderOpacity(float Opacity) const
{
	if(Button)
		Button->SetRenderOpacity(Opacity);
}

void UMainMenuButton::SetButtonText(const FText& Text)
{
	ButtonText = Text;
	
	if(ButtonTextBlock)
		ButtonTextBlock->SetText(ButtonText);
}

void UMainMenuButton::SetButtonIndex(const int32 Index)
{
	ButtonIndex = Index;
}

int32 UMainMenuButton::GetButtonIndex() const
{
	return ButtonIndex;
}

UUMGSequencePlayer* UMainMenuButton::PlayButtonAnimation(const EMainMenuButtonAnimation& Animation)
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

void UMainMenuButton::NativeConstruct()
{
	Super::NativeConstruct();

	if(Button)
		Button->OnClicked.AddUniqueDynamic(this, &UMainMenuButton::ButtonClicked);

	if(ButtonTextBlock)
		ButtonTextBlock->SetText(ButtonText);
}

void UMainMenuButton::NativeDestruct()
{
	Super::NativeDestruct();

	OnClickedDelegate.Unbind();
	OnClickedRefDelegate.Unbind();
	OnClickAnimationFinishedDelegate.Unbind();
	OnClickAnimationFinishedRefDelegate.Unbind();
}

//=======================================================================================|
//==== EVENT HANDLERS ===================================================================|
//=======================================================================================|

void UMainMenuButton::ButtonClicked()
{
	OnClickedDelegate.ExecuteIfBound();
	OnClickedRefDelegate.ExecuteIfBound(this);
}
