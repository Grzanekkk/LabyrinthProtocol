// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LabyrinthProtocolWeaponComponent.h"
#include "LabyrinthProtocolGrenadeLauncherComponent.generated.h"

UCLASS( Blueprintable, BlueprintType, ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class LABYRINTHPROTOCOL_API ULabyrinthProtocolGrenadeLauncherComponent : public ULabyrinthProtocolWeaponComponent
{
	GENERATED_BODY()

public:
	ULabyrinthProtocolGrenadeLauncherComponent();
};
