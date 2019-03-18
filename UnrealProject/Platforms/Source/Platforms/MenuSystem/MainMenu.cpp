// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

bool UMainMenu::Initialize()
{
	bool success = Super::Initialize();
	if (!success) return false;

	if (!ensure(hostBtn != nullptr)) return false;
	if (!ensure(joinBtn != nullptr)) return false;
	if (!ensure(exitBtn != nullptr)) return false;
	if (!ensure(jBack != nullptr)) return false;
	if (!ensure(jJoin != nullptr)) return false;

	hostBtn->OnClicked.AddDynamic(this, &UMainMenu::hostServer);
	joinBtn->OnClicked.AddDynamic(this, &UMainMenu::openJoinMenu);
	jBack->OnClicked.AddDynamic(this, &UMainMenu::back);
	jJoin->OnClicked.AddDynamic(this, &UMainMenu::join);
	exitBtn->OnClicked.AddDynamic(this, &UMainMenu::exit);

	return true;
}

void UMainMenu::openJoinMenu()
{
	if (!ensure(menuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;

	menuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::back()
{
	if (!ensure(MainMenu != nullptr)) return;

	menuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::hostServer()
{
	if (menuInterface != nullptr)
	{
		menuInterface->Host();
	}
}

void UMainMenu::join()
{
	if (!ensure(ipAddress != nullptr)) return;

	if (menuInterface != nullptr)
	{
		menuInterface->Join(*ipAddress->Text.ToString());
	}
}

void UMainMenu::exit()
{
	if (menuInterface != nullptr)
	{
		teardown();
		menuInterface->exitGame();
	}
}
