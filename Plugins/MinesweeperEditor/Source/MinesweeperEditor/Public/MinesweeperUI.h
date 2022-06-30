// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SGridPanel.h"

/**
 * 
 */

enum ECellState
{
	ECellState_NotClicked,
	ECellState_Clicked
};

enum ECellFlag
{
	ECellFlag_NotFlagged,
	ECellFlag_Flagged
};

enum ECellType
{
	ECellType_NotBomb,
	ECellType_Bomb
};

struct InputData
{
public:
	inline bool IsDataValid() { return Width != 0 && Height != 0 && NumberOfBombs != 0; }
	inline bool IsNumberOfBombsValid() { return Width * Height > NumberOfBombs; }
	inline void Reset() { Width = 0; Height = 0; NumberOfBombs = 0; }
public:
	int32 Width;
	int32 Height;
	int32 NumberOfBombs;
};

struct MinesweeperCell
{
public:
	MinesweeperCell()
	{
		FlagState = ECellFlag::ECellFlag_NotFlagged;
		State = ECellState::ECellState_NotClicked;
		Type = ECellType::ECellType_NotBomb;
		AdjacentBombsNumber = 0;
		Checked = false;
	}
	MinesweeperCell(ECellType InType) : MinesweeperCell()
	{
		Type = InType;
	}
	bool Checked;
	int32 AdjacentBombsNumber;
	ECellFlag FlagState;
	ECellType Type;
	ECellState State;
	TArray<MinesweeperCell*> AdjacentCell;
};

class MINESWEEPEREDITOR_API MinesweeperUI
{
public:
	MinesweeperUI();
	~MinesweeperUI();

	TSharedRef<SDockTab> GetUI();


private:
	TSharedRef<SDockTab> CreateMinesweeperUI();
	FReply PopulateGrid();
	void PressedButton(int32 InIndex);
	FSlateColor ChangeColor(int32 InIndex) const;
	FText SetButtonText(int32 InIndex) const;
	void CheckUncheckFlaggedClick(ECheckBoxState InState);
	void CheckWinLose(int32 InIndex);


	void AssignWidth(int32 InValue);
	void AssignHeight(int32 InValue);
	void AssignNumberOfBomb(int32 InValue);

	void PopulateBombPositionArray();

	void SetAdjacentBombForAllCell();
	void CheckAndAddAdjacent(MinesweeperCell* CellToAddAdjacent, int32 InY, int32 InX);
	void CheckAdjacent(MinesweeperCell* InCellToCheck);
	void ResetCheckedState();

	void ShowAll();
	void CheckWin();

	void Reset();

	void OnDockTabClosing(TSharedRef<SDockTab> InTab);
private:
	bool FlaggedClick;
	bool Playing;
	TSharedPtr<SGridPanel> GridPanel;
	TArray<TSharedPtr<SButton>> ButtonCell;
	InputData GameData;
	TArray<int32> BombPosition;
	TArray<MinesweeperCell> MineCell;
	FText Lose;
	FText LoseTitle;
	FText Win;
	FText WinTitle;
	FText InputNotValid;
	FText InputNotValidTitle;
	FText Rules;
};
