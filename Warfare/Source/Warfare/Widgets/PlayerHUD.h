#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

UCLASS()
class WARFARE_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void SetInteractText(FString _message = "");
	
protected:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	class UTextBlock* InteractText;
};
