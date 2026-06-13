// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/LabyrinthProtocolHUDTypes.h"
#include "LabyrinthProtocolHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;
class ALabyrinthProtocolCharacter;

UCLASS( Blueprintable )
class LABYRINTHPROTOCOL_API ULabyrinthProtocolHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintCallable, Category = "HUD" )
	void ApplyHUDData( const FLabyrinthProtocolHUDViewData& HUDData );

	UFUNCTION( BlueprintCallable, Category = "HUD" )
	void BindToPlayerCharacter();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	TObjectPtr< UTextBlock > CurrentHealthWidget;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	TObjectPtr< UTextBlock > MaxHealthWidget;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	TObjectPtr< UTextBlock > CurrentAmmoWidget;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	TObjectPtr< UTextBlock > MaxAmmoWidget;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidgetOptional ) )
	TObjectPtr< UTextBlock > ReserveAmmoWidget;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	TObjectPtr< UProgressBar > HealthBar;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidgetOptional ) )
	TObjectPtr< UImage > DamageOverlay;

	UPROPERTY( EditDefaultsOnly, Category = "HUD|Damage", meta = ( ClampMin = "0.0", UIMin = "0.0" ) )
	float DamageOverlayMaxOpacity = 0.5f;

	UPROPERTY( EditDefaultsOnly, Category = "HUD|Damage", meta = ( ClampMin = "0.1", UIMin = "0.1" ) )
	float DamageOverlayFadeInSpeed = 4.0f;

	UPROPERTY( EditDefaultsOnly, Category = "HUD|Damage", meta = ( ClampMin = "0.1", UIMin = "0.1" ) )
	float DamageOverlayFadeOutSpeed = 2.0f;

	UPROPERTY( EditDefaultsOnly, Category = "HUD|Damage", meta = ( ClampMin = "1.0", UIMin = "1.0" ) )
	float DamageOverlayReferenceDamage = 30.0f;

	void UpdateHealthBarVisuals( float HealthPercent );
	void PlayDamageOverlay( int32 DamageAmount );
	void TickDamageOverlay();
	void UpdateDamageOverlayOpacity( float Opacity ) const;
	void HideDamageOverlay() const;
	void SetCounterText( UTextBlock* TextWidget, int32 Value, int32 FallbackValue = 0 ) const;
	void UnbindFromCharacter();

	UFUNCTION()
	void HandleHUDDataChanged( FLabyrinthProtocolHUDViewData HUDData );

	UFUNCTION( BlueprintImplementableEvent, Category = "HUD" )
	void OnHUDDataApplied( const FLabyrinthProtocolHUDViewData& HUDData );

	UFUNCTION( BlueprintImplementableEvent, Category = "HUD" )
	void OnDamageTaken( int32 DamageAmount );

private:
	UPROPERTY()
	TObjectPtr< ALabyrinthProtocolCharacter > BoundCharacter;

	float DamageOverlayOpacity = 0.0f;
	float DamageOverlayTargetOpacity = 0.0f;
	bool bDamageOverlayFadingIn = false;
	bool bDamageOverlayFadingOut = false;

	FTimerHandle CharacterBindingRetryTimerHandle;

	FTimerHandle DamageOverlayFadeTimerHandle;
};
