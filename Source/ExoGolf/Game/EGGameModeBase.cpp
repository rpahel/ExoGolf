// Fill out your copyright notice in the Description page of Project Settings.


#include "EGGameModeBase.h"

#include "ExoGolf/Datas/Data_Assets/LevelsData.h"
#include "Kismet/GameplayStatics.h"

APawn* AEGGameModeBase::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	if(!LevelsData)
		return nullptr;
	
	const FName CurrentLevel = FName(UGameplayStatics::GetCurrentLevelName(GetWorld()));
	const FName FirstLevel = LevelsData->LevelsInGame[0];
	
	if(CurrentLevel == FirstLevel)
	{
		return nullptr;
	}
	
	return Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
}
