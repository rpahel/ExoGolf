// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

//==============================================================================================
//==== PUBLIC
//==============================================================================================

void UFinishWidget::SetUp(const uint8 Minutes, const uint8 Seconds, const uint16 Strikes, const bool bHideNextLevelButton)
{
	FNumberFormattingOptions NumberFormattingOptions;
	if(TimerText)
	{
		NumberFormattingOptions.MinimumIntegralDigits = 2;
		NumberFormattingOptions.MaximumIntegralDigits = 2;
		const FString String = FText::AsNumber(Minutes, &NumberFormattingOptions).ToString() + FText::FromString(":").ToString() + FText::AsNumber(Seconds, &NumberFormattingOptions).ToString();
		TimerText->SetText(FText::FromString(String));
	}

	if(Strikes)
	{
		NumberFormattingOptions.MinimumIntegralDigits = 3;
		NumberFormattingOptions.MaximumIntegralDigits = 3;
		const FText Text = FText::AsNumber(Strikes, &NumberFormattingOptions);
		TimerText->SetText(Text);
	}

	if(!NextLevelButton)
		return;
	
	if(bHideNextLevelButton)
	{
		NextLevelButton->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		NextLevelButton->OnClicked.AddUniqueDynamic(this, &UFinishWidget::NextLevelClicked);
	}
}

//==============================================================================================
//==== OVERRIDES
//==============================================================================================

void UFinishWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(MainMenuButton)
		MainMenuButton->OnClicked.AddUniqueDynamic(this, &UFinishWidget::MainMenuClicked);

	if(QuitButton)
		QuitButton->OnClicked.AddUniqueDynamic(this, &UFinishWidget::QuitClicked);
}

//==============================================================================================
//==== EVENT HANDLERS
//==============================================================================================

void UFinishWidget::NextLevelClicked()
{
	OnNextLevelClickedDelegate.ExecuteIfBound();
}

void UFinishWidget::MainMenuClicked()
{
	OnMainMenuClickedDelegate.ExecuteIfBound();
}

void UFinishWidget::QuitClicked()
{
	OnQuitClickedDelegate.ExecuteIfBound();
}
