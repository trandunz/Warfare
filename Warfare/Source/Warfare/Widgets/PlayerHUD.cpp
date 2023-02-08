#include "PlayerHUD.h"

#include "Components/TextBlock.h"

void UPlayerHUD::SetInteractText(FString _message)
{
	if (InteractText && _message != "")
	{
		InteractText->SetText(FText::FromString("Press E To " + _message));
	}
	else
	{
		InteractText->SetText(FText::FromString(""));
	}
}
