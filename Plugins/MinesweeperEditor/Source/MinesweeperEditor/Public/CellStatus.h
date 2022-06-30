#pragma once


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