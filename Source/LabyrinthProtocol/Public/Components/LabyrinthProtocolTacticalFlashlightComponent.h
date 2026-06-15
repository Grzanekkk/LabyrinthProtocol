// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LabyrinthProtocolTacticalFlashlightComponent.generated.h"

class USpotLightComponent;

UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class LABYRINTHPROTOCOL_API ULabyrinthProtocolTacticalFlashlightComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	ULabyrinthProtocolTacticalFlashlightComponent();

	void RegisterSpotLight( USpotLightComponent* InSpotLight );

	UFUNCTION( BlueprintCallable, Category = "Flashlight" )
	void ApplyFlashlightSettings();

	UFUNCTION( BlueprintCallable, Category = "Flashlight" )
	void SetFlashlightEnabled( bool bEnabled );

	UFUNCTION( BlueprintPure, Category = "Flashlight" )
	bool IsFlashlightEnabled() const { return bFlashlightEnabled; }

	USpotLightComponent* GetSpotLight() const { return SpotLight; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight" )
	TObjectPtr< USpotLightComponent > SpotLight;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Flashlight" )
	bool bFlashlightEnabled = true;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Flashlight", meta = ( ClampMin = "0.0", UIMin = "0.0" ) )
	float LightIntensity = 2800.0f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Flashlight" )
	FLinearColor LightColor = FLinearColor( 0.85f, 0.92f, 1.0f, 1.0f );

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Flashlight", meta = ( ClampMin = "1.0", UIMin = "1.0", ClampMax = "80.0", UIMax = "80.0" ) )
	float InnerConeAngle = 12.0f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Flashlight", meta = ( ClampMin = "1.0", UIMin = "1.0", ClampMax = "80.0", UIMax = "80.0" ) )
	float OuterConeAngle = 28.0f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Flashlight", meta = ( ClampMin = "100.0", UIMin = "100.0" ) )
	float AttenuationRadius = 3200.0f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Flashlight", meta = ( ClampMin = "0.0", UIMin = "0.0" ) )
	float VolumetricScatteringIntensity = 0.35f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Flashlight" )
	bool bCastShadows = true;
};
