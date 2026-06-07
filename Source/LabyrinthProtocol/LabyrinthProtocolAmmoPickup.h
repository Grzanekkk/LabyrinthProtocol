// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LabyrinthProtocolAmmoTypes.h"
#include "LabyrinthProtocolPickupBase.h"
#include "LabyrinthProtocolAmmoPickup.generated.h"

UCLASS( Blueprintable )
class LABYRINTHPROTOCOL_API ALabyrinthProtocolAmmoPickup : public ALabyrinthProtocolPickupBase
{
	GENERATED_BODY()

public:
	ALabyrinthProtocolAmmoPickup();

	UFUNCTION( BlueprintPure, Category = "Pickup|Ammo" )
	ELabyrinthProtocolAmmoType GetAmmoType() const
	{
		return AmmoType;
	}

	UFUNCTION( BlueprintPure, Category = "Pickup|Ammo" )
	int32 GetAmmoAmount() const
	{
		return AmmoAmount;
	}

protected:
	UPROPERTY( EditDefaultsOnly, Category = "Pickup|Ammo" )
	ELabyrinthProtocolAmmoType AmmoType = ELabyrinthProtocolAmmoType::Rifle;

	UPROPERTY( EditDefaultsOnly, Category = "Pickup|Ammo", meta = ( ClampMin = "1", UIMin = "1" ) )
	int32 AmmoAmount = 15;

	virtual bool TryApplyPickup( ALabyrinthProtocolCharacter* Character ) override;
};
