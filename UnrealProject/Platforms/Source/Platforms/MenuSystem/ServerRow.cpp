// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"
#include "MainMenu.h"
#include "Components/Button.h"

bool UServerRow::Initialize()
{
	bool success = Super::Initialize();
	if (!success) return false;

	if (!ensure(rowBtn != nullptr)) return false;

	rowBtn->OnClicked.AddDynamic(this, &UServerRow::OnClicked);

	return true;
}

void UServerRow::Setup(class UMainMenu* parent, uint32 index)
{
	this->parent = parent;
	this->index = index;
}

void UServerRow::OnClicked()
{
	if (!ensure(parent != nullptr)) return;

	parent->setSelectedIndex(index);
}
