// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabyrinthProtocolGrenadeLauncherComponent.h"
#include "LabyrinthProtocolGrenadeProjectile.h"

ULabyrinthProtocolGrenadeLauncherComponent::ULabyrinthProtocolGrenadeLauncherComponent()
{
	WeaponDisplayName = FText::FromString( TEXT( "Grenade Launcher" ) );
	AmmoType = ELabyrinthProtocolAmmoType::Grenade;
	MaxAmmo = 1;
	CurrentAmmo = 1;
	ReserveAmmo = 5;
	MaxReserveAmmo = 8;
	ProjectileClass = ALabyrinthProtocolGrenadeProjectile::StaticClass();
	MuzzleOffset = FVector( 80.0f, 0.0f, 0.0f );
}
