// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/LabyrinthProtocolHUDWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void ULabyrinthProtocolHUDWidget::ApplyHUDData( const FLabyrinthProtocolHUDViewData& HUDData )
{
	if( HealthText != nullptr )
	{
		HealthText->SetText( FText::FromString( FString::Printf( TEXT( "HP %d / %d" ), HUDData.CurrentHealth, HUDData.MaxHealth ) ) );
	}

	if( HealthBar != nullptr )
	{
		HealthBar->SetPercent( FMath::Clamp( HUDData.HealthPercent, 0.0f, 1.0f ) );
	}

	if( WeaponNameText != nullptr )
	{
		WeaponNameText->SetText( HUDData.bHasWeapon ? HUDData.WeaponName : FText::FromString( TEXT( "No Weapon" ) ) );
	}

	if( AmmoTypeText != nullptr )
	{
		if( HUDData.bHasWeapon )
		{
			AmmoTypeText->SetText( HUDData.AmmoTypeName );
			AmmoTypeText->SetVisibility( ESlateVisibility::Visible );
		}
		else
		{
			AmmoTypeText->SetVisibility( ESlateVisibility::Collapsed );
		}
	}

	if( MagazineAmmoText != nullptr )
	{
		MagazineAmmoText->SetText( HUDData.MagazineAmmoLabel );
	}

	if( ReserveAmmoText != nullptr )
	{
		ReserveAmmoText->SetText( HUDData.ReserveAmmoLabel );
	}

	if( AmmoText != nullptr )
	{
		if( HUDData.bHasWeapon )
		{
			const FString ReloadHint = HUDData.bCanReload ? TEXT( "  [R]" ) : TEXT( "" );
			AmmoText->SetText( FText::FromString( FString::Printf(
				TEXT( "Mag %d/%d  |  Res %d/%d%s" ),
				HUDData.MagazineAmmo,
				HUDData.MaxMagazineAmmo,
				HUDData.ReserveAmmo,
				HUDData.MaxReserveAmmo,
				*ReloadHint
			) ) );
		}
		else
		{
			AmmoText->SetText( FText::FromString( TEXT( "-- / --" ) ) );
		}
	}

	if( HUDData.HealthDelta < 0 )
	{
		OnDamageTaken( FMath::Abs( HUDData.HealthDelta ) );
	}

	OnHUDDataApplied( HUDData );
}
