// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/LabyrinthProtocolLootComponent.h"
#include "Components/HealthComponent.h"
#include "LabyrinthProtocolHealthPickup.h"
#include "LabyrinthProtocolAmmoPickup.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

ULabyrinthProtocolLootComponent::ULabyrinthProtocolLootComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	HealthPickupClass = ALabyrinthProtocolHealthPickup::StaticClass();
	AmmoPickupClasses.Add( ALabyrinthProtocolAmmoPickup::StaticClass() );
}

void ULabyrinthProtocolLootComponent::BeginPlay()
{
	Super::BeginPlay();

	if( AActor* const Owner = GetOwner() )
	{
		if( UHealthComponent* const HealthComponent = Owner->FindComponentByClass< UHealthComponent >() )
		{
			HealthComponent->OnDeath.AddDynamic( this, &ULabyrinthProtocolLootComponent::HandleOwnerDeath );
		}
	}
}

void ULabyrinthProtocolLootComponent::HandleOwnerDeath( UHealthComponent* const HealthComponent )
{
	RollAndSpawnLoot();
}

void ULabyrinthProtocolLootComponent::RollAndSpawnLoot()
{
	if( bHasDroppedLoot )
	{
		return;
	}

	bHasDroppedLoot = true;

	const ELabyrinthProtocolLootDropType DropType = RollLootDropType();
	switch( DropType )
	{
	case ELabyrinthProtocolLootDropType::Health:
		if( HealthPickupClass != nullptr )
		{
			SpawnPickupAtOwner( HealthPickupClass );
		}
		break;

	case ELabyrinthProtocolLootDropType::Ammo:
		if( TSubclassOf< ALabyrinthProtocolAmmoPickup > AmmoPickupClass = PickRandomAmmoPickupClass() )
		{
			SpawnPickupAtOwner( AmmoPickupClass );
		}
		break;

	default:
		break;
	}
}

ELabyrinthProtocolLootDropType ULabyrinthProtocolLootComponent::RollLootDropType() const
{
	const float ClampedHealthChance = FMath::Clamp( HealthDropChance, 0.0f, 1.0f );
	const float ClampedAmmoChance = HasValidAmmoPickupClasses()
		? FMath::Clamp( AmmoDropChance, 0.0f, 1.0f - ClampedHealthChance )
		: 0.0f;
	const float Roll = FMath::FRand();

	if( Roll < ClampedHealthChance )
	{
		return ELabyrinthProtocolLootDropType::Health;
	}

	if( Roll < ClampedHealthChance + ClampedAmmoChance )
	{
		return ELabyrinthProtocolLootDropType::Ammo;
	}

	return ELabyrinthProtocolLootDropType::None;
}

bool ULabyrinthProtocolLootComponent::HasValidAmmoPickupClasses() const
{
	for( const TSubclassOf< ALabyrinthProtocolAmmoPickup >& AmmoPickupClass : AmmoPickupClasses )
	{
		if( AmmoPickupClass != nullptr )
		{
			return true;
		}
	}

	return false;
}

TSubclassOf< ALabyrinthProtocolAmmoPickup > ULabyrinthProtocolLootComponent::PickRandomAmmoPickupClass() const
{
	TArray< TSubclassOf< ALabyrinthProtocolAmmoPickup > > ValidAmmoPickupClasses;
	ValidAmmoPickupClasses.Reserve( AmmoPickupClasses.Num() );

	for( const TSubclassOf< ALabyrinthProtocolAmmoPickup >& AmmoPickupClass : AmmoPickupClasses )
	{
		if( AmmoPickupClass != nullptr )
		{
			ValidAmmoPickupClasses.Add( AmmoPickupClass );
		}
	}

	if( ValidAmmoPickupClasses.Num() == 0 )
	{
		return nullptr;
	}

	const int32 RandomIndex = FMath::RandRange( 0, ValidAmmoPickupClasses.Num() - 1 );
	return ValidAmmoPickupClasses[ RandomIndex ];
}

float ULabyrinthProtocolLootComponent::GetNothingDropChance() const
{
	const float ClampedHealthChance = FMath::Clamp( HealthDropChance, 0.0f, 1.0f );
	const float ClampedAmmoChance = HasValidAmmoPickupClasses()
		? FMath::Clamp( AmmoDropChance, 0.0f, 1.0f - ClampedHealthChance )
		: 0.0f;
	return FMath::Clamp( 1.0f - ClampedHealthChance - ClampedAmmoChance, 0.0f, 1.0f );
}

void ULabyrinthProtocolLootComponent::SpawnPickupAtOwner( TSubclassOf< AActor > PickupClass )
{
	AActor* const Owner = GetOwner();
	UWorld* const World = GetWorld();
	if( Owner == nullptr || World == nullptr || PickupClass == nullptr )
	{
		return;
	}

	const FVector BaseLocation = Owner->GetActorLocation() + FVector( 0.0f, 0.0f, SpawnHeightOffset );
	const FVector RandomOffset = FVector(
		FMath::FRandRange( -SpawnRadius, SpawnRadius ),
		FMath::FRandRange( -SpawnRadius, SpawnRadius ),
		0.0f
	);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	World->SpawnActor< AActor >( PickupClass, BaseLocation + RandomOffset, FRotator::ZeroRotator, SpawnParams );
}
