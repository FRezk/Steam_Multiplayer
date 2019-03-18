// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "GameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMS_API UGameMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* ResumeInBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitInBtn;

	UFUNCTION()
	void quit();

	UFUNCTION()
	void resume();
	
};
