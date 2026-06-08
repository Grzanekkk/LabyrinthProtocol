// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabyrinthProtocolWeaponComponent.h"
#include "LabyrinthProtocolCharacter.h"
#include "LabyrinthProtocolProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"

ULabyrinthProtocolWeaponComponent::ULabyrinthProtocolWeaponComponent()
{
	MuzzleOffset = FVector( 100.0f, 0.0f, 10.0f );
	WeaponDisplayName = FText::FromString( TEXT( "Rifle" ) );
	GripRelativeLocation = FVector( 0.0f, 17.0f, 10.0f );
	GripRelativeRotation = FRotator( 0.0f, 180.0f, 0.0f );
	GripRelativeScale = FVector( 0.01f, 0.01f, 0.01f );
}

void ULabyrinthProtocolWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = FMath::Clamp( CurrentAmmo, 0, MaxAmmo );

	if( MaxReserveAmmo > 0 )
	{
		ReserveAmmo = FMath::Clamp( ReserveAmmo, 0, MaxReserveAmmo );
	}
	else
	{
		ReserveAmmo = FMath::Max( ReserveAmmo, 0 );
	}
}

bool ULabyrinthProtocolWeaponComponent::CanFire() const
{
	return CurrentAmmo > 0;
}

void ULabyrinthProtocolWeaponComponent::ConsumeAmmo()
{
	CurrentAmmo = FMath::Max( CurrentAmmo - 1, 0 );
	OnAmmoChanged.Broadcast( this );
}

void ULabyrinthProtocolWeaponComponent::PlayFireEffects()
{
	if( FireSound != nullptr && Character != nullptr )
	{
		UGameplayStatics::PlaySoundAtLocation( this, FireSound, Character->GetActorLocation() );
	}

	if( FireAnimation != nullptr && Character != nullptr )
	{
		if( UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance() )
		{
			AnimInstance->Montage_Play( FireAnimation, 1.f );
		}
	}

	ProcessFiredBP();
}

void ULabyrinthProtocolWeaponComponent::Fire()
{
	if( !CanFire() )
	{
		return;
	}

	if( Character == nullptr || Character->GetController() == nullptr )
	{
		return;
	}

	if( ProjectileClass != nullptr )
	{
		UWorld* const World = GetWorld();
		if( World != nullptr )
		{
			APlayerController* const PlayerController = Cast< APlayerController >( Character->GetController() );
			if( PlayerController == nullptr || PlayerController->PlayerCameraManager == nullptr )
			{
				return;
			}

			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			FVector SpawnLocation = GetBoneLocation( MuzzleSocketName );
			if( SpawnLocation.IsNearlyZero() )
			{
				SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector( MuzzleOffset );
			}

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ActorSpawnParams.Instigator = Character;
			ActorSpawnParams.Owner = Character;

			World->SpawnActor< ALabyrinthProtocolProjectile >( ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams );
		}

		ConsumeAmmo();
	}

	PlayFireEffects();
}

void ULabyrinthProtocolWeaponComponent::Reload()
{
	if( !CanReload() )
	{
		return;
	}

	const int32 AmmoNeeded = MaxAmmo - CurrentAmmo;
	const int32 AmmoToTransfer = FMath::Min( AmmoNeeded, ReserveAmmo );
	if( AmmoToTransfer <= 0 )
	{
		return;
	}

	CurrentAmmo += AmmoToTransfer;
	ReserveAmmo -= AmmoToTransfer;
	OnAmmoChanged.Broadcast( this );
	ProcessReloadedBP();
}

bool ULabyrinthProtocolWeaponComponent::AddReserveAmmo( int32 Amount )
{
	if( Amount <= 0 )
	{
		return false;
	}

	if( MaxReserveAmmo > 0 )
	{
		ReserveAmmo = FMath::Min( ReserveAmmo + Amount, MaxReserveAmmo );
	}
	else
	{
		ReserveAmmo += Amount;
	}

	OnAmmoChanged.Broadcast( this );
	return true;
}

bool ULabyrinthProtocolWeaponComponent::CanReload() const
{
	return CurrentAmmo < MaxAmmo && ReserveAmmo > 0;
}

FText ULabyrinthProtocolWeaponComponent::GetAmmoTypeDisplayName() const
{
	const UEnum* const AmmoTypeEnum = StaticEnum< ELabyrinthProtocolAmmoType >();
	if( AmmoTypeEnum != nullptr )
	{
		return AmmoTypeEnum->GetDisplayNameTextByValue( static_cast< int64 >( AmmoType ) );
	}

	return FText::GetEmpty();
}

void ULabyrinthProtocolWeaponComponent::InitializeWeapon( ALabyrinthProtocolCharacter* TargetCharacter )
{
	Character = TargetCharacter;
	if( Character == nullptr )
	{
		return;
	}

	FAttachmentTransformRules AttachmentRules( EAttachmentRule::SnapToTarget, true );
	AttachToComponent( Character->GetMesh1P(), AttachmentRules, GripSocketName );
	ApplyGripTransform();
}

void ULabyrinthProtocolWeaponComponent::ApplyGripTransform()
{
	SetRelativeLocation( GripRelativeLocation );
	SetRelativeRotation( GripRelativeRotation );
	SetRelativeScale3D( GripRelativeScale );
}

void ULabyrinthProtocolWeaponComponent::SetWeaponEquipped( bool bEquipped, bool bEnableInput )
{
	bIsEquipped = bEquipped;
	SetHiddenInGame( !bEquipped, true );
	SetVisibility( bEquipped, true );

	if( bEquipped && bEnableInput )
	{
		EnableWeaponInput();
	}
	else
	{
		DisableWeaponInput();
	}
}

void ULabyrinthProtocolWeaponComponent::EnableWeaponInput()
{
	if( bInputBound || Character == nullptr )
	{
		return;
	}

	APlayerController* const PlayerController = Cast< APlayerController >( Character->GetController() );
	if( PlayerController == nullptr )
	{
		return;
	}

	if( UEnhancedInputLocalPlayerSubsystem* const Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem >( PlayerController->GetLocalPlayer() ) )
	{
		if( FireMappingContext != nullptr )
		{
			Subsystem->AddMappingContext( FireMappingContext, 1 );
		}
	}

	if( UEnhancedInputComponent* const EnhancedInputComponent = Cast< UEnhancedInputComponent >( PlayerController->InputComponent ) )
	{
		if( FireAction != nullptr )
		{
			FireInputBindingHandle = EnhancedInputComponent->BindAction( FireAction, ETriggerEvent::Triggered, this, &ULabyrinthProtocolWeaponComponent::Fire ).GetHandle();
		}

		if( ReloadAction != nullptr )
		{
			ReloadInputBindingHandle = EnhancedInputComponent->BindAction( ReloadAction, ETriggerEvent::Triggered, this, &ULabyrinthProtocolWeaponComponent::Reload ).GetHandle();
		}
	}

	bInputBound = true;
}

void ULabyrinthProtocolWeaponComponent::DisableWeaponInput()
{
	if( Character == nullptr )
	{
		return;
	}

	if( APlayerController* const PlayerController = Cast< APlayerController >( Character->GetController() ) )
	{
		if( UEnhancedInputLocalPlayerSubsystem* const Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem >( PlayerController->GetLocalPlayer() ) )
		{
			if( FireMappingContext != nullptr )
			{
				Subsystem->RemoveMappingContext( FireMappingContext );
			}
		}

		if( UEnhancedInputComponent* const EnhancedInputComponent = Cast< UEnhancedInputComponent >( PlayerController->InputComponent ) )
		{
			if( FireInputBindingHandle != 0 )
			{
				EnhancedInputComponent->RemoveBindingByHandle( FireInputBindingHandle );
				FireInputBindingHandle = 0;
			}

			if( ReloadInputBindingHandle != 0 )
			{
				EnhancedInputComponent->RemoveBindingByHandle( ReloadInputBindingHandle );
				ReloadInputBindingHandle = 0;
			}
		}
	}

	bInputBound = false;
}

bool ULabyrinthProtocolWeaponComponent::AttachWeapon( ALabyrinthProtocolCharacter* TargetCharacter )
{
	InitializeWeapon( TargetCharacter );
	if( Character == nullptr )
	{
		return false;
	}

	SetWeaponEquipped( true, true );
	Character->RegisterTrackedWeapon( this );
	return true;
}

void ULabyrinthProtocolWeaponComponent::EndPlay( const EEndPlayReason::Type EndPlayReason )
{
	DisableWeaponInput();
	Super::EndPlay( EndPlayReason );
}
