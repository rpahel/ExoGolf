// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountersDisplay.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class EXOGOLF_API UCountersDisplay : public UUserWidget
{
	GENERATED_BODY()
	
private:
	//==== Hidden Fields ====
	
	uint8 Seconds = 0;
	uint8 Minutes = 0;
	uint16 Strikes = 0;
	
	FNumberFormattingOptions ClockFormattingOptions;
	FNumberFormattingOptions ParFormattingOptions;
	FNumberFormattingOptions StrikesFormattingOptions;
	
	//==== Widgets ====

	UPROPERTY(meta=(BindWidget))
	UTextBlock* SecondsDisplay;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MinutesDisplay;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ParDisplay;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* StrikesDisplay;

public:
	void ResetTimer();
	void IncrementSeconds();
	void SetPar(uint8 Amount) const;
	void ResetStrikes();
	void IncrementStrikes();

private:
	virtual void NativeConstruct() override;
};
