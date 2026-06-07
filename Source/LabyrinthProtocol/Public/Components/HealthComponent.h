// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class LABYRINTHPROTOCOL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams( FOnHealthChanged, UHealthComponent* const, HealthComponent, int32, CurrentHealth, int32, HealthDelta );
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams( FOnDamaged, UHealthComponent* const, HealthComponent, int32, CurrentHealth, int32, Damage );
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams( FOnHealed, UHealthComponent* const, HealthComponent, int32, CurrentHealth, int32, Healing );
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnDeath, UHealthComponent* const, HealthComponent );

	UPROPERTY( Category = "Health", BlueprintAssignable )
	FOnHealthChanged OnHealthChanged;

	UPROPERTY( Category = "Health", BlueprintAssignable )
	FOnDamaged OnDamaged;

	UPROPERTY( Category = "Health", BlueprintAssignable )
	FOnHealed OnHealed;

	UPROPERTY( Category = "Health", BlueprintAssignable )
	FOnDeath OnDeath;

protected:
	UPROPERTY( Category = "Health", BlueprintReadOnly, EditDefaultsOnly, Meta = ( ClampMin = 1, UIMin = 1 ) )
	int32 MaxHealth = 100;

	UPROPERTY( Category = "Health", BlueprintReadOnly, EditDefaultsOnly, Meta = ( ClampMin = 1, UIMin = 1 ) )
	int32 StartingHealth = 100;

	UPROPERTY( Category = "Health", BlueprintReadOnly, VisibleAnywhere, Meta = ( ClampMin = 1, UIMin = 1 ) )
	int32 CurrentHealth = 100;

public:
	UFUNCTION( BlueprintCallable )
	void AddHealth( int32 const DeltaHealth );

	UFUNCTION( BlueprintCallable )
	void ApplyDamage( int32 const DeltaDamage );

	UFUNCTION( BlueprintPure, Category = "Health" )
	int32 GetCurrentHealth() const
	{
		return CurrentHealth;
	}

	UFUNCTION( BlueprintPure, Category = "Health" )
	int32 GetMaxHealth() const
	{
		return MaxHealth;
	}

	UFUNCTION( BlueprintPure, Category = "Health" )
	float GetHealthPercent() const
	{
		return MaxHealth > 0 ? static_cast< float >( CurrentHealth ) / static_cast< float >( MaxHealth ) : 0.0f;
	}

	UFUNCTION( BlueprintPure, Category = "Health" )
	bool IsDead() const
	{
		return bIsDead;
	}

protected:
	virtual void BeginPlay() override;

private:
	bool bIsDead = false;
};
