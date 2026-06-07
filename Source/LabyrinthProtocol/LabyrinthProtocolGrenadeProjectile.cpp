// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabyrinthProtocolGrenadeProjectile.h"
#include "Components/HealthComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"

ALabyrinthProtocolGrenadeProjectile::ALabyrinthProtocolGrenadeProjectile()
{
	if( ProjectileMovement != nullptr )
	{
		ProjectileMovement->InitialSpeed = 1800.f;
		ProjectileMovement->MaxSpeed = 1800.f;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->bShouldBounce = false;
		ProjectileMovement->ProjectileGravityScale = 1.0f;
	}

	InitialLifeSpan = 8.0f;
}

void ALabyrinthProtocolGrenadeProjectile::OnHit( UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit )
{
	if( bHasExploded )
	{
		return;
	}

	if( !IsValid( OtherActor ) || !IsValid( OtherComp ) )
	{
		return;
	}

	if( OtherActor == GetInstigator() || OtherActor == GetOwner() )
	{
		return;
	}

	Explode( Hit.ImpactPoint );
}

void ALabyrinthProtocolGrenadeProjectile::Explode( const FVector& ExplosionLocation )
{
	if( bHasExploded )
	{
		return;
	}

	bHasExploded = true;

	UWorld* const World = GetWorld();
	if( World != nullptr )
	{
		TArray< FOverlapResult > OverlapResults;
		FCollisionQueryParams QueryParams( SCENE_QUERY_STAT( GrenadeExplosionOverlap ), false, this );
		QueryParams.AddIgnoredActor( this );

		if( AActor* const ProjectileOwner = GetOwner() )
		{
			QueryParams.AddIgnoredActor( ProjectileOwner );
		}

		if( APawn* const InstigatorPawn = GetInstigator() )
		{
			QueryParams.AddIgnoredActor( InstigatorPawn );
		}

		const bool bHasOverlaps = World->OverlapMultiByChannel(
			OverlapResults,
			ExplosionLocation,
			FQuat::Identity,
			ExplosionTraceChannel,
			FCollisionShape::MakeSphere( ExplosionRadius ),
			QueryParams
		);

		if( bHasOverlaps )
		{
			TSet< AActor* > DamagedActors;

			for( const FOverlapResult& OverlapResult : OverlapResults )
			{
				AActor* const HitActor = OverlapResult.GetActor();
				if( !IsValid( HitActor ) || DamagedActors.Contains( HitActor ) )
				{
					continue;
				}

				UHealthComponent* const HealthComponent = HitActor->GetComponentByClass< UHealthComponent >();
				if( !IsValid( HealthComponent ) )
				{
					continue;
				}

				const float Distance = FVector::Dist( ExplosionLocation, HitActor->GetActorLocation() );
				const float DamageFalloff = 1.0f - FMath::Clamp( Distance / ExplosionRadius, 0.0f, 1.0f );
				const int32 DamageToApply = FMath::RoundToInt( static_cast< float >( ExplosionDamage ) * DamageFalloff );

				if( DamageToApply > 0 )
				{
					HealthComponent->ApplyDamage( DamageToApply );
					DamagedActors.Add( HitActor );
				}
			}
		}
	}

	OnExplodedBP( ExplosionLocation );
	Destroy();
}
