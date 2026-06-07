// Copyright Epic Games, Inc. All Rights Reserved.

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

bool ULabyrinthProtocolWeaponComponent::AttachWeapon( ALabyrinthProtocolCharacter* TargetCharacter )
{
	Character = TargetCharacter;

	if( Character == nullptr || Character->GetInstanceComponents().FindItemByClass< ULabyrinthProtocolWeaponComponent >() )
	{
		return false;
	}

	FAttachmentTransformRules AttachmentRules( EAttachmentRule::SnapToTarget, true );
	AttachToComponent( Character->GetMesh1P(), AttachmentRules, FName( TEXT( "GripPoint" ) ) );

	if( APlayerController* PlayerController = Cast< APlayerController >( Character->GetController() ) )
	{
		if( UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem >( PlayerController->GetLocalPlayer() ) )
		{
			Subsystem->AddMappingContext( FireMappingContext, 1 );
		}

		if( UEnhancedInputComponent* EnhancedInputComponent = Cast< UEnhancedInputComponent >( PlayerController->InputComponent ) )
		{
			EnhancedInputComponent->BindAction( FireAction, ETriggerEvent::Triggered, this, &ULabyrinthProtocolWeaponComponent::Fire );
			EnhancedInputComponent->BindAction( ReloadAction, ETriggerEvent::Triggered, this, &ULabyrinthProtocolWeaponComponent::Reload );
		}
	}

	Character->RegisterTrackedWeapon( this );

	return true;
}

void ULabyrinthProtocolWeaponComponent::EndPlay( const EEndPlayReason::Type EndPlayReason )
{
	if( Character != nullptr )
	{
		if( APlayerController* PlayerController = Cast< APlayerController >( Character->GetController() ) )
		{
			if( UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem >( PlayerController->GetLocalPlayer() ) )
			{
				Subsystem->RemoveMappingContext( FireMappingContext );
			}
		}
	}

	Super::EndPlay( EndPlayReason );
}
