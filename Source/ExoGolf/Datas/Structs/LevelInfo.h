#pragma once

#include "CoreMinimal.h"
#include "LevelInfo.generated.h"

USTRUCT()
struct FLevelInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName DisplayName;
	
	UPROPERTY(VisibleAnywhere)
	uint32 CurrentStrikesAmount;

	UPROPERTY(VisibleAnywhere)
	uint32 StrikesItTookToFinish;

	UPROPERTY(EditAnywhere)
	uint32 ParStrikeAmount;

	UPROPERTY(VisibleAnywhere)
	float PlayerTime;
};