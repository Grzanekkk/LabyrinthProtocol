// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LabyrinthProtocolHUDTypes.generated.h"

USTRUCT( BlueprintType )
struct FLabyrinthProtocolHUDViewData
{
	GENERATED_BODY()

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	int32 CurrentHealth = 0;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	int32 MaxHealth = 0;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	FText WeaponName;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	int32 MagazineAmmo = 0;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	int32 MaxMagazineAmmo = 0;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	int32 ReserveAmmo = 0;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	bool bHasWeapon = false;
};
