// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LabyrinthProtocolProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS( config = Game )
class ALabyrinthProtocolProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY( Category = "Projectile", VisibleDefaultsOnly )
	USphereComponent* CollisionComp;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = ( AllowPrivateAccess = "true" ) )
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY( Category = "Projectile", EditDefaultsOnly )
	int32 Damage = 30;

public:
	ALabyrinthProtocolProjectile();

	UFUNCTION()
	void OnHit( UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

	USphereComponent* GetCollisionComp() const
	{
		return CollisionComp;
	}
	UProjectileMovementComponent* GetProjectileMovement() const
	{
		return ProjectileMovement;
	}
};
