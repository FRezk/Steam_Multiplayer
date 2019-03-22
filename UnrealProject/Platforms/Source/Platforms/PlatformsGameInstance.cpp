// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "PlatformTrigger.h"
#include "MenuSystem/ServerRow.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("My Game Session");

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
	IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get();
	if (onlineSubsystem != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Found online subsystem: %s"), *onlineSubsystem->GetSubsystemName().ToString());
		sessionInterface = onlineSubsystem->GetSessionInterface();
		if (sessionInterface.IsValid()) {
			sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPlatformsGameInstance::OnCreateSessionComplete);
			sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPlatformsGameInstance::OnDestroySessionComplete);
			sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPlatformsGameInstance::OnFindSessionsComplete);
			sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPlatformsGameInstance::OnJoinSessionComplete);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Online Subsystem null!!"));
	}
	
}

void UPlatformsGameInstance::LoadMenuWidget()
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
	if (sessionInterface.IsValid()) 
	{
		auto existingSession = sessionInterface->GetNamedSession(SESSION_NAME);
		if (existingSession != nullptr) 
		{
			sessionInterface->DestroySession(SESSION_NAME);
		}
		else 
		{
			CreateSession();
		}
	}
}

	// Create Sessions
void UPlatformsGameInstance::CreateSession()
{
	if (sessionInterface.IsValid())
	{
		FOnlineSessionSettings sessionSettings;
		sessionSettings.bIsLANMatch = false;
		sessionSettings.NumPublicConnections = 5;
		sessionSettings.bShouldAdvertise = true;
		sessionSettings.bUsesPresence = true;
		sessionInterface->CreateSession(0, SESSION_NAME, sessionSettings);
	}
}

	// Find sessions
void UPlatformsGameInstance::RefreshServerList()
{
	sessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (sessionSearch.IsValid())
	{
		sessionSearch->MaxSearchResults = 100;
		sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Looking for sessions..."));
		sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
	}
}

void UPlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to create the session."));
		return;
	}

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

void UPlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UPlatformsGameInstance::OnFindSessionsComplete(bool Success)
{

	if (Success && sessionSearch.IsValid() && menu != nullptr) {
		TArray<FString> names;
		for (const FOnlineSessionSearchResult& session : sessionSearch->SearchResults) {
			names.Add(session.GetSessionIdStr());
			UE_LOG(LogTemp, Warning, TEXT("Found session %s"), *session.GetSessionIdStr());
		}
		menu->SetServerList(names);
	}
	
}

void UPlatformsGameInstance::Join(uint32 index)
{
	if (!sessionInterface.IsValid()) return;
	if (!sessionSearch.IsValid()) return;

	if (menu != nullptr)
	{
		menu->teardown();
	}

	sessionInterface->JoinSession(0, SESSION_NAME, sessionSearch->SearchResults[index]);

	
}

void UPlatformsGameInstance::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	if (!sessionInterface.IsValid()) return;

	FString address;
	if (!sessionInterface->GetResolvedConnectString(SESSION_NAME, address)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not retrieve connection."));
		return;
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
