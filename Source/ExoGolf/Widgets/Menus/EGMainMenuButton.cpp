// Fill out your copyright notice in the Description page of Project Settings.


#include "EGMainMenuButton.h"

void UEGMainMenuButton::NativeDestruct()
{
	Super::NativeDestruct();

	OnClickedDelegate.Clear();
}
