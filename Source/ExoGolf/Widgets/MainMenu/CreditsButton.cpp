// Fill out your copyright notice in the Description page of Project Settings.


#include "CreditsButton.h"

#include "Kismet/KismetSystemLibrary.h"

void UCreditsButton::SetUpButton()
{
	OnClicked.AddUniqueDynamic(this, &UCreditsButton::OpenUrl);
}

void UCreditsButton::OpenUrl()
{
	UKismetSystemLibrary::LaunchURL(UrlToLaunch);
}
