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
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
ULabyrinthProtocolWeaponComponent::ULabyrinthProtocolWeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector( 100.0f, 0.0f, 10.0f );
}

void ULabyrinthProtocolWeaponComponent::Fire()
{
	if( CurrentAmmo > 0 && !IsReloading)
	{
		if( Character == nullptr || Character->GetController() == nullptr )
		{
			return;
		}

		// Try and fire a projectile
		if( ProjectileClass != nullptr )
		{
			UWorld* const World = GetWorld();
			if( World != nullptr )
			{
				APlayerController* PlayerController = Cast< APlayerController >( Character->GetController() );
				const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				// const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
				// FVector SpawnLocation = GetSocketLocation( MuzzleSocketName );
				FVector SpawnLocation = GetBoneLocation( MuzzleSocketName );
				if( SpawnLocation.IsNearlyZero() )
				{
					SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector( MuzzleOffset );
				}

				// DrawDebugSphere( World, SpawnLocation, 5.f, 8, FColor::Red, false, 5 );
				//  Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// Spawn the projectile at the muzzle
				World->SpawnActor< ALabyrinthProtocolProjectile >( ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams );
			}

			CurrentAmmo = CurrentAmmo - 1;
			OnAmmoChanged.Broadcast( this );
		}

		// Try and play the sound if specified
		if( FireSound != nullptr )
		{
			UGameplayStatics::PlaySoundAtLocation( this, FireSound, Character->GetActorLocation() );
		}

		// Try and play a firing animation if specified
		if( FireAnimation != nullptr )
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
			if( AnimInstance != nullptr )
			{
				AnimInstance->Montage_Play( FireAnimation, 1.f );
			}
		}

		ProcessFiredBP();
	}
}

void ULabyrinthProtocolWeaponComponent::Reload()
{
	// Jeœli ju¿ trwa prze³adowanie, nie rób nic
	if( GetWorld()->GetTimerManager().IsTimerActive( ReloadTimerHandle ) )
	{
		return;
	}
	IsReloading = true;
	// Ustaw timer na 1 sekundê (mo¿esz zmieniæ na dowoln¹ wartoœæ)
	GetWorld()->GetTimerManager().SetTimer( ReloadTimerHandle, this, &ULabyrinthProtocolWeaponComponent::FinishReload,
		2.0f, // czas w sekundach
		false );
	CurrentAmmo = MaxAmmo;
	OnAmmoChanged.Broadcast( this );
	ProcessReloadedBP();
}

void ULabyrinthProtocolWeaponComponent::FinishReload()
{
	IsReloading = false;
}

bool ULabyrinthProtocolWeaponComponent::AttachWeapon( ALabyrinthProtocolCharacter* TargetCharacter )
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if( Character == nullptr || Character->GetInstanceComponents().FindItemByClass< ULabyrinthProtocolWeaponComponent >() )
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules( EAttachmentRule::SnapToTarget, true );
	AttachToComponent( Character->GetMesh1P(), AttachmentRules, FName( TEXT( "GripPoint" ) ) );

	// Set up action bindings
	if( APlayerController* PlayerController = Cast< APlayerController >( Character->GetController() ) )
	{
		if( UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem >( PlayerController->GetLocalPlayer() ) )
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext( FireMappingContext, 1 );
		}

		if( UEnhancedInputComponent* EnhancedInputComponent = Cast< UEnhancedInputComponent >( PlayerController->InputComponent ) )
		{
			// Fire
			EnhancedInputComponent->BindAction( FireAction, ETriggerEvent::Triggered, this, &ULabyrinthProtocolWeaponComponent::Fire );
			EnhancedInputComponent->BindAction( ReloadAction, ETriggerEvent::Triggered, this, &ULabyrinthProtocolWeaponComponent::Reload );
		}
	}

	return true;
}

void ULabyrinthProtocolWeaponComponent::EndPlay( const EEndPlayReason::Type EndPlayReason )
{
	// ensure we have a character owner
	if( Character != nullptr )
	{
		// remove the input mapping context from the Player Controller
		if( APlayerController* PlayerController = Cast< APlayerController >( Character->GetController() ) )
		{
			if( UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem >( PlayerController->GetLocalPlayer() ) )
			{
				Subsystem->RemoveMappingContext( FireMappingContext );
			}
		}
	}

	// maintain the EndPlay call chain
	Super::EndPlay( EndPlayReason );
}