// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

UPlatformsGameInstance::UPlatformsGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> menuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu")); 
	if (!ensure(menuBPClass.Class != nullptr)) return;
	menuClass = menuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> gameMenuBP(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(gameMenuBP.Class != nullptr)) return;
	gameMenuClass = gameMenuBP.Class;
}

void UPlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *menuClass->GetName());
}

void UPlatformsGameInstance::LoadMenu()
{
	if (!ensure(menuClass != nullptr)) return;
	menu = CreateWidget<UMainMenu>(this, menuClass);

	if (!ensure(menu != nullptr)) return;

	menu->setup();
	menu->setMenuInterface(this);

}

void UPlatformsGameInstance::InGameLoadMenu()
{
	if (!ensure(gameMenuClass != nullptr)) return;

	UMenuWidget* menu = CreateWidget<UMenuWidget>(this, gameMenuClass);
	if (!ensure(menu != nullptr)) return;

	menu->setup();
	menu->setMenuInterface(this);
}

void UPlatformsGameInstance::Host()
{
	if (menu != nullptr)
	{
		menu->teardown();
	}
	UEngine* engine = GetEngine();
	if (!ensure(engine != nullptr)) return;

	engine->AddOnScreenDebugMessage(0, 5, FColor::Green, TEXT("Hosting"));

	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;

	world->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");

}

void UPlatformsGameInstance::Join(const FString& address)
{
	if (menu != nullptr)
	{
		menu->teardown();
	}

	UEngine* engine = GetEngine();
	if (!ensure(engine != nullptr)) return;

	engine->AddOnScreenDebugMessage(0, 7, FColor::Yellow, FString::Printf(TEXT("Joining on server: %s"), *address));

	APlayerController* playerController = GetFirstLocalPlayerController();
	if (!ensure(playerController != nullptr)) return;

	playerController->ClientTravel(*address, ETravelType::TRAVEL_Absolute);
}

void UPlatformsGameInstance::LoadMainMenu()
{
	APlayerController* playerController = GetFirstLocalPlayerController();
	if (!ensure(playerController != nullptr)) return;

	playerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPlatformsGameInstance::exitGame()
{
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (!ensure(playerController != nullptr)) return;
	
	playerController->ConsoleCommand("quit");
}
