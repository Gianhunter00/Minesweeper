// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MinesweeperEditorStyle.h"

class FMinesweeperEditorCommands : public TCommands<FMinesweeperEditorCommands>
{
public:

	FMinesweeperEditorCommands()
		: TCommands<FMinesweeperEditorCommands>(TEXT("MinesweeperEditor"), NSLOCTEXT("Contexts", "MinesweeperEditor", "MinesweeperEditor Plugin"), NAME_None, FMinesweeperEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> PluginAction;
};
