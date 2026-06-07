// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LabyrinthProtocolProjectile.h"
#include "LabyrinthProtocolGrenadeProjectile.generated.h"

UCLASS()
class LABYRINTHPROTOCOL_API ALabyrinthProtocolGrenadeProjectile : public ALabyrinthProtocolProjectile
{
	GENERATED_BODY()

public:
	ALabyrinthProtocolGrenadeProjectile();

	virtual void OnHit( UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit ) override;

protected:
	UPROPERTY( Category = "Explosion", EditDefaultsOnly, BlueprintReadOnly, meta = ( ClampMin = "0.0", UIMin = "0.0" ) )
	float ExplosionRadius = 350.0f;

	UPROPERTY( Category = "Explosion", EditDefaultsOnly, BlueprintReadOnly, meta = ( ClampMin = "0", UIMin = "0" ) )
	int32 ExplosionDamage = 80;

	UPROPERTY( Category = "Explosion", EditDefaultsOnly, BlueprintReadOnly )
	TEnumAsByte< ECollisionChannel > ExplosionTraceChannel = ECC_Pawn;

	UFUNCTION( BlueprintImplementableEvent, Category = "Explosion" )
	void OnExplodedBP( const FVector& ExplosionLocation );

private:
	void Explode( const FVector& ExplosionLocation );

	bool bHasExploded = false;
};
