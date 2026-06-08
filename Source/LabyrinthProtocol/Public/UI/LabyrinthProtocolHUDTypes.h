// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LabyrinthProtocol/LabyrinthProtocolAmmoTypes.h"
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
	float HealthPercent = 0.0f;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	int32 HealthDelta = 0;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	FText WeaponName;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	ELabyrinthProtocolAmmoType AmmoType = ELabyrinthProtocolAmmoType::Rifle;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	FText AmmoTypeName;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	int32 MagazineAmmo = 0;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	int32 MaxMagazineAmmo = 0;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	int32 ReserveAmmo = 0;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	int32 MaxReserveAmmo = 0;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	bool bCanReload = false;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	bool bHasWeapon = false;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	FText MagazineAmmoLabel;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	FText ReserveAmmoLabel;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	bool bTookDamage = false;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	int32 DamageTakenAmount = 0;

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	int32 ActiveWeaponSlotIndex = INDEX_NONE;
};
