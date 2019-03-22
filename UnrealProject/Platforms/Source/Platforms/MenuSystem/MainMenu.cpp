// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/EditableTextBox.h"

#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> serverRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(serverRowBPClass.Class != nullptr)) return;
	serverRowClass = serverRowBPClass.Class;
}

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
	if (menuInterface != nullptr)
	{
		menuInterface->RefreshServerList();
	}
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

void UMainMenu::setSelectedIndex(uint32 index)
{
	selectedIndex = index;
}

void UMainMenu::join()
{
	if (selectedIndex.IsSet() && menuInterface != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Selected index %d"), selectedIndex.GetValue());
		menuInterface->Join(selectedIndex.GetValue());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Index not set!!"));
	}
}

void UMainMenu::SetServerList(TArray<FString> names)
{
	UWorld* world = this->GetWorld();
	if (!ensure(world != nullptr)) return;

	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FString& serverName : names) {
		UServerRow* row = CreateWidget<UServerRow>(world, serverRowClass);
		if (!ensure(row != nullptr)) return;
		row->ServerName->SetText(FText::FromString(serverName));
		row->Setup(this, i);
		i++;
		ServerList->AddChild(row);
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
