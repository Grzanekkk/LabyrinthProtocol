// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/WeaponInventoryComponent.h"
#include "LabyrinthProtocolCharacter.h"
#include "LabyrinthProtocolWeaponComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

UWeaponInventoryComponent::UWeaponInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	DiscoverWeaponSlots();

	if( WeaponSlots.Num() > 0 )
	{
		EquipWeaponBySlot( 0, false );
	}
}

void UWeaponInventoryComponent::DiscoverWeaponSlots()
{
	WeaponSlots.Reset();

	AActor* const Owner = GetOwner();
	ALabyrinthProtocolCharacter* const Character = Cast< ALabyrinthProtocolCharacter >( Owner );
	if( Owner == nullptr || Character == nullptr )
	{
		return;
	}

	TArray< ULabyrinthProtocolWeaponComponent* > FoundWeapons;
	Owner->GetComponents< ULabyrinthProtocolWeaponComponent >( FoundWeapons );

	for( ULabyrinthProtocolWeaponComponent* Weapon : FoundWeapons )
	{
		if( Weapon == nullptr )
		{
			continue;
		}

		Weapon->InitializeWeapon( Character );
		Weapon->SetWeaponEquipped( false, false );
		WeaponSlots.Add( Weapon );

		if( WeaponSlots.Num() >= MaxWeaponSlots )
		{
			break;
		}
	}
}

bool UWeaponInventoryComponent::EquipWeaponBySlot( int32 SlotIndex, bool bPlaySwitchSound )
{
	if( !WeaponSlots.IsValidIndex( SlotIndex ) )
	{
		return false;
	}

	ULabyrinthProtocolWeaponComponent* const NewWeapon = WeaponSlots[ SlotIndex ];
	if( NewWeapon == nullptr || NewWeapon == ActiveWeapon )
	{
		return false;
	}

	if( ActiveWeapon != nullptr )
	{
		ActiveWeapon->SetWeaponEquipped( false, false );
	}

	ActiveWeapon = NewWeapon;
	ActiveWeaponSlotIndex = SlotIndex;
	ActiveWeapon->SetWeaponEquipped( true, true );

	ALabyrinthProtocolCharacter* const Character = Cast< ALabyrinthProtocolCharacter >( GetOwner() );
	if( Character != nullptr )
	{
		Character->RegisterTrackedWeapon( ActiveWeapon );
		Character->RefreshActiveWeaponInput();
	}

	if( bPlaySwitchSound )
	{
		PlayWeaponSwitchSound();
	}

	OnActiveWeaponChanged.Broadcast( ActiveWeapon, ActiveWeaponSlotIndex );
	return true;
}

bool UWeaponInventoryComponent::EquipWeaponSlot1()
{
	return EquipWeaponBySlot( 0, true );
}

bool UWeaponInventoryComponent::EquipWeaponSlot2()
{
	return EquipWeaponBySlot( 1, true );
}

void UWeaponInventoryComponent::PlayWeaponSwitchSound() const
{
	if( WeaponSwitchSound == nullptr )
	{
		return;
	}

	AActor* const Owner = GetOwner();
	if( Owner == nullptr )
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation( Owner, WeaponSwitchSound, Owner->GetActorLocation(), WeaponSwitchSoundVolume );
}
