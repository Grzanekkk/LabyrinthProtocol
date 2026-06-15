// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LabyrinthProtocolNightLightingPreset.generated.h"

class UDirectionalLightComponent;
class UExponentialHeightFogComponent;
class UPostProcessComponent;
class USceneComponent;
class USkyAtmosphereComponent;
class USkyLightComponent;

UCLASS( Blueprintable )
class LABYRINTHPROTOCOL_API ALabyrinthProtocolNightLightingPreset : public AActor
{
	GENERATED_BODY()

public:
	ALabyrinthProtocolNightLightingPreset();

	UFUNCTION( BlueprintCallable, CallInEditor, Category = "Night Lighting" )
	void ApplyNightPreset();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction( const FTransform& Transform ) override;

	void ScheduleSkyLightRecapture();
	void RecaptureSkyLightDeferred();

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Night Lighting" )
	TObjectPtr< USceneComponent > SceneRoot;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Night Lighting" )
	TObjectPtr< UDirectionalLightComponent > MoonLight;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Night Lighting" )
	TObjectPtr< USkyLightComponent > SkyLight;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Night Lighting" )
	TObjectPtr< USkyAtmosphereComponent > SkyAtmosphere;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Night Lighting" )
	TObjectPtr< UExponentialHeightFogComponent > HeightFog;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Night Lighting" )
	TObjectPtr< UPostProcessComponent > PostProcess;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Night Lighting" )
	bool bApplyOnBeginPlay = true;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Night Lighting" )
	bool bRecaptureSkyLightOnApply = true;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Night Lighting|Directional Light", meta = ( ClampMin = "0.0", UIMin = "0.0" ) )
	float DirectionalLightIntensity = 2.5f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Night Lighting|Directional Light" )
	FLinearColor DirectionalLightColor = FLinearColor( 0.6f, 0.7f, 1.0f, 1.0f );

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Night Lighting|Directional Light" )
	FRotator DirectionalLightRotation = FRotator( -45.0f, 0.0f, 0.0f );

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Night Lighting|Sky Light", meta = ( ClampMin = "0.0", UIMin = "0.0" ) )
	float SkyLightIntensity = 1.2f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Night Lighting|Sky Light" )
	FLinearColor SkyLightColor = FLinearColor( 0.5f, 0.55f, 0.7f, 1.0f );

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Night Lighting|Sky Atmosphere", meta = ( ClampMin = "0.0", UIMin = "0.0" ) )
	float MieScatteringScale = 0.5f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Night Lighting|Sky Atmosphere" )
	FLinearColor SkyLuminanceFactor = FLinearColor( 0.3f, 0.35f, 0.55f, 1.0f );

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Night Lighting|Fog", meta = ( ClampMin = "0.0", UIMin = "0.0" ) )
	float FogDensity = 0.01f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Night Lighting|Fog", meta = ( ClampMin = "0.0", UIMin = "0.0" ) )
	float FogHeightFalloff = 0.2f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Night Lighting|Fog" )
	FLinearColor FogInscatteringColor = FLinearColor( 0.15f, 0.18f, 0.35f, 1.0f );

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Night Lighting|Fog" )
	bool bEnableVolumetricFog = false;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Night Lighting|Post Process" )
	float ExposureCompensation = -1.0f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Night Lighting|Post Process", meta = ( ClampMin = "0.0", UIMin = "0.0", ClampMax = "2.0", UIMax = "2.0" ) )
	float ColorSaturation = 0.8f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Night Lighting|Post Process", meta = ( ClampMin = "0.0", UIMin = "0.0", ClampMax = "2.0", UIMax = "2.0" ) )
	float ColorContrast = 1.05f;

private:
	FTimerHandle SkyLightRecaptureTimerHandle;
};
