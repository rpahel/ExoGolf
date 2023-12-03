// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ExoGolf/Datas/Structs/LevelInfo.h"
#include "LevelsData.generated.h"

/**
 * 
 */
UCLASS()
class EXOGOLF_API ULevelsData : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere)
	TMap<FName, FLevelInfo> LevelsInGame;
};
