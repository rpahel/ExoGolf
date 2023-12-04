// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelTitle.generated.h"

class UTextBlock;
class AEGHUD;
/**
 * 
 */
UCLASS()
class EXOGOLF_API ULevelTitle : public UUserWidget
{
	GENERATED_BODY()

private:
	//==== Widgets ====
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* LevelNameText;

public:
	void SetLevelName(const FName& Name) const;
};
