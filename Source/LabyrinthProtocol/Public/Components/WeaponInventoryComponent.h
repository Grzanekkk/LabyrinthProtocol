// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponInventoryComponent.generated.h"

class ALabyrinthProtocolCharacter;
class ULabyrinthProtocolWeaponComponent;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnActiveWeaponChanged, ULabyrinthProtocolWeaponComponent*, NewWeapon, int32, WeaponSlotIndex );

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class LABYRINTHPROTOCOL_API UWeaponInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponInventoryComponent();

	UFUNCTION( BlueprintCallable, Category = "Weapon|Inventory" )
	bool EquipWeaponBySlot( int32 SlotIndex, bool bPlaySwitchSound = true );

	UFUNCTION( BlueprintCallable, Category = "Weapon|Inventory" )
	bool EquipWeaponSlot1();

	UFUNCTION( BlueprintCallable, Category = "Weapon|Inventory" )
	bool EquipWeaponSlot2();

	UFUNCTION( BlueprintPure, Category = "Weapon|Inventory" )
	ULabyrinthProtocolWeaponComponent* GetActiveWeapon() const
	{
		return ActiveWeapon;
	}

	UFUNCTION( BlueprintPure, Category = "Weapon|Inventory" )
	int32 GetActiveWeaponSlotIndex() const
	{
		return ActiveWeaponSlotIndex;
	}

	UFUNCTION( BlueprintPure, Category = "Weapon|Inventory" )
	int32 GetWeaponSlotCount() const
	{
		return WeaponSlots.Num();
	}

	UPROPERTY( Category = "Weapon|Inventory", BlueprintAssignable )
	FOnActiveWeaponChanged OnActiveWeaponChanged;

protected:
	virtual void BeginPlay() override;

	UPROPERTY( EditDefaultsOnly, Category = "Weapon|Inventory", meta = ( ClampMin = "1", UIMin = "1" ) )
	int32 MaxWeaponSlots = 2;

	UPROPERTY( EditDefaultsOnly, Category = "Weapon|Inventory|Audio" )
	TObjectPtr< USoundBase > WeaponSwitchSound;

	UPROPERTY( EditDefaultsOnly, Category = "Weapon|Inventory|Audio", meta = ( ClampMin = "0.0", UIMin = "0.0" ) )
	float WeaponSwitchSoundVolume = 1.0f;

	void DiscoverWeaponSlots();
	void PlayWeaponSwitchSound() const;

private:
	UPROPERTY()
	TArray< TObjectPtr< ULabyrinthProtocolWeaponComponent > > WeaponSlots;

	UPROPERTY()
	TObjectPtr< ULabyrinthProtocolWeaponComponent > ActiveWeapon;

	int32 ActiveWeaponSlotIndex = INDEX_NONE;
};
