// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/LabyrinthProtocolHUDTypes.h"
#include "LabyrinthProtocolHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;

UCLASS()
class LABYRINTHPROTOCOL_API ULabyrinthProtocolHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintCallable, Category = "HUD" )
	void ApplyHUDData( const FLabyrinthProtocolHUDViewData& HUDData );

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = ( BindWidget ) )
	TObjectPtr< UTextBlock > CurrentHealthWidget;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	TObjectPtr< UTextBlock > MaxHealthWidget;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	TObjectPtr< UTextBlock > CurrentAmmoWidget;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	TObjectPtr< UTextBlock > MaxAmmoWidget;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	TObjectPtr< UProgressBar > HealthBar;

	UPROPERTY( BlueprintReadWrite, meta = ( BindWidget ) )
	TObjectPtr< UImage > DamageOverlay;

	UPROPERTY( EditDefaultsOnly, Category = "HUD|Damage", meta = ( ClampMin = "0.0", UIMin = "0.0" ) )
	float DamageOverlayMaxAlpha = 0.45f;

	UPROPERTY( EditDefaultsOnly, Category = "HUD|Damage", meta = ( ClampMin = "0.1", UIMin = "0.1" ) )
	float DamageOverlayFadeSpeed = 2.5f;

	UPROPERTY( EditDefaultsOnly, Category = "HUD|Damage", meta = ( ClampMin = "1.0", UIMin = "1.0" ) )
	float DamageOverlayReferenceDamage = 30.0f;

	void UpdateHealthBarVisuals( float HealthPercent );
	void PlayDamageOverlay( int32 DamageAmount );
	void UpdateDamageOverlayVisual() const;
	void TickDamageOverlayFade();
	void SetCounterText( UTextBlock* TextWidget, int32 Value, int32 FallbackValue = 0 ) const;

	UFUNCTION( BlueprintImplementableEvent, Category = "HUD" )
	void OnHUDDataApplied( const FLabyrinthProtocolHUDViewData& HUDData );

	UFUNCTION( BlueprintImplementableEvent, Category = "HUD" )
	void OnDamageTaken( int32 DamageAmount );

private:
	float DamageOverlayAlpha = 0.0f;

	FTimerHandle DamageOverlayFadeTimerHandle;
};
