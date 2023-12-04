#include "CountersDisplay.h"

#include "Components/TextBlock.h"

//=====================================================================================
//==== PUBLIC
//=====================================================================================

void UCountersDisplay::ResetTimer()
{
	Seconds = 0;
	Minutes = 0;
	
	if(SecondsDisplay && MinutesDisplay)
	{
		SecondsDisplay->SetText(FText::AsNumber(Seconds, &ClockFormattingOptions));
		MinutesDisplay->SetText(FText::AsNumber(Minutes, &ClockFormattingOptions));
	}
}

void UCountersDisplay::IncrementSeconds()
{
	if(++Seconds % 60 == 0)
	{
		Seconds = 0;
		Minutes++;
	}

	if(SecondsDisplay && MinutesDisplay)
	{
		SecondsDisplay->SetText(FText::AsNumber(Seconds, &ClockFormattingOptions));
		MinutesDisplay->SetText(FText::AsNumber(Minutes, &ClockFormattingOptions));
	}
}

void UCountersDisplay::SetPar(const uint8 Amount) const
{
	if(ParDisplay)
		ParDisplay->SetText(FText::AsNumber(Amount, &ParFormattingOptions));
}

void UCountersDisplay::ResetStrikes()
{
	Strikes = 0;
	if(StrikesDisplay)
		StrikesDisplay->SetText(FText::AsNumber(Strikes, &StrikesFormattingOptions));
}

void UCountersDisplay::IncrementStrikes()
{
	Strikes++;
	if(StrikesDisplay)
		StrikesDisplay->SetText(FText::AsNumber(Strikes, &StrikesFormattingOptions));
}

//=====================================================================================
//==== OVERRIDES
//=====================================================================================

void UCountersDisplay::NativeConstruct()
{
	Super::NativeConstruct();

	ClockFormattingOptions.MinimumIntegralDigits = 2;
	ClockFormattingOptions.MaximumIntegralDigits = 2;
	
	ParFormattingOptions.MinimumIntegralDigits = 2;
	ParFormattingOptions.MaximumIntegralDigits = 2;
	
	StrikesFormattingOptions.MinimumIntegralDigits = 3;
	StrikesFormattingOptions.MaximumIntegralDigits = 3;
}
