// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabyrinthProtocolAmmoPickup.h"
#include "LabyrinthProtocolCharacter.h"

ALabyrinthProtocolAmmoPickup::ALabyrinthProtocolAmmoPickup()
{
}

bool ALabyrinthProtocolAmmoPickup::TryApplyPickup( ALabyrinthProtocolCharacter* Character )
{
	if( Character == nullptr || AmmoAmount <= 0 )
	{
		return false;
	}

	return Character->TryAddReserveAmmo( AmmoType, AmmoAmount );
}
