// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EGMainMenuButton.generated.h"

class UTextBlock;

DECLARE_DYNAMIC_DELEGATE(FMainMenuButtonDelegate);
/**
 * 
 */
UCLASS()
class EXOGOLF_API UEGMainMenuButton : public UUserWidget
{
	GENERATED_BODY()

public:
	FMainMenuButtonDelegate OnClickedDelegate;
	
private:
	//==== Widgets =====
	
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category = "ButtonText")
	UTextBlock* ButtonText;

private:
	//==== Overrides ====

	virtual void NativeDestruct() override;
};