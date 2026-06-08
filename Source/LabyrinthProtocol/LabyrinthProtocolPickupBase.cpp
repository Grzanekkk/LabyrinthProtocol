// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabyrinthProtocolPickupBase.h"
#include "LabyrinthProtocolCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundAttenuation.h"

ALabyrinthProtocolPickupBase::ALabyrinthProtocolPickupBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject< USphereComponent >( TEXT( "CollisionComponent" ) );
	CollisionComponent->InitSphereRadius( 40.0f );
	CollisionComponent->SetCollisionProfileName( TEXT( "OverlapAllDynamic" ) );
	RootComponent = CollisionComponent;

	MeshComponent = CreateDefaultSubobject< UStaticMeshComponent >( TEXT( "MeshComponent" ) );
	MeshComponent->SetupAttachment( RootComponent );
	MeshComponent->SetCollisionEnabled( ECollisionEnabled::NoCollision );
}

void ALabyrinthProtocolPickupBase::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic( this, &ALabyrinthProtocolPickupBase::OnPickupOverlap );
}

void ALabyrinthProtocolPickupBase::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );

	if( MeshComponent != nullptr && PickupRotationSpeed > 0.0f )
	{
		MeshComponent->AddLocalRotation( FRotator( 0.0f, PickupRotationSpeed * DeltaSeconds, 0.0f ) );
	}
}

void ALabyrinthProtocolPickupBase::OnPickupOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	if( bWasCollected )
	{
		return;
	}

	ALabyrinthProtocolCharacter* const Character = Cast< ALabyrinthProtocolCharacter >( OtherActor );
	if( Character == nullptr )
	{
		return;
	}

	if( TryApplyPickup( Character ) )
	{
		bWasCollected = true;
		PlayPickupSound( Character );
		OnPickupCollected( Character );
		OnCollectedBP( Character );
		Destroy();
	}
	else
	{
		PlayPickupDeniedSound( Character );
	}
}

bool ALabyrinthProtocolPickupBase::TryApplyPickup( ALabyrinthProtocolCharacter* Character )
{
	return Character != nullptr;
}

void ALabyrinthProtocolPickupBase::OnPickupCollected( ALabyrinthProtocolCharacter* Character )
{
}

void ALabyrinthProtocolPickupBase::PlayPickupSound( ALabyrinthProtocolCharacter* Character ) const
{
	if( PickupSound == nullptr )
	{
		return;
	}

	const FVector SoundLocation = bPlayPickupSoundAtCharacter && Character != nullptr
		? Character->GetActorLocation()
		: GetActorLocation();

	UGameplayStatics::PlaySoundAtLocation(
		this,
		PickupSound,
		SoundLocation,
		PickupSoundVolume,
		1.0f,
		0.0f,
		PickupSoundAttenuation
	);
}

void ALabyrinthProtocolPickupBase::PlayPickupDeniedSound( ALabyrinthProtocolCharacter* Character ) const
{
	if( PickupDeniedSound == nullptr )
	{
		return;
	}

	const FVector SoundLocation = bPlayPickupSoundAtCharacter && Character != nullptr
		? Character->GetActorLocation()
		: GetActorLocation();

	UGameplayStatics::PlaySoundAtLocation(
		this,
		PickupDeniedSound,
		SoundLocation,
		PickupSoundVolume,
		1.0f,
		0.0f,
		PickupSoundAttenuation
	);
}
