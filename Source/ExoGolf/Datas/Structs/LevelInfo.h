#pragma once

#include "CoreMinimal.h"
#include "LevelInfo.generated.h"

USTRUCT()
struct FLevelInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	uint32 CurrentStrikesAmount;

	UPROPERTY(EditAnywhere)
	uint32 ParStrikeAmount;

	UPROPERTY(VisibleAnywhere)
	float PlayerTime;
};