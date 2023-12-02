// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EGHUD.generated.h"

class ULevelsData;
/**
 * 
 */
UCLASS()
class EXOGOLF_API AEGHUD : public AHUD
{
	GENERATED_BODY()

private:
	//==== Exposed fields ====

	UPROPERTY(EditAnywhere)
	ULevelsData* LevelsData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> W_MainMenu;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> W_HUD;

	//==== Hidden fields ====
	
	UPROPERTY()
	UUserWidget* MainMenu;

	UPROPERTY()
	UUserWidget* HUD;

public:
	void LoadNextLevel() const;
	void LoadLevelByIndex(int32 LevelIndex) const;
	void LoadLevelByName(FName LevelName) const;

private:
	virtual void BeginPlay() override;
};
