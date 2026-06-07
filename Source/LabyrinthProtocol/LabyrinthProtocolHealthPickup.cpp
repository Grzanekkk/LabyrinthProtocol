// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabyrinthProtocolHealthPickup.h"
#include "LabyrinthProtocolCharacter.h"
#include "Components/HealthComponent.h"

ALabyrinthProtocolHealthPickup::ALabyrinthProtocolHealthPickup()
{
}

bool ALabyrinthProtocolHealthPickup::TryApplyPickup( ALabyrinthProtocolCharacter* Character )
{
	if( Character == nullptr || HealAmount <= 0 )
	{
		return false;
	}

	UHealthComponent* const HealthComponent = Character->GetHealthComponent();
	if( HealthComponent == nullptr )
	{
		return false;
	}

	if( HealthComponent->GetCurrentHealth() >= HealthComponent->GetMaxHealth() )
	{
		return false;
	}

	HealthComponent->AddHealth( HealAmount );
	return true;
}
