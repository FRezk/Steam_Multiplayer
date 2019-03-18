// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"

void UMenuWidget::setup()
{
	this->AddToViewport();

	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;

	APlayerController* playerController = world->GetFirstPlayerController();

	if (!ensure(playerController != nullptr)) return;

	FInputModeUIOnly inputMode = FInputModeUIOnly::FInputModeUIOnly();
	inputMode.SetWidgetToFocus(this->TakeWidget());
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	playerController->SetInputMode(inputMode);
	playerController->bShowMouseCursor = true;
}

void UMenuWidget::teardown()
{
	this->RemoveFromViewport();

	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;

	APlayerController* playerController = world->GetFirstPlayerController();
	if (!ensure(playerController != nullptr)) return;

	playerController->SetInputMode(FInputModeGameOnly::FInputModeGameOnly());
	playerController->bShowMouseCursor = false;
}

void UMenuWidget::setMenuInterface(IMenuInterface* menuInterface)
{
	this->menuInterface = menuInterface;
}