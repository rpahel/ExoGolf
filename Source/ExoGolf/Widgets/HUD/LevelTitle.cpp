#include "LevelTitle.h"

#include "Components/TextBlock.h"

void ULevelTitle::SetLevelName(const FName& Name) const
{
	if(LevelNameText)
		LevelNameText->SetText(FText::FromName(Name));
}
