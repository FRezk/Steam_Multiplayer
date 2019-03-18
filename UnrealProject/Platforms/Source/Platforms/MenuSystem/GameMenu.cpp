// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMenu.h"

#include "Components/Button.h"

bool UGameMenu::Initialize()
{
	
	bool success = Super::Initialize();
	if (!success) return false;

	if (!ensure(ResumeInBtn != nullptr)) return false;
	ResumeInBtn->OnClicked.AddDynamic(this, &UGameMenu::resume);

	if (!ensure(QuitInBtn != nullptr)) return false;
	QuitInBtn->OnClicked.AddDynamic(this, &UGameMenu::quit);

	return true;
}

void UGameMenu::quit()
{
	if (menuInterface != nullptr)
	{
		teardown();
		menuInterface->LoadMainMenu();
	}
}

void UGameMenu::resume()
{
	teardown();
}