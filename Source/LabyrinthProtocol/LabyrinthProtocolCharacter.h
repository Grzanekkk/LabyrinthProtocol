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

class ULabyrinthProtocolWeaponComponent;

struct FInputActionValue;



DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnHUDDataChanged, FLabyrinthProtocolHUDViewData, HUDData );



UCLASS(config=Game)

class ALabyrinthProtocolCharacter : public ACharacter

{
public:
	GENERATED_BODY()



	/** Pawn mesh: 1st person view (arms; seen only by self) */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))

	USkeletalMeshComponent* Mesh1P;



	/** First person camera */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))

	UCameraComponent* FirstPersonCameraComponent;



	/** MappingContext */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))

	UInputMappingContext* DefaultMappingContext;



	/** Jump Input Action */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))

	UInputAction* JumpAction;



	/** Move Input Action */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))

	UInputAction* MoveAction;



	/** Look Input Action */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))

	class UInputAction* LookAction;



	UPROPERTY( Category = "HUD", BlueprintAssignable )

	FOnHUDDataChanged OnHUDDataChanged;

	

public:

	ALabyrinthProtocolCharacter();



protected:

	virtual void BeginPlay() override;



	/** Called for movement input */

	void Move(const FInputActionValue& Value);



	/** Called for looking input */

	void Look(const FInputActionValue& Value);



protected:

	// APawn interface

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	// End of APawn interface



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

	/** Returns Mesh1P subobject **/

	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	/** Returns FirstPersonCameraComponent subobject **/

	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }



	UFUNCTION( BlueprintCallable, Category = "Weapon" )

	ULabyrinthProtocolWeaponComponent* GetWeaponComponent() const;



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



private:

	UPROPERTY()

	TObjectPtr< UHealthComponent > BoundHealthComponent;



	UPROPERTY()

	TObjectPtr< ULabyrinthProtocolWeaponComponent > TrackedWeapon;



	FTimerHandle WeaponBindingRetryTimerHandle;

	int32 LastHUDHealthDelta = 0;
};


