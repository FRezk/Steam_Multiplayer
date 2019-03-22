// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSessionInterface.h"

#include "PlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMS_API UPlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:

	UPlatformsGameInstance(const FObjectInitializer & ObjectInitializer);
	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();

	UFUNCTION(Exec)
	void Host() override;

	UFUNCTION(Exec)
	void Join(uint32 index) override;

	virtual void LoadMainMenu() override;
	virtual void exitGame() override;
	virtual void RefreshServerList() override;

private:
	TSubclassOf<class UUserWidget> menuClass;
	TSubclassOf<class UUserWidget> gameMenuClass;
	class UMainMenu* menu;

	IOnlineSessionPtr sessionInterface;
	TSharedPtr<class FOnlineSessionSearch> sessionSearch;
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void CreateSession();

};
