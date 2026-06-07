// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LabyrinthProtocolPickupBase.h"
#include "LabyrinthProtocolHealthPickup.generated.h"

UCLASS( Blueprintable )
class LABYRINTHPROTOCOL_API ALabyrinthProtocolHealthPickup : public ALabyrinthProtocolPickupBase
{
	GENERATED_BODY()

public:
	ALabyrinthProtocolHealthPickup();

protected:
	UPROPERTY( EditDefaultsOnly, Category = "Pickup|Health", meta = ( ClampMin = "1", UIMin = "1" ) )
	int32 HealAmount = 25;

	virtual bool TryApplyPickup( ALabyrinthProtocolCharacter* Character ) override;
};
