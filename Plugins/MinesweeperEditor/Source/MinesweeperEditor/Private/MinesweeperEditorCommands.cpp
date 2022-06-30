// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperEditorCommands.h"

#define LOCTEXT_NAMESPACE "FMinesweeperEditorModule"

void FMinesweeperEditorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Minesweeper Game", "Start Minesweeper game", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
