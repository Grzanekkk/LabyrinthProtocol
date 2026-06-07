// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/LabyrinthProtocolHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void ULabyrinthProtocolHUDWidget::ApplyHUDData( const FLabyrinthProtocolHUDViewData& HUDData )
{
	if( HealthText != nullptr )
	{
		HealthText->SetText( FText::FromString( FString::Printf( TEXT( "%d / %d" ), HUDData.CurrentHealth, HUDData.MaxHealth ) ) );
	}

	if( HealthBar != nullptr )
	{
		const float HealthPercent = HUDData.MaxHealth > 0
			? static_cast< float >( HUDData.CurrentHealth ) / static_cast< float >( HUDData.MaxHealth )
			: 0.0f;
		HealthBar->SetPercent( HealthPercent );
	}

	if( WeaponNameText != nullptr )
	{
		WeaponNameText->SetText( HUDData.bHasWeapon ? HUDData.WeaponName : FText::FromString( TEXT( "No Weapon" ) ) );
	}

	if( AmmoText != nullptr )
	{
		if( HUDData.bHasWeapon )
		{
			AmmoText->SetText( FText::FromString( FString::Printf(
				TEXT( "%d / %d  |  %d" ),
				HUDData.MagazineAmmo,
				HUDData.MaxMagazineAmmo,
				HUDData.ReserveAmmo
			) ) );
		}
		else
		{
			AmmoText->SetText( FText::FromString( TEXT( "-- / --" ) ) );
		}
	}

	OnHUDDataApplied( HUDData );
}
