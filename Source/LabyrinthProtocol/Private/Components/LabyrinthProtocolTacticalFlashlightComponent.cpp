// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/LabyrinthProtocolTacticalFlashlightComponent.h"
#include "Components/SpotLightComponent.h"

ULabyrinthProtocolTacticalFlashlightComponent::ULabyrinthProtocolTacticalFlashlightComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetRelativeLocation( FVector( 18.0f, 12.0f, -6.0f ) );
	SetRelativeRotation( FRotator( -4.0f, 6.0f, 0.0f ) );
}

void ULabyrinthProtocolTacticalFlashlightComponent::RegisterSpotLight( USpotLightComponent* InSpotLight )
{
	SpotLight = InSpotLight;

	if( SpotLight != nullptr )
	{
		SpotLight->Mobility = EComponentMobility::Movable;
		SpotLight->bUseInverseSquaredFalloff = true;
		SpotLight->SetRelativeLocation( FVector::ZeroVector );
		SpotLight->SetRelativeRotation( FRotator::ZeroRotator );
		ApplyFlashlightSettings();
	}
}

void ULabyrinthProtocolTacticalFlashlightComponent::BeginPlay()
{
	Super::BeginPlay();
	ApplyFlashlightSettings();
}

void ULabyrinthProtocolTacticalFlashlightComponent::ApplyFlashlightSettings()
{
	if( SpotLight == nullptr )
	{
		return;
	}

	SpotLight->SetIntensity( LightIntensity );
	SpotLight->SetLightColor( LightColor );
	SpotLight->SetInnerConeAngle( InnerConeAngle );
	SpotLight->SetOuterConeAngle( OuterConeAngle );
	SpotLight->AttenuationRadius = AttenuationRadius;
	SpotLight->VolumetricScatteringIntensity = VolumetricScatteringIntensity;
	SpotLight->SetCastShadows( bCastShadows );
	SpotLight->SetVisibility( bFlashlightEnabled );
}

void ULabyrinthProtocolTacticalFlashlightComponent::SetFlashlightEnabled( bool bEnabled )
{
	bFlashlightEnabled = bEnabled;
	ApplyFlashlightSettings();
}
