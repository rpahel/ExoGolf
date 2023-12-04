#pragma once

#include "CoreMinimal.h"
#include "LevelInfo.generated.h"

USTRUCT()
struct FLevelInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName DisplayName = FName("");
	
	UPROPERTY(VisibleAnywhere)
	uint32 CurrentStrikesAmount = 0;

	UPROPERTY(VisibleAnywhere)
	uint32 StrikesItTookToFinish = 0;

	UPROPERTY(EditAnywhere)
	uint8 ParStrikeAmount = 0;

	UPROPERTY(VisibleAnywhere)
	float PlayerTime = 0;
};