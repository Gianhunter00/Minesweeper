// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputModel.h"
#include "CellStatus.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SGridPanel.h"

/**
 * 
 */

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
