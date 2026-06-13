// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LabyrinthProtocolLootComponent.generated.h"

class UHealthComponent;
class ALabyrinthProtocolHealthPickup;
class ALabyrinthProtocolAmmoPickup;

UENUM( BlueprintType )
enum class ELabyrinthProtocolLootDropType : uint8
{
	None	UMETA( DisplayName = "Nothing" ),
	Health	UMETA( DisplayName = "Health Pickup" ),
	Ammo	UMETA( DisplayName = "Ammo Pickup" )
};

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class LABYRINTHPROTOCOL_API ULabyrinthProtocolLootComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULabyrinthProtocolLootComponent();

	UFUNCTION( BlueprintCallable, Category = "Loot" )
	void RollAndSpawnLoot();

	UFUNCTION( BlueprintPure, Category = "Loot" )
	float GetNothingDropChance() const;

protected:
	UPROPERTY( EditDefaultsOnly, Category = "Loot|Chances", meta = ( ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0" ) )
	float HealthDropChance = 0.35f;

	UPROPERTY( EditDefaultsOnly, Category = "Loot|Chances", meta = ( ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0" ) )
	float AmmoDropChance = 0.35f;

	UPROPERTY( EditDefaultsOnly, Category = "Loot|Pickup Classes" )
	TSubclassOf< ALabyrinthProtocolHealthPickup > HealthPickupClass;

	UPROPERTY( EditDefaultsOnly, Category = "Loot|Pickup Classes" )
	TArray< TSubclassOf< ALabyrinthProtocolAmmoPickup > > AmmoPickupClasses;

	UPROPERTY( EditDefaultsOnly, Category = "Loot|Spawn", meta = ( ClampMin = "0.0", UIMin = "0.0" ) )
	float SpawnHeightOffset = 12.0f;

	UPROPERTY( EditDefaultsOnly, Category = "Loot|Spawn", meta = ( ClampMin = "0.0", UIMin = "0.0" ) )
	float SpawnRadius = 35.0f;

	UPROPERTY( EditDefaultsOnly, Category = "Loot|Spawn" )
	bool bTraceToGround = true;

	UPROPERTY( EditDefaultsOnly, Category = "Loot|Spawn", meta = ( ClampMin = "0.0", UIMin = "0.0" ) )
	float GroundTraceDistance = 500.0f;

	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleOwnerDeath( UHealthComponent* const HealthComponent );

	ELabyrinthProtocolLootDropType RollLootDropType() const;
	TSubclassOf< ALabyrinthProtocolAmmoPickup > PickRandomAmmoPickupClass() const;
	bool HasValidAmmoPickupClasses() const;
	FVector GetPickupSpawnLocation( const FVector& OriginLocation ) const;
	void SpawnPickupAtOwner( TSubclassOf< AActor > PickupClass );

private:
	bool bHasDroppedLoot = false;
};
