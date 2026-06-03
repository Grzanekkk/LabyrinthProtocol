// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabyrinthProtocolProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/HealthComponent.h"

ALabyrinthProtocolProjectile::ALabyrinthProtocolProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject< USphereComponent >( TEXT( "SphereComp" ) );
	CollisionComp->InitSphereRadius( 5.0f );
	CollisionComp->BodyInstance.SetCollisionProfileName( "Projectile" );
	CollisionComp->OnComponentHit.AddDynamic( this, &ALabyrinthProtocolProjectile::OnHit );

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride( FWalkableSlopeOverride( WalkableSlope_Unwalkable, 0.f ) );
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject< UProjectileMovementComponent >( TEXT( "ProjectileComp" ) );
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ALabyrinthProtocolProjectile::OnHit( UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit )
{
	if( IsValid( OtherActor ) && IsValid( OtherActor ) && IsValid( OtherComp ) )
	{
		if( OtherComp->IsSimulatingPhysics() )
		{
			OtherComp->AddImpulseAtLocation( GetVelocity() * 100.0f, GetActorLocation() );
			Destroy();
		}

		UHealthComponent* const OtherHealthComponent = OtherActor->GetComponentByClass< UHealthComponent >();
		if( IsValid( OtherHealthComponent ) )
		{
			OtherHealthComponent->AddHealth( -Damage );
		}
	}
}