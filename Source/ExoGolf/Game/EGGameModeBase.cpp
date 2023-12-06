// Fill out your copyright notice in the Description page of Project Settings.


#include "EGGameModeBase.h"

#include "ExoGolf/Datas/Data_Assets/LevelsData.h"
#include "Kismet/GameplayStatics.h"

APawn* AEGGameModeBase::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	if(!LevelsData)
		return nullptr;
	
	const FName CurrentLevel = FName(UGameplayStatics::GetCurrentLevelName(GetWorld()));
	TArray<FName> Keys;
	if(LevelsData->LevelsInGame.GetKeys(Keys) <= 0)
		return nullptr;
	
	if(CurrentLevel == Keys[0])
	{
		return nullptr;
	}
	
	return Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);
}
