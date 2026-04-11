// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "LabyrinthProtocolWeaponComponent.generated.h"

class ALabyrinthProtocolCharacter;

UCLASS( Blueprintable, BlueprintType, ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class LABYRINTHPROTOCOL_API ULabyrinthProtocolWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Projectile class to spawn */
	UPROPERTY( EditDefaultsOnly, Category = Projectile )
	TSubclassOf< class ALabyrinthProtocolProjectile > ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input )
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input )
	class UInputAction* FireAction;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Camera" )
	FName MuzzleSocketName = "Muzzle";

	/** Sets default values for this component's properties */
	ULabyrinthProtocolWeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION( BlueprintCallable, Category = "Weapon" )
	bool AttachWeapon( ALabyrinthProtocolCharacter* TargetCharacter );

	/** Make the weapon Fire a Projectile */
	UFUNCTION( BlueprintCallable, Category = "Weapon" )
	void Fire();

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;

private:
	/** The Character holding this weapon*/
	ALabyrinthProtocolCharacter* Character;
};
