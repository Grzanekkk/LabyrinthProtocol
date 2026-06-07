// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LabyrinthProtocolPlayerController.generated.h"

class ULabyrinthProtocolHUDWidget;
class ALabyrinthProtocolCharacter;

UCLASS()
class LABYRINTHPROTOCOL_API ALabyrinthProtocolPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALabyrinthProtocolPlayerController();

protected:
	virtual void OnPossess( APawn* InPawn ) override;
	virtual void OnUnPossess() override;

	UPROPERTY( EditDefaultsOnly, Category = "HUD" )
	TSubclassOf< ULabyrinthProtocolHUDWidget > HUDWidgetClass;

private:
	UFUNCTION()
	void HandleHUDDataChanged( FLabyrinthProtocolHUDViewData HUDData );

	void BindToCharacter( ALabyrinthProtocolCharacter* Character );
	void UnbindFromCharacter();

	UPROPERTY()
	TObjectPtr< ULabyrinthProtocolHUDWidget > HUDWidget;

	UPROPERTY()
	TObjectPtr< ALabyrinthProtocolCharacter > BoundCharacter;
};
