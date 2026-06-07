// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/LabyrinthProtocolHUDTypes.h"
#include "LabyrinthProtocolHUDWidget.generated.h"

class UTextBlock;
class UProgressBar;

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
	TObjectPtr< UTextBlock > AmmoText;

	UFUNCTION( BlueprintImplementableEvent, Category = "HUD" )
	void OnHUDDataApplied( const FLabyrinthProtocolHUDViewData& HUDData );
};
