// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "MinesweeperUI.h"

class FToolBarBuilder;
class FMenuBuilder;

class FMinesweeperEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	TSharedRef<SDockTab> OnSpawnNomad(const FSpawnTabArgs& InTabSpawnArgs);
	
private:

	void RegisterMenus();


private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<MinesweeperUI> MinesweeperEditorUI;
};
