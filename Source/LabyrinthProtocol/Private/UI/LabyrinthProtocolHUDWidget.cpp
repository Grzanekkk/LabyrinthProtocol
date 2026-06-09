// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/LabyrinthProtocolHUDWidget.h"
#include "LabyrinthProtocol/LabyrinthProtocolCharacter.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

namespace
{
	UWorld* GetWidgetWorld( const UUserWidget* Widget )
	{
		if( Widget == nullptr )
		{
			return nullptr;
		}

		if( UWorld* World = Widget->GetWorld() )
		{
			return World;
		}

		if( APlayerController* PlayerController = Widget->GetOwningPlayer() )
		{
			return PlayerController->GetWorld();
		}

		return nullptr;
	}
}

void ULabyrinthProtocolHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DamageOverlayAlpha = 0.0f;
	UpdateDamageOverlayVisual();
	BindToPlayerCharacter();
}

void ULabyrinthProtocolHUDWidget::NativeDestruct()
{
	UnbindFromCharacter();

	if( UWorld* World = GetWidgetWorld( this ) )
	{
		World->GetTimerManager().ClearTimer( CharacterBindingRetryTimerHandle );
		World->GetTimerManager().ClearTimer( DamageOverlayFadeTimerHandle );
	}

	Super::NativeDestruct();
}

void ULabyrinthProtocolHUDWidget::BindToPlayerCharacter()
{
	UnbindFromCharacter();

	APlayerController* const PlayerController = GetOwningPlayer();
	ALabyrinthProtocolCharacter* const Character = PlayerController != nullptr
		? Cast< ALabyrinthProtocolCharacter >( PlayerController->GetPawn() )
		: nullptr;

	if( Character == nullptr )
	{
		if( UWorld* World = GetWidgetWorld( this ) )
		{
			World->GetTimerManager().SetTimer(
				CharacterBindingRetryTimerHandle,
				this,
				&ULabyrinthProtocolHUDWidget::BindToPlayerCharacter,
				0.1f,
				false
			);
		}
		return;
	}

	BoundCharacter = Character;
	BoundCharacter->OnHUDDataChanged.AddDynamic( this, &ULabyrinthProtocolHUDWidget::HandleHUDDataChanged );
	BoundCharacter->InitializeHUDBindings();
	ApplyHUDData( BoundCharacter->BuildHUDViewData() );
}

void ULabyrinthProtocolHUDWidget::UnbindFromCharacter()
{
	if( BoundCharacter != nullptr )
	{
		BoundCharacter->OnHUDDataChanged.RemoveDynamic( this, &ULabyrinthProtocolHUDWidget::HandleHUDDataChanged );
		BoundCharacter = nullptr;
	}
}

void ULabyrinthProtocolHUDWidget::HandleHUDDataChanged( FLabyrinthProtocolHUDViewData HUDData )
{
	ApplyHUDData( HUDData );
}

void ULabyrinthProtocolHUDWidget::TickDamageOverlayFade()
{
	UWorld* const World = GetWidgetWorld( this );
	if( World == nullptr )
	{
		return;
	}

	const float DeltaTime = World->GetDeltaSeconds();
	DamageOverlayAlpha = FMath::Max( 0.0f, DamageOverlayAlpha - ( DamageOverlayFadeSpeed * DeltaTime ) );
	UpdateDamageOverlayVisual();

	if( DamageOverlayAlpha <= KINDA_SMALL_NUMBER )
	{
		World->GetTimerManager().ClearTimer( DamageOverlayFadeTimerHandle );
	}
}

void ULabyrinthProtocolHUDWidget::ApplyHUDData( const FLabyrinthProtocolHUDViewData& HUDData )
{
	SetCounterText( CurrentHealthWidget, HUDData.CurrentHealth );
	SetCounterText( MaxHealthWidget, HUDData.MaxHealth );

	const float ClampedHealthPercent = FMath::Clamp( HUDData.HealthPercent, 0.0f, 1.0f );
	HealthBar->SetPercent( ClampedHealthPercent );
	UpdateHealthBarVisuals( ClampedHealthPercent );

	if( HUDData.bHasWeapon )
	{
		SetCounterText( CurrentAmmoWidget, HUDData.MagazineAmmo );
		SetCounterText( MaxAmmoWidget, HUDData.MaxMagazineAmmo );

		if( ReserveAmmoWidget != nullptr )
		{
			if( HUDData.MaxReserveAmmo > 0 )
			{
				ReserveAmmoWidget->SetText( FText::FromString( FString::Printf(
					TEXT( "Res: %d / %d" ),
					HUDData.ReserveAmmo,
					HUDData.MaxReserveAmmo
				) ) );
			}
			else
			{
				ReserveAmmoWidget->SetText( FText::FromString( FString::Printf(
					TEXT( "Res: %d" ),
					HUDData.ReserveAmmo
				) ) );
			}

			ReserveAmmoWidget->SetVisibility( ESlateVisibility::Visible );
		}
	}
	else
	{
		SetCounterText( CurrentAmmoWidget, 0 );
		SetCounterText( MaxAmmoWidget, 0 );

		if( ReserveAmmoWidget != nullptr )
		{
			ReserveAmmoWidget->SetText( FText::FromString( TEXT( "Res: 0" ) ) );
		}
	}

	if( HUDData.bTookDamage && HUDData.DamageTakenAmount > 0 )
	{
		PlayDamageOverlay( HUDData.DamageTakenAmount );
		OnDamageTaken( HUDData.DamageTakenAmount );
	}
	else if( HUDData.HealthDelta < 0 )
	{
		const int32 DamageAmount = FMath::Abs( HUDData.HealthDelta );
		PlayDamageOverlay( DamageAmount );
		OnDamageTaken( DamageAmount );
	}

	OnHUDDataApplied( HUDData );
}

void ULabyrinthProtocolHUDWidget::SetCounterText( UTextBlock* TextWidget, int32 Value, int32 FallbackValue ) const
{
	if( TextWidget == nullptr )
	{
		return;
	}

	TextWidget->SetText( FText::AsNumber( Value >= 0 ? Value : FallbackValue ) );
}

void ULabyrinthProtocolHUDWidget::UpdateHealthBarVisuals( float HealthPercent )
{
	const FLinearColor FullHealthColor( 0.1f, 0.85f, 0.2f, 1.0f );
	const FLinearColor LowHealthColor( 0.9f, 0.1f, 0.05f, 1.0f );
	const FLinearColor HealthBarColor = FMath::Lerp( LowHealthColor, FullHealthColor, HealthPercent );

	HealthBar->SetFillColorAndOpacity( HealthBarColor );
}

void ULabyrinthProtocolHUDWidget::PlayDamageOverlay( int32 DamageAmount )
{
	if( DamageOverlay == nullptr )
	{
		return;
	}

	const float DamageIntensity = FMath::Clamp(
		static_cast< float >( DamageAmount ) / DamageOverlayReferenceDamage,
		0.25f,
		1.0f
	);

	DamageOverlayAlpha = FMath::Max( DamageOverlayAlpha, DamageOverlayMaxAlpha * DamageIntensity );
	UpdateDamageOverlayVisual();

	UWorld* const World = GetWidgetWorld( this );
	if( World != nullptr )
	{
		World->GetTimerManager().SetTimer(
			DamageOverlayFadeTimerHandle,
			this,
			&ULabyrinthProtocolHUDWidget::TickDamageOverlayFade,
			0.016f,
			true
		);
	}
}

void ULabyrinthProtocolHUDWidget::UpdateDamageOverlayVisual() const
{
	if( DamageOverlay == nullptr )
	{
		return;
	}

	if( DamageOverlayAlpha <= KINDA_SMALL_NUMBER )
	{
		DamageOverlay->SetVisibility( ESlateVisibility::Collapsed );
		DamageOverlay->SetColorAndOpacity( FLinearColor( 0.85f, 0.0f, 0.0f, 0.0f ) );
		return;
	}

	DamageOverlay->SetVisibility( ESlateVisibility::HitTestInvisible );
	DamageOverlay->SetColorAndOpacity( FLinearColor( 0.85f, 0.0f, 0.0f, DamageOverlayAlpha ) );
}
