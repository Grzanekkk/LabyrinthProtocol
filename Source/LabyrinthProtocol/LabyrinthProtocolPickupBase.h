// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LabyrinthProtocolPickupBase.generated.h"

class ALabyrinthProtocolCharacter;
class USphereComponent;
class UStaticMeshComponent;
class USoundBase;
class USoundAttenuation;

UCLASS( Abstract, Blueprintable )
class LABYRINTHPROTOCOL_API ALabyrinthProtocolPickupBase : public AActor
{
	GENERATED_BODY()

public:
	ALabyrinthProtocolPickupBase();

protected:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Pickup" )
	TObjectPtr< USphereComponent > CollisionComponent;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Pickup" )
	TObjectPtr< UStaticMeshComponent > MeshComponent;

	UPROPERTY( EditDefaultsOnly, Category = "Pickup", meta = ( ClampMin = "0.0", UIMin = "0.0" ) )
	float PickupRotationSpeed = 90.0f;

	UPROPERTY( EditDefaultsOnly, Category = "Pickup|Audio" )
	TObjectPtr< USoundBase > PickupSound;

	UPROPERTY( EditDefaultsOnly, Category = "Pickup|Audio" )
	TObjectPtr< USoundBase > PickupDeniedSound;

	UPROPERTY( EditDefaultsOnly, Category = "Pickup|Audio", meta = ( ClampMin = "0.0", UIMin = "0.0" ) )
	float PickupSoundVolume = 1.0f;

	UPROPERTY( EditDefaultsOnly, Category = "Pickup|Audio" )
	TObjectPtr< USoundAttenuation > PickupSoundAttenuation;

	UPROPERTY( EditDefaultsOnly, Category = "Pickup|Audio" )
	bool bPlayPickupSoundAtCharacter = true;

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
	void OnPickupOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

	virtual bool TryApplyPickup( ALabyrinthProtocolCharacter* Character );
	virtual void OnPickupCollected( ALabyrinthProtocolCharacter* Character );

	void PlayPickupSound( ALabyrinthProtocolCharacter* Character ) const;
	void PlayPickupDeniedSound( ALabyrinthProtocolCharacter* Character ) const;

	UFUNCTION( BlueprintImplementableEvent, Category = "Pickup" )
	void OnCollectedBP( ALabyrinthProtocolCharacter* Character );

private:
	bool bWasCollected = false;
};
