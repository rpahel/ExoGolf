// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuButton.generated.h"

enum class EMainMenuButtonAnimation;
class UButton;
class UTextBlock;

DECLARE_DELEGATE(FMainMenuButtonDelegate);
DECLARE_DELEGATE_OneParam(FMainMenuButtonWithRefDelegate, class UMainMenuButton*);
/**
 * 
 */
UCLASS()
class EXOGOLF_API UMainMenuButton : public UUserWidget
{
	GENERATED_BODY()

public:
	FMainMenuButtonDelegate OnClickedDelegate;
	FMainMenuButtonDelegate OnClickAnimationFinishedDelegate;

	FMainMenuButtonWithRefDelegate OnClickedRefDelegate;
	FMainMenuButtonWithRefDelegate OnClickAnimationFinishedRefDelegate;
	
private:
	//==== Exposed Fields ====
	
	UPROPERTY(EditAnywhere)
	FText ButtonText;
	
	//==== Hidden Fields ====

	int32 ButtonIndex;
	
	//==== Widgets =====
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ButtonTextBlock;

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
	void SetButtonRenderOpacity(float Opacity) const;
	void SetButtonText(const FText& Text);
	void SetButtonIndex(const int32 Index);
	int32 GetButtonIndex() const;
	UUMGSequencePlayer* PlayButtonAnimation(const EMainMenuButtonAnimation& Animation);
	
private:
	//==== Overrides ====

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	//==== Event Handlers ====

	UFUNCTION()
	void ButtonClicked();
};

UENUM()
enum class EMainMenuButtonAnimation
{
	Click,
	RightEnter,
	LeftEnter,
	RightExit,
	LeftExit
};