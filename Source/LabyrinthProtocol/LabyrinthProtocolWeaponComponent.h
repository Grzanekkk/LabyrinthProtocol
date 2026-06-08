// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "LabyrinthProtocolAmmoTypes.h"
#include "LabyrinthProtocolWeaponComponent.generated.h"

class ALabyrinthProtocolCharacter;
class UInputMappingContext;
class UInputAction;

UCLASS( Blueprintable, BlueprintType, ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class LABYRINTHPROTOCOL_API ULabyrinthProtocolWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnAmmoChanged, ULabyrinthProtocolWeaponComponent*, Weapon );

	UPROPERTY( Category = "Ammo", BlueprintAssignable )
	FOnAmmoChanged OnAmmoChanged;

public:
	UPROPERTY( EditDefaultsOnly, Category = "Weapon" )
	FText WeaponDisplayName;

	UPROPERTY( EditDefaultsOnly, Category = "Weapon" )
	ELabyrinthProtocolAmmoType AmmoType = ELabyrinthProtocolAmmoType::Rifle;

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

	/** Maximum rounds held in the magazine. */
	UPROPERTY( Category = "Ammo", EditAnywhere, BlueprintReadOnly, meta = ( ClampMin = "0", UIMin = "0" ) )
	int32 MaxAmmo = 30;

	/** Rounds currently loaded in the magazine. */
	UPROPERTY( Category = "Ammo", BlueprintReadOnly, VisibleAnywhere, meta = ( ClampMin = "0", UIMin = "0" ) )
	int32 CurrentAmmo = 30;

	/** Spare rounds carried outside the magazine. */
	UPROPERTY( Category = "Ammo", BlueprintReadOnly, VisibleAnywhere, meta = ( ClampMin = "0", UIMin = "0" ) )
	int32 ReserveAmmo = 90;

	/** Maximum reserve capacity. 0 means unlimited reserve. */
	UPROPERTY( Category = "Ammo", EditAnywhere, BlueprintReadOnly, meta = ( ClampMin = "0", UIMin = "0" ) )
	int32 MaxReserveAmmo = 120;

public:
	ULabyrinthProtocolWeaponComponent();

	UFUNCTION( BlueprintCallable, Category = "Weapon" )
	bool AttachWeapon( ALabyrinthProtocolCharacter* TargetCharacter );

	UFUNCTION( BlueprintCallable, Category = "Weapon" )
	void Fire();

	UFUNCTION( BlueprintCallable, Category = "Weapon" )
	void Reload();

	UFUNCTION( BlueprintCallable, Category = "Weapon|Ammo" )
	bool AddReserveAmmo( int32 Amount );

	UFUNCTION( BlueprintPure, Category = "Weapon|Ammo" )
	int32 GetCurrentAmmo() const
	{
		return CurrentAmmo;
	}

	UFUNCTION( BlueprintPure, Category = "Weapon|Ammo" )
	int32 GetMaxAmmo() const
	{
		return MaxAmmo;
	}

	UFUNCTION( BlueprintPure, Category = "Weapon|Ammo" )
	int32 GetMagazineAmmo() const
	{
		return CurrentAmmo;
	}

	UFUNCTION( BlueprintPure, Category = "Weapon|Ammo" )
	int32 GetMaxMagazineAmmo() const
	{
		return MaxAmmo;
	}

	UFUNCTION( BlueprintPure, Category = "Weapon|Ammo" )
	int32 GetReserveAmmo() const
	{
		return ReserveAmmo;
	}

	UFUNCTION( BlueprintPure, Category = "Weapon|Ammo" )
	int32 GetMaxReserveAmmo() const
	{
		return MaxReserveAmmo;
	}

	UFUNCTION( BlueprintPure, Category = "Weapon|Ammo" )
	bool CanReload() const;

	UFUNCTION( BlueprintPure, Category = "Weapon" )
	FText GetWeaponDisplayName() const
	{
		return WeaponDisplayName;
	}

	UFUNCTION( BlueprintPure, Category = "Weapon" )
	ELabyrinthProtocolAmmoType GetAmmoType() const
	{
		return AmmoType;
	}

	UFUNCTION( BlueprintPure, Category = "Weapon" )
	FText GetAmmoTypeDisplayName() const;

	UFUNCTION( BlueprintImplementableEvent, Category = "Weapon" )
	void ProcessFiredBP();

	UFUNCTION( BlueprintImplementableEvent, Category = "Weapon" )
	void ProcessReloadedBP();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;

	virtual bool CanFire() const;
	virtual void ConsumeAmmo();
	virtual void PlayFireEffects();

private:
	ALabyrinthProtocolCharacter* Character = nullptr;
};
