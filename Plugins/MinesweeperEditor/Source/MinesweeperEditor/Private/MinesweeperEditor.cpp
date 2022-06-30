// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperEditor.h"
#include "MinesweeperEditorStyle.h"
#include "MinesweeperEditorCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName MinesweeperEditorTabName("MinesweeperEditor");

#define LOCTEXT_NAMESPACE "FMinesweeperEditorModule"

void FMinesweeperEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMinesweeperEditorStyle::Initialize();
	FMinesweeperEditorStyle::ReloadTextures();

	FMinesweeperEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMinesweeperEditorCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FMinesweeperEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMinesweeperEditorModule::RegisterMenus));

	MinesweeperEditorUI = MakeShared<MinesweeperUI>();

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MinesweeperEditorTabName, FOnSpawnTab::CreateRaw(this, &FMinesweeperEditorModule::OnSpawnNomad));

}

void FMinesweeperEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MinesweeperEditorTabName);

	MinesweeperEditorUI.Reset();

	MinesweeperEditorUI = nullptr;

	FMinesweeperEditorStyle::Shutdown();

	FMinesweeperEditorCommands::Unregister();
}

void FMinesweeperEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperEditorTabName);
}

TSharedRef<SDockTab> FMinesweeperEditorModule::OnSpawnNomad(const FSpawnTabArgs& InTabSpawnArgs)
{
	return MinesweeperEditorUI->GetUI();
}

void FMinesweeperEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMinesweeperEditorCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMinesweeperEditorCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMinesweeperEditorModule, MinesweeperEditor)