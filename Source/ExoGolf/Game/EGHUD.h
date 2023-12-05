// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EGHUD.generated.h"

enum class EMenu;
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
	TSubclassOf<UUserWidget> W_LevelsMenu;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> W_HelpMenu;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> W_HUD;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> W_End;

	//==== Hidden fields ====

	UPROPERTY()
	UUserWidget* CurrentWidget;

public:
	void LoadLevel(FName LevelName) const;
	void LoadMenu(const EMenu Menu);

private:
	virtual void BeginPlay() override;
	void KillCurrentWidget();
	void SetHUDInCurrentWidget();
};

UENUM()
enum class EMenu
{
	MainMenu,
	LevelsMenu,
	HelpMenu
};