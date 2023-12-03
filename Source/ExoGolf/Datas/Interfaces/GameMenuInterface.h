// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameMenuInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UGameMenuInterface : public UInterface
{
	GENERATED_BODY()
};

class IGameMenuInterface
{
	GENERATED_BODY()

public:
	virtual void SetHUD(class AEGHUD* Hud) = 0;
};
