// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/HealthComponent.h"

void UHealthComponent::AddHealth( int32 const DeltaHealth )
{
	if( DeltaHealth != 0 )
	{
		CurrentHealth = FMath::Clamp( CurrentHealth + DeltaHealth, 0, MaxHealth );

		if( DeltaHealth > 0 )
		{
			// Heal
			OnHealed.Broadcast( this, CurrentHealth, DeltaHealth );
		}
		else
		{
			// Damaged
			OnDamaged.Broadcast( this, CurrentHealth, DeltaHealth );
		}

		OnHealthChanged.Broadcast( this, CurrentHealth, DeltaHealth );

		if( CurrentHealth <= 0 )
		{
			OnDeath.Broadcast( this );
		}
	}
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = StartingHealth;
}
