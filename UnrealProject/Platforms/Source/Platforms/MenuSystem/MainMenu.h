// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"

#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);
	void SetServerList(TArray<FString> names);
	void setSelectedIndex(uint32 index);

private:

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* hostBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* joinBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* exitBtn;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* menuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UButton* jBack;

	UPROPERTY(meta = (BindWidget))
	class UButton* jJoin;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	TSubclassOf<class UUserWidget> serverRowClass;

	UFUNCTION()
	void hostServer();

	UFUNCTION()
	void openJoinMenu();

	UFUNCTION()
	void back();

	UFUNCTION()
	void join();

	UFUNCTION()
	void exit();

	TOptional<uint32> selectedIndex;

protected:
	virtual bool Initialize() override;
	
};
