// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabyrinthProtocolPlayerController.h"
#include "LabyrinthProtocolCharacter.h"
#include "UI/LabyrinthProtocolHUDWidget.h"
#include "UI/LabyrinthProtocolHUDTypes.h"
#include "Blueprint/UserWidget.h"

ALabyrinthProtocolPlayerController::ALabyrinthProtocolPlayerController()
{
	bShowMouseCursor = false;
}

void ALabyrinthProtocolPlayerController::OnPossess( APawn* InPawn )
{
	Super::OnPossess( InPawn );

	if( HUDWidgetClass != nullptr && HUDWidget == nullptr && IsLocalPlayerController() )
	{
		HUDWidget = CreateWidget< ULabyrinthProtocolHUDWidget >( this, HUDWidgetClass );
		if( HUDWidget != nullptr )
		{
			HUDWidget->AddToViewport();
		}
	}

	BindToCharacter( Cast< ALabyrinthProtocolCharacter >( InPawn ) );
}

void ALabyrinthProtocolPlayerController::OnUnPossess()
{
	UnbindFromCharacter();
	Super::OnUnPossess();
}

void ALabyrinthProtocolPlayerController::BindToCharacter( ALabyrinthProtocolCharacter* Character )
{
	UnbindFromCharacter();

	BoundCharacter = Character;
	if( BoundCharacter == nullptr )
	{
		return;
	}

	BoundCharacter->OnHUDDataChanged.AddDynamic( this, &ALabyrinthProtocolPlayerController::HandleHUDDataChanged );
	BoundCharacter->InitializeHUDBindings();

	if( HUDWidget != nullptr )
	{
		HUDWidget->ApplyHUDData( BoundCharacter->BuildHUDViewData() );
	}
}

void ALabyrinthProtocolPlayerController::UnbindFromCharacter()
{
	if( BoundCharacter != nullptr )
	{
		BoundCharacter->OnHUDDataChanged.RemoveDynamic( this, &ALabyrinthProtocolPlayerController::HandleHUDDataChanged );
		BoundCharacter->ClearHUDBindings();
	}

	BoundCharacter = nullptr;
}

void ALabyrinthProtocolPlayerController::HandleHUDDataChanged( FLabyrinthProtocolHUDViewData HUDData )
{
	if( HUDWidget != nullptr )
	{
		HUDWidget->ApplyHUDData( HUDData );
	}
}
