// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextBlock.h"

#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;

	

	void Setup(class UMainMenu* parent, uint32 index);

private:

	UPROPERTY()
	class UMainMenu* parent;

	uint32 index;

	UPROPERTY(meta = (BindWidget))
		class UButton* rowBtn;

	UFUNCTION()
	void OnClicked();

protected:
	virtual bool Initialize() override;
};
