#include "HeadsUpDisplay.h"

#include "ExoGolf/Actors/Pawns/Player/EGPlayer.h"
#include "Kismet/GameplayStatics.h"

void UHeadsUpDisplay::SetHUD(AEGHUD* Hud)
{
	HUD = Hud;
}

void UHeadsUpDisplay::SetUp()
{
	Player = Cast<AEGPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(Player)
		Player->SetHeadsUpDisplay(this);
}

void UHeadsUpDisplay::ShowPauseMenu()
{
}

void UHeadsUpDisplay::HidePauseMenu()
{
}

void UHeadsUpDisplay::ShowLevelSelector()
{
}

void UHeadsUpDisplay::ShowHelp()
{
}
