// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabyrinthProtocolCharacter.h"
#include "LabyrinthProtocolWeaponComponent.h"
#include "Components/WeaponInventoryComponent.h"
#include "Components/HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY( LogTemplateCharacter );

ALabyrinthProtocolCharacter::ALabyrinthProtocolCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize( 55.f, 96.0f );

	FirstPersonCameraComponent = CreateDefaultSubobject< UCameraComponent >( TEXT( "FirstPersonCamera" ) );
	FirstPersonCameraComponent->SetupAttachment( GetCapsuleComponent() );
	FirstPersonCameraComponent->SetRelativeLocation( FVector( -10.f, 0.f, 60.f ) );
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject< USkeletalMeshComponent >( TEXT( "CharacterMesh1P" ) );
	Mesh1P->SetOnlyOwnerSee( true );
	Mesh1P->SetupAttachment( FirstPersonCameraComponent );
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation( FVector( -30.f, 0.f, -150.f ) );

	WeaponInventoryComponent = CreateDefaultSubobject< UWeaponInventoryComponent >( TEXT( "WeaponInventory" ) );
}

void ALabyrinthProtocolCharacter::BeginPlay()
{
	Super::BeginPlay();

	BindHealthComponent( GetHealthComponent() );

	if( WeaponInventoryComponent == nullptr )
	{
		TryBindWeaponComponent();

		if( GetWorld() != nullptr )
		{
			GetWorld()->GetTimerManager().SetTimer(
				WeaponBindingRetryTimerHandle,
				this,
				&ALabyrinthProtocolCharacter::RetryWeaponBinding,
				0.1f,
				true,
				0.1f
			);
		}
	}

	BroadcastHUDUpdate();
}

void ALabyrinthProtocolCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if( APlayerController* PlayerController = Cast< APlayerController >( Controller ) )
	{
		if( UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem >( PlayerController->GetLocalPlayer() ) )
		{
			Subsystem->AddMappingContext( DefaultMappingContext, 0 );
		}
	}

	RefreshActiveWeaponInput();
	InitializeHUDBindings();
}

void ALabyrinthProtocolCharacter::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	if( UEnhancedInputComponent* EnhancedInputComponent = Cast< UEnhancedInputComponent >( PlayerInputComponent ) )
	{
		EnhancedInputComponent->BindAction( JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump );
		EnhancedInputComponent->BindAction( JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping );
		EnhancedInputComponent->BindAction( MoveAction, ETriggerEvent::Triggered, this, &ALabyrinthProtocolCharacter::Move );
		EnhancedInputComponent->BindAction( LookAction, ETriggerEvent::Triggered, this, &ALabyrinthProtocolCharacter::Look );

		if( WeaponSlot1Action != nullptr )
		{
			EnhancedInputComponent->BindAction( WeaponSlot1Action, ETriggerEvent::Started, this, &ALabyrinthProtocolCharacter::SelectWeaponSlot1 );
		}

		if( WeaponSlot2Action != nullptr )
		{
			EnhancedInputComponent->BindAction( WeaponSlot2Action, ETriggerEvent::Started, this, &ALabyrinthProtocolCharacter::SelectWeaponSlot2 );
		}
	}
	else
	{
		UE_LOG( LogTemplateCharacter, Error, TEXT( "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file." ), *GetNameSafe( this ) );
	}
}

void ALabyrinthProtocolCharacter::Move( const FInputActionValue& Value )
{
	const FVector2D MovementVector = Value.Get< FVector2D >();

	if( Controller != nullptr )
	{
		AddMovementInput( GetActorForwardVector(), MovementVector.Y );
		AddMovementInput( GetActorRightVector(), MovementVector.X );
	}
}

void ALabyrinthProtocolCharacter::Look( const FInputActionValue& Value )
{
	const FVector2D LookAxisVector = Value.Get< FVector2D >();

	if( Controller != nullptr )
	{
		AddControllerYawInput( LookAxisVector.X );
		AddControllerPitchInput( LookAxisVector.Y );
	}
}

void ALabyrinthProtocolCharacter::SelectWeaponSlot1()
{
	if( WeaponInventoryComponent != nullptr )
	{
		WeaponInventoryComponent->EquipWeaponSlot1();
	}
}

void ALabyrinthProtocolCharacter::SelectWeaponSlot2()
{
	if( WeaponInventoryComponent != nullptr )
	{
		WeaponInventoryComponent->EquipWeaponSlot2();
	}
}

void ALabyrinthProtocolCharacter::RefreshActiveWeaponInput()
{
	if( WeaponInventoryComponent == nullptr )
	{
		return;
	}

	TArray< ULabyrinthProtocolWeaponComponent* > WeaponComponents;
	GetComponents< ULabyrinthProtocolWeaponComponent >( WeaponComponents );
	for( ULabyrinthProtocolWeaponComponent* WeaponComponent : WeaponComponents )
	{
		if( WeaponComponent != nullptr )
		{
			WeaponComponent->DisableWeaponInput();
		}
	}

	if( ULabyrinthProtocolWeaponComponent* ActiveWeapon = GetWeaponComponent() )
	{
		ActiveWeapon->EnableWeaponInput();
	}
}

ULabyrinthProtocolWeaponComponent* ALabyrinthProtocolCharacter::GetWeaponComponent() const
{
	if( WeaponInventoryComponent != nullptr )
	{
		if( ULabyrinthProtocolWeaponComponent* ActiveWeapon = WeaponInventoryComponent->GetActiveWeapon() )
		{
			return ActiveWeapon;
		}
	}

	TArray< ULabyrinthProtocolWeaponComponent* > WeaponComponents;
	GetComponents< ULabyrinthProtocolWeaponComponent >( WeaponComponents );

	for( ULabyrinthProtocolWeaponComponent* WeaponComponent : WeaponComponents )
	{
		if( WeaponComponent != nullptr && WeaponComponent->IsWeaponEquipped() )
		{
			return WeaponComponent;
		}
	}

	for( ULabyrinthProtocolWeaponComponent* WeaponComponent : WeaponComponents )
	{
		if( WeaponComponent != nullptr && WeaponComponent->IsRegistered() )
		{
			return WeaponComponent;
		}
	}

	return nullptr;
}

UHealthComponent* ALabyrinthProtocolCharacter::GetHealthComponent() const
{
	return FindComponentByClass< UHealthComponent >();
}

FLabyrinthProtocolHUDViewData ALabyrinthProtocolCharacter::BuildHUDViewData() const
{
	FLabyrinthProtocolHUDViewData HUDData;
	HUDData.HealthDelta = LastHUDHealthDelta;
	HUDData.bTookDamage = PendingHUDDamageAmount > 0;
	HUDData.DamageTakenAmount = PendingHUDDamageAmount;

	if( const UHealthComponent* HealthComponent = GetHealthComponent() )
	{
		HUDData.CurrentHealth = HealthComponent->GetCurrentHealth();
		HUDData.MaxHealth = HealthComponent->GetMaxHealth();
		HUDData.HealthPercent = HealthComponent->GetHealthPercent();
	}

	if( const ULabyrinthProtocolWeaponComponent* WeaponComponent = GetWeaponComponent() )
	{
		HUDData.bHasWeapon = true;
		HUDData.WeaponName = WeaponComponent->GetWeaponDisplayName();
		HUDData.AmmoType = WeaponComponent->GetAmmoType();
		HUDData.AmmoTypeName = WeaponComponent->GetAmmoTypeDisplayName();
		HUDData.MagazineAmmo = WeaponComponent->GetMagazineAmmo();
		HUDData.MaxMagazineAmmo = WeaponComponent->GetMaxMagazineAmmo();
		HUDData.ReserveAmmo = WeaponComponent->GetReserveAmmo();
		HUDData.MaxReserveAmmo = WeaponComponent->GetMaxReserveAmmo();
		HUDData.bCanReload = WeaponComponent->CanReload();
		HUDData.MagazineAmmoLabel = FText::FromString( FString::Printf(
			TEXT( "%d / %d" ),
			HUDData.MagazineAmmo,
			HUDData.MaxMagazineAmmo
		) );
		HUDData.ReserveAmmoLabel = FText::FromString( FString::Printf(
			TEXT( "Reserve: %d" ),
			HUDData.ReserveAmmo
		) );
	}

	if( WeaponInventoryComponent != nullptr )
	{
		HUDData.ActiveWeaponSlotIndex = WeaponInventoryComponent->GetActiveWeaponSlotIndex();
	}

	return HUDData;
}

void ALabyrinthProtocolCharacter::InitializeHUDBindings()
{
	BindHealthComponent( GetHealthComponent() );
	BindTrackedWeapon( GetWeaponComponent() );
	BroadcastHUDUpdate();
}

void ALabyrinthProtocolCharacter::ClearHUDBindings()
{
	if( BoundHealthComponent != nullptr )
	{
		BoundHealthComponent->OnHealthChanged.RemoveDynamic( this, &ALabyrinthProtocolCharacter::HandleHealthChanged );
		BoundHealthComponent = nullptr;
	}

	if( TrackedWeapon != nullptr )
	{
		TrackedWeapon->OnAmmoChanged.RemoveDynamic( this, &ALabyrinthProtocolCharacter::HandleWeaponAmmoChanged );
		TrackedWeapon = nullptr;
	}
}

void ALabyrinthProtocolCharacter::RegisterTrackedWeapon( ULabyrinthProtocolWeaponComponent* Weapon )
{
	BindTrackedWeapon( Weapon );

	if( GetWorld() != nullptr )
	{
		GetWorld()->GetTimerManager().ClearTimer( WeaponBindingRetryTimerHandle );
	}

	BroadcastHUDUpdate();
}

void ALabyrinthProtocolCharacter::BindHealthComponent( UHealthComponent* HealthComponent )
{
	if( BoundHealthComponent == HealthComponent )
	{
		return;
	}

	if( BoundHealthComponent != nullptr )
	{
		BoundHealthComponent->OnHealthChanged.RemoveDynamic( this, &ALabyrinthProtocolCharacter::HandleHealthChanged );
	}

	BoundHealthComponent = HealthComponent;

	if( BoundHealthComponent != nullptr )
	{
		BoundHealthComponent->OnHealthChanged.AddDynamic( this, &ALabyrinthProtocolCharacter::HandleHealthChanged );
	}
}

void ALabyrinthProtocolCharacter::BindTrackedWeapon( ULabyrinthProtocolWeaponComponent* Weapon )
{
	if( TrackedWeapon == Weapon )
	{
		return;
	}

	if( TrackedWeapon != nullptr )
	{
		TrackedWeapon->OnAmmoChanged.RemoveDynamic( this, &ALabyrinthProtocolCharacter::HandleWeaponAmmoChanged );
	}

	TrackedWeapon = Weapon;

	if( TrackedWeapon != nullptr )
	{
		TrackedWeapon->OnAmmoChanged.AddDynamic( this, &ALabyrinthProtocolCharacter::HandleWeaponAmmoChanged );
	}
}

void ALabyrinthProtocolCharacter::TryBindWeaponComponent()
{
	BindTrackedWeapon( GetWeaponComponent() );
}

void ALabyrinthProtocolCharacter::RetryWeaponBinding()
{
	if( TrackedWeapon != nullptr )
	{
		if( GetWorld() != nullptr )
		{
			GetWorld()->GetTimerManager().ClearTimer( WeaponBindingRetryTimerHandle );
		}
		return;
	}

	TryBindWeaponComponent();

	if( TrackedWeapon != nullptr )
	{
		BroadcastHUDUpdate();

		if( GetWorld() != nullptr )
		{
			GetWorld()->GetTimerManager().ClearTimer( WeaponBindingRetryTimerHandle );
		}
	}
}

void ALabyrinthProtocolCharacter::HandleHealthChanged( UHealthComponent* const HealthComponent, int32 CurrentHealth, int32 HealthDelta )
{
	LastHUDHealthDelta = HealthDelta;
	if( HealthDelta < 0 )
	{
		PendingHUDDamageAmount = FMath::Abs( HealthDelta );
	}

	BroadcastHUDUpdate();

	LastHUDHealthDelta = 0;
	PendingHUDDamageAmount = 0;
}

void ALabyrinthProtocolCharacter::HandleWeaponAmmoChanged( ULabyrinthProtocolWeaponComponent* Weapon )
{
	BroadcastHUDUpdate();
}

void ALabyrinthProtocolCharacter::BroadcastHUDUpdate()
{
	OnHUDDataChanged.Broadcast( BuildHUDViewData() );
}

bool ALabyrinthProtocolCharacter::TryAddReserveAmmo( ELabyrinthProtocolAmmoType AmmoType, int32 Amount )
{
	if( Amount <= 0 )
	{
		return false;
	}

	ULabyrinthProtocolWeaponComponent* const WeaponComponent = GetWeaponComponent();
	if( WeaponComponent == nullptr )
	{
		return false;
	}

	if( WeaponComponent->GetAmmoType() != AmmoType )
	{
		return false;
	}

	const bool bAmmoAdded = WeaponComponent->AddReserveAmmo( Amount );
	if( bAmmoAdded )
	{
		BroadcastHUDUpdate();
	}

	return bAmmoAdded;
}
