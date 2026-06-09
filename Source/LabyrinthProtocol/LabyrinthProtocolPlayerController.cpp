// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabyrinthProtocolPlayerController.h"
#include "LabyrinthProtocolCharacter.h"
#include "UI/LabyrinthProtocolHUDWidget.h"
#include "Blueprint/UserWidget.h"

ALabyrinthProtocolPlayerController::ALabyrinthProtocolPlayerController()
{
	bShowMouseCursor = false;
}

void ALabyrinthProtocolPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if( IsLocalPlayerController() )
	{
		CreateHUDWidget();
	}
}

void ALabyrinthProtocolPlayerController::OnPossess( APawn* InPawn )
{
	Super::OnPossess( InPawn );

	if( IsLocalPlayerController() && HUDWidget == nullptr )
	{
		CreateHUDWidget();
	}
}

void ALabyrinthProtocolPlayerController::OnUnPossess()
{
	if( HUDWidget != nullptr )
	{
		HUDWidget->RemoveFromParent();
		HUDWidget = nullptr;
	}

	Super::OnUnPossess();
}

void ALabyrinthProtocolPlayerController::CreateHUDWidget()
{
	if( !IsLocalPlayerController() || HUDWidgetClass == nullptr || HUDWidget != nullptr )
	{
		return;
	}

	HUDWidget = CreateWidget< ULabyrinthProtocolHUDWidget >( this, HUDWidgetClass );
	if( HUDWidget != nullptr )
	{
		HUDWidget->AddToViewport( 0 );
	}
}
