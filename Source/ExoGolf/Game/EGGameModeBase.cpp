// Fill out your copyright notice in the Description page of Project Settings.


#include "EGGameModeBase.h"

#include "ExoGolf/Datas/Data_Assets/LevelsData.h"

APawn* AEGGameModeBase::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	if(LevelsData && LevelsData->CurrentLevelIndex == 0)
	{
		return nullptr;
	}
	
	return Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
}
