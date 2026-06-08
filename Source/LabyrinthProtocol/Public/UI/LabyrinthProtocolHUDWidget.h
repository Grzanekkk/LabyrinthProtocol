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
	UPROPERTY( meta = ( BindWidgetOptional ) )
	TObjectPtr< UTextBlock > HealthText;

	UPROPERTY( meta = ( BindWidgetOptional ) )
	TObjectPtr< UProgressBar > HealthBar;

	UPROPERTY( meta = ( BindWidgetOptional ) )
	TObjectPtr< UTextBlock > WeaponNameText;

	UPROPERTY( meta = ( BindWidgetOptional ) )
	TObjectPtr< UTextBlock > AmmoTypeText;

	UPROPERTY( meta = ( BindWidgetOptional ) )
	TObjectPtr< UTextBlock > AmmoText;

	UPROPERTY( meta = ( BindWidgetOptional ) )
	TObjectPtr< UTextBlock > MagazineAmmoText;

	UPROPERTY( meta = ( BindWidgetOptional ) )
	TObjectPtr< UTextBlock > ReserveAmmoText;

	UPROPERTY( meta = ( BindWidgetOptional ) )
	TObjectPtr< UImage > CrosshairImage;

	UPROPERTY( meta = ( BindWidgetOptional ) )
	TObjectPtr< UImage > DamageOverlay;

	UFUNCTION( BlueprintImplementableEvent, Category = "HUD" )
	void OnHUDDataApplied( const FLabyrinthProtocolHUDViewData& HUDData );

	UFUNCTION( BlueprintImplementableEvent, Category = "HUD" )
	void OnDamageTaken( int32 DamageAmount );
};
