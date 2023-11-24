// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Scalable.generated.h"

UINTERFACE(MinimalAPI)
class UScalable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EXOGOLF_API IScalable
{
	GENERATED_BODY()

public:
	virtual void UniformAddScale(float Amount) = 0;
	virtual void UniformMultiplyScale(float Amount) = 0;
	virtual void NonUniformAddScale(float X, float Y, float Z) = 0;
	virtual void NonUniformMultiplyScale(float X, float Y, float Z) = 0;
};
