// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LabyrinthProtocolPlayerController.generated.h"

class ULabyrinthProtocolHUDWidget;

UCLASS( Blueprintable )
class LABYRINTHPROTOCOL_API ALabyrinthProtocolPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALabyrinthProtocolPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess( APawn* InPawn ) override;
	virtual void OnUnPossess() override;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "HUD" )
	TSubclassOf< ULabyrinthProtocolHUDWidget > HUDWidgetClass;

	void CreateHUDWidget();

	UPROPERTY( BlueprintReadOnly, Category = "HUD" )
	TObjectPtr< ULabyrinthProtocolHUDWidget > HUDWidget;
};
