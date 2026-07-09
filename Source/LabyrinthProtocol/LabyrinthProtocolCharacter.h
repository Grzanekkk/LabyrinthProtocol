// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UI/LabyrinthProtocolHUDTypes.h"
#include "LabyrinthProtocolAmmoTypes.h"
#include "Logging/LogMacros.h"
#include "LabyrinthProtocolCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UHealthComponent;
class UWeaponInventoryComponent;
class ULabyrinthProtocolWeaponComponent;
class ULabyrinthProtocolTacticalFlashlightComponent;
class USpotLightComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN( LogTemplateCharacter, Log, All );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnHUDDataChanged, FLabyrinthProtocolHUDViewData, HUDData );

UCLASS( config = Game )
class ALabyrinthProtocolCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = ( AllowPrivateAccess = "true" ) )
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ) )
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Flashlight, meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr< ULabyrinthProtocolTacticalFlashlightComponent > TacticalFlashlightComponent;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Flashlight, meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr< USpotLightComponent > TacticalFlashlightSpotLight;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = ( AllowPrivateAccess = "true" ) )
	TObjectPtr< UWeaponInventoryComponent > WeaponInventoryComponent;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	UInputAction* JumpAction;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	UInputAction* MoveAction;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	UInputAction* LookAction;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	UInputAction* WeaponSlot1Action;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Input, meta = ( AllowPrivateAccess = "true" ) )
	UInputAction* WeaponSlot2Action;

	UPROPERTY( Category = "HUD", BlueprintAssignable )
	FOnHUDDataChanged OnHUDDataChanged;

public:
	ALabyrinthProtocolCharacter();

protected:
	virtual void BeginPlay() override;

	void Move( const FInputActionValue& Value );
	void Look( const FInputActionValue& Value );
	void SelectWeaponSlot1();
	void SelectWeaponSlot2();

	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent( UInputComponent* InputComponent ) override;

	UFUNCTION()
	void HandleHealthChanged( UHealthComponent* const HealthComponent, int32 CurrentHealth, int32 HealthDelta );

	UFUNCTION()
	void HandleWeaponAmmoChanged( ULabyrinthProtocolWeaponComponent* Weapon );

	void BindHealthComponent( UHealthComponent* HealthComponent );
	void BindTrackedWeapon( ULabyrinthProtocolWeaponComponent* Weapon );
	void TryBindWeaponComponent();
	void BroadcastHUDUpdate();

	UFUNCTION()
	void RetryWeaponBinding();

public:
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	ULabyrinthProtocolTacticalFlashlightComponent* GetTacticalFlashlightComponent() const { return TacticalFlashlightComponent; }
	USpotLightComponent* GetTacticalFlashlightSpotLight() const { return TacticalFlashlightSpotLight; }

	void RefreshActiveWeaponInput();

	UFUNCTION( BlueprintCallable, Category = "Weapon" )
	ULabyrinthProtocolWeaponComponent* GetWeaponComponent() const;

	UFUNCTION( BlueprintCallable, Category = "Weapon" )
	UWeaponInventoryComponent* GetWeaponInventoryComponent() const
	{
		return WeaponInventoryComponent;
	}

	UFUNCTION( BlueprintCallable, Category = "Health" )
	UHealthComponent* GetHealthComponent() const;

	UFUNCTION( BlueprintCallable, Category = "HUD" )
	FLabyrinthProtocolHUDViewData BuildHUDViewData() const;

	UFUNCTION( BlueprintCallable, Category = "HUD" )
	void InitializeHUDBindings();

	UFUNCTION( BlueprintCallable, Category = "HUD" )
	void ClearHUDBindings();

	UFUNCTION( BlueprintCallable, Category = "HUD" )
	void RegisterTrackedWeapon( ULabyrinthProtocolWeaponComponent* Weapon );

	UFUNCTION( BlueprintCallable, Category = "Weapon|Ammo" )
	bool TryAddReserveAmmo( ELabyrinthProtocolAmmoType AmmoType, int32 Amount );

	UFUNCTION( BlueprintPure, Category = "Weapon|Ammo" )
	ULabyrinthProtocolWeaponComponent* FindWeaponComponentByAmmoType( ELabyrinthProtocolAmmoType AmmoType ) const;

private:
	UPROPERTY()
	TObjectPtr< UHealthComponent > BoundHealthComponent;

	UPROPERTY()
	TObjectPtr< ULabyrinthProtocolWeaponComponent > TrackedWeapon;

	FTimerHandle WeaponBindingRetryTimerHandle;

	int32 LastHUDHealthDelta = 0;
	int32 PendingHUDDamageAmount = 0;
};
