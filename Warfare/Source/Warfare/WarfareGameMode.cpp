// Copyright Epic Games, Inc. All Rights Reserved.

#include "WarfareGameMode.h"
#include "WarfareCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWarfareGameMode::AWarfareGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_WarfareCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
