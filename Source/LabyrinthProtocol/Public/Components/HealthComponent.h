// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class LABYRINTHPROTOCOL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
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
	void AddHealth( int32 const DeltaHealth );

protected:
	virtual void BeginPlay() override;
};
