// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LabyrinthProtocolAmmoTypes.generated.h"

UENUM( BlueprintType )
enum class ELabyrinthProtocolAmmoType : uint8
{
	Rifle		UMETA( DisplayName = "Rifle" ),
	Grenade		UMETA( DisplayName = "Grenade" )
};
