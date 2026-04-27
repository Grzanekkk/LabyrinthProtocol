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

	UPROPERTY( Category = "Camera", EditAnywhere, BlueprintReadOnly )
	FName MuzzleSocketName = "Muzzle";

	UPROPERTY( Category = "Weapon", EditAnywhere, BlueprintReadOnly )
	int32 MaxAmmo = 30;

	UPROPERTY( Category = "Weapon", EditAnywhere, BlueprintReadOnly )
	int32 CurrentAmmo = 30;

public:
	UFUNCTION( BlueprintCallable, Category = "Weapon" )
	bool AttachWeapon( ALabyrinthProtocolCharacter* TargetCharacter );

	UFUNCTION( BlueprintCallable, Category = "Weapon" )
	void Fire();

	UFUNCTION( BlueprintImplementableEvent, Category = "Weapon" )
	void ProcessFiredBP();

public:
	ULabyrinthProtocolWeaponComponent();

protected:
	UFUNCTION()
	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;

private:
	ALabyrinthProtocolCharacter* Character;
};
