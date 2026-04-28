// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "LabyrinthProtocolWeaponComponent.generated.h"

class ALabyrinthProtocolCharacter;
class UInputMappingContext;
class UInputAction;

UCLASS( Blueprintable, BlueprintType, ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class LABYRINTHPROTOCOL_API ULabyrinthProtocolWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnAmmoChanged, ULabyrinthProtocolWeaponComponent*, Weapon );

	UPROPERTY( Category = "Ammo", BlueprintAssignable )
	FOnAmmoChanged OnAmmoChanged;

public:
	UPROPERTY( EditDefaultsOnly, Category = Projectile )
	TSubclassOf< class ALabyrinthProtocolProjectile > ProjectileClass;

	UPROPERTY( Category = "Effect", EditAnywhere, BlueprintReadWrite )
	USoundBase* FireSound;

	UPROPERTY( Category = "Effect", EditAnywhere, BlueprintReadWrite )
	UAnimMontage* FireAnimation;

	UPROPERTY( Category = "Effect", EditAnywhere, BlueprintReadWrite )
	FVector MuzzleOffset;

	UPROPERTY( Category = "Input", EditAnywhere, BlueprintReadOnly )
	UInputMappingContext* FireMappingContext;

	UPROPERTY( Category = "Input", EditAnywhere, BlueprintReadOnly )
	UInputAction* FireAction;

	UPROPERTY( Category = "Input", EditAnywhere, BlueprintReadOnly )
	UInputAction* ReloadAction;

	UPROPERTY( Category = "Camera", EditAnywhere, BlueprintReadOnly )
	FName MuzzleSocketName = "Muzzle";

	UPROPERTY( Category = "Ammo", EditAnywhere, BlueprintReadOnly )
	int32 MaxAmmo = 30;

	UPROPERTY( Category = "Ammo", EditAnywhere, BlueprintReadOnly )
	int32 CurrentAmmo = 30;

public:
	UFUNCTION( BlueprintCallable, Category = "Weapon" )
	bool AttachWeapon( ALabyrinthProtocolCharacter* TargetCharacter );

	UFUNCTION( BlueprintCallable, Category = "Weapon" )
	void Fire();

	void Reload();

	UFUNCTION( BlueprintImplementableEvent, Category = "Weapon" )
	void ProcessFiredBP();

	UFUNCTION( BlueprintImplementableEvent, Category = "Weapon" )
	void ProcessReloadedBP();

public:
	ULabyrinthProtocolWeaponComponent();

protected:
	UFUNCTION()
	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;

private:
	ALabyrinthProtocolCharacter* Character;
};
