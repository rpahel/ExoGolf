// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EGMainMenuButton.generated.h"

enum class EMainMenuButtonAnimation;
class UButton;
class UTextBlock;

DECLARE_DELEGATE(FMainMenuButtonDelegate);
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

	UPROPERTY(meta=(BindWidget))
	UButton* Button;

	//==== Animations ====

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* ClickAnimation;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* RightEnterAnimation;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* LeftEnterAnimation;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* RightExitAnimation;

	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* LeftExitAnimation;

public:
	UWidgetAnimation* PlayButtonAnimation(const EMainMenuButtonAnimation& Animation);
	
private:
	//==== Overrides ====

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	//==== Event Handlers ====

	UFUNCTION()
	void ButtonClicked();
};