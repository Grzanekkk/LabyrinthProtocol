// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabyrinthProtocolGameMode.h"
#include "LabyrinthProtocolCharacter.h"
#include "LabyrinthProtocolPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ALabyrinthProtocolGameMode::ALabyrinthProtocolGameMode()
	: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	PlayerControllerClass = ALabyrinthProtocolPlayerController::StaticClass();
}
