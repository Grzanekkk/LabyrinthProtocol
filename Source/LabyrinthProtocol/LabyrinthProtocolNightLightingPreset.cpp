// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabyrinthProtocolNightLightingPreset.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "TimerManager.h"

ALabyrinthProtocolNightLightingPreset::ALabyrinthProtocolNightLightingPreset()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject< USceneComponent >( TEXT( "SceneRoot" ) );
	SetRootComponent( SceneRoot );

	MoonLight = CreateDefaultSubobject< UDirectionalLightComponent >( TEXT( "MoonLight" ) );
	MoonLight->SetupAttachment( SceneRoot );
	MoonLight->Mobility = EComponentMobility::Movable;
	MoonLight->bAtmosphereSunLight = true;
	MoonLight->SetCastShadows( true );

	SkyLight = CreateDefaultSubobject< USkyLightComponent >( TEXT( "SkyLight" ) );
	SkyLight->SetupAttachment( SceneRoot );
	SkyLight->Mobility = EComponentMobility::Movable;
	SkyLight->SourceType = SLS_CapturedScene;
	SkyLight->bRealTimeCapture = true;
	SkyLight->bLowerHemisphereIsBlack = false;

	SkyAtmosphere = CreateDefaultSubobject< USkyAtmosphereComponent >( TEXT( "SkyAtmosphere" ) );
	SkyAtmosphere->SetupAttachment( SceneRoot );

	HeightFog = CreateDefaultSubobject< UExponentialHeightFogComponent >( TEXT( "HeightFog" ) );
	HeightFog->SetupAttachment( SceneRoot );

	PostProcess = CreateDefaultSubobject< UPostProcessComponent >( TEXT( "PostProcess" ) );
	PostProcess->SetupAttachment( SceneRoot );
	PostProcess->bUnbound = true;
}

void ALabyrinthProtocolNightLightingPreset::OnConstruction( const FTransform& Transform )
{
	Super::OnConstruction( Transform );
	ApplyNightPreset();
}

void ALabyrinthProtocolNightLightingPreset::BeginPlay()
{
	Super::BeginPlay();

	if( bApplyOnBeginPlay )
	{
		ApplyNightPreset();
	}
}

void ALabyrinthProtocolNightLightingPreset::ScheduleSkyLightRecapture()
{
	if( !bRecaptureSkyLightOnApply || SkyLight == nullptr )
	{
		return;
	}

	UWorld* const World = GetWorld();
	if( World == nullptr )
	{
		return;
	}

	World->GetTimerManager().ClearTimer( SkyLightRecaptureTimerHandle );
	World->GetTimerManager().SetTimer(
		SkyLightRecaptureTimerHandle,
		this,
		&ALabyrinthProtocolNightLightingPreset::RecaptureSkyLightDeferred,
		0.1f,
		false
	);
}

void ALabyrinthProtocolNightLightingPreset::RecaptureSkyLightDeferred()
{
	if( SkyLight != nullptr )
	{
		SkyLight->RecaptureSky();
	}
}

void ALabyrinthProtocolNightLightingPreset::ApplyNightPreset()
{
	if( MoonLight != nullptr )
	{
		MoonLight->SetIntensity( DirectionalLightIntensity );
		MoonLight->SetLightColor( DirectionalLightColor );
		MoonLight->SetRelativeRotation( DirectionalLightRotation );
		MoonLight->SetVisibility( true );
	}

	if( SkyLight != nullptr )
	{
		SkyLight->SetIntensity( SkyLightIntensity );
		SkyLight->SetLightColor( SkyLightColor );
		SkyLight->SourceType = SLS_CapturedScene;
		SkyLight->bRealTimeCapture = true;
		SkyLight->SetVisibility( true );
		ScheduleSkyLightRecapture();
	}

	if( SkyAtmosphere != nullptr )
	{
		SkyAtmosphere->MieScatteringScale = MieScatteringScale;
		SkyAtmosphere->SetSkyLuminanceFactor( SkyLuminanceFactor );
	}

	if( HeightFog != nullptr )
	{
		HeightFog->SetFogDensity( FogDensity );
		HeightFog->FogHeightFalloff = FogHeightFalloff;
		HeightFog->FogInscatteringLuminance = FogInscatteringColor;
		HeightFog->bEnableVolumetricFog = bEnableVolumetricFog;
	}

	if( PostProcess != nullptr )
	{
		FPostProcessSettings& Settings = PostProcess->Settings;

		Settings.bOverride_AutoExposureMethod = true;
		Settings.AutoExposureMethod = EAutoExposureMethod::AEM_Histogram;

		Settings.bOverride_AutoExposureBias = true;
		Settings.AutoExposureBias = ExposureCompensation;

		Settings.bOverride_ColorSaturation = true;
		Settings.ColorSaturation = FVector4( ColorSaturation, ColorSaturation, ColorSaturation, 1.0f );

		Settings.bOverride_ColorContrast = true;
		Settings.ColorContrast = FVector4( ColorContrast, ColorContrast, ColorContrast, 1.0f );

		Settings.bOverride_ColorGainShadows = true;
		Settings.ColorGainShadows = FVector4( 0.9f, 0.95f, 1.1f, 1.0f );
	}
}
