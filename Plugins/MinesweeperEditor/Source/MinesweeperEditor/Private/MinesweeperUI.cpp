// Fill out your copyright notice in the Description page of Project Settings.


#include "MinesweeperUI.h"

#define LOCTEXT_NAMESPACE "FMinesweeperUI"

MinesweeperUI::MinesweeperUI()
{
	Lose = FText(LOCTEXT("Lose","You found a bomb!!"));
	
	LoseTitle = FText(LOCTEXT("LoseTitle", "YOU LOST"));

	Win = FText(LOCTEXT("Win", "You dodged every bomb!!"));

	WinTitle = FText(LOCTEXT("WinTitle", "YOU WON"));

	InputNotValid = FText(LOCTEXT("InputNotValid","Check: Width/Height/Number Of Bombs can't be 0 AND Number Of Bombs must be less then Width * Height"));
	
	InputNotValidTitle = FText(LOCTEXT("InputNotValidTitle", "INPUT NOT VALID"));

	Rules = FText(LOCTEXT("Rules", "RULES: HOW TO PLAY\n\n\n"
										 "Choose the width, height and number of bombs you want for the level.\n\n"
										 "Then press create grid to create the level.\n\n"
										 "Clicking on the button will show\n"
										 "whether or not you are clicking on a bomb or on an empty space.\n\n"
										 "The goal is too clear the grid without clicking a bomb.\n\n"
										 "If you are insecure on a cell you can check the \"Apply Flag?\" checkbox\n\n"
										 "Then click on the cell you are insecure\n\n"
										 "The color will be different for that cell\n\n"
										 "You cannot show that cell unless you free it\n\n"
										 "Reclicking the cell when the \"Apply Flag?\" checkbox is on will free the cell.\n\n"
										 "GOOD GAME"));
	GameData.Playing = true;
	GameData.FlaggedClick = false;
	GameData.Reset();
}

MinesweeperUI::~MinesweeperUI()
{
}

TSharedRef<SDockTab> MinesweeperUI::GetUI()
{
	return CreateMinesweeperUI();
}

TSharedRef<SDockTab> MinesweeperUI::CreateMinesweeperUI()
{
	return SNew(SDockTab).TabRole(ETabRole::NomadTab).OnTabClosed_Raw(this, &MinesweeperUI::OnDockTabClosing)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().HAlign(EHorizontalAlignment::HAlign_Left)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot().AutoHeight().Padding(0, 20)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().AutoWidth().VAlign(EVerticalAlignment::VAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText(LOCTEXT("TextBlockWidth","Width:")))
					]
					+ SHorizontalBox::Slot().MaxWidth(200).Padding(50, 0)
					[
						SNew(SSpinBox<int32>)
						.MinSliderValue(0)
						.MaxSliderValue(15)
						.OnValueChanged_Raw(this, &MinesweeperUI::AssignWidth)
					]
				]
				+SVerticalBox::Slot().AutoHeight().Padding(0, 20)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot().AutoWidth().VAlign(EVerticalAlignment::VAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText(LOCTEXT("TextBlockHeight", "Height:")))
					]
					+ SHorizontalBox::Slot().MaxWidth(200).Padding(50, 0)
					[
						SNew(SSpinBox<int32>)
						.MinSliderValue(0)
						.MaxSliderValue(15)
						.OnValueChanged_Raw(this, &MinesweeperUI::AssignHeight)
					]
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0,20)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot().AutoWidth().VAlign(EVerticalAlignment::VAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText(LOCTEXT("TextBlockNumberOfBombs", "Number Of Bombs:")))
					]
					+ SHorizontalBox::Slot().MaxWidth(200).Padding(50, 0)
					[
						SNew(SSpinBox<int32>)
						.MinSliderValue(0)
						.MaxSliderValue(50)
						.OnValueChanged_Raw(this, &MinesweeperUI::AssignNumberOfBomb)
					]
				]
				+ SVerticalBox::Slot().AutoHeight().Padding(0, 40)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot().MaxWidth(200)
					[
						SNew(SButton)
						.Text(FText(LOCTEXT("GenerateNewGrid", "Generate new grid")))
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Center)
						.ForegroundColor(FLinearColor(1, 1, 1))
						.OnClicked_Raw(this, &MinesweeperUI::PopulateGrid)
					]
					+ SHorizontalBox::Slot().AutoWidth().Padding(10,0).VAlign(EVerticalAlignment::VAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText(LOCTEXT("ApplyFlagOnClick", "Apply Flag On Click?")))
					]
					+SHorizontalBox::Slot().AutoWidth().Padding(4,0)
					[
						SNew(SCheckBox)
						.OnCheckStateChanged_Raw(this, &MinesweeperUI::CheckUncheckFlaggedClick)
					]

				]
				+SVerticalBox::Slot().AutoHeight().Padding(0, 40)
				[
					SAssignNew(GridPanel, SGridPanel).Clipping(EWidgetClipping::ClipToBounds)
				]
			]
			+ SHorizontalBox::Slot().AutoWidth().HAlign(EHorizontalAlignment::HAlign_Left)
			[
				SNew(STextBlock)
				.Text(Rules)
			]
		];
}

void MinesweeperUI::AssignWidth(int32 InValue)
{
	GameData.Width = InValue;
}

void MinesweeperUI::AssignHeight(int32 InValue)
{
	GameData.Height = InValue;
}

void MinesweeperUI::AssignNumberOfBomb(int32 InValue)
{
	GameData.NumberOfBombs = InValue;
}

FReply MinesweeperUI::PopulateGrid()
{
	if (GameData.IsDataValid() && GameData.IsNumberOfBombsValid())
	{
		Reset();

		PopulateBombPositionArray();

		ButtonCell.SetNum(GameData.Height * GameData.Width);
		MineCell.SetNum(GameData.Height * GameData.Width);

		int CurrentCellIndex = 0;

		for (int32 Height = 0; Height < GameData.Height; Height++)
		{
			for (int32 Width = 0; Width < GameData.Width; Width++)
			{
				CurrentCellIndex = Height * GameData.Width + Width;

				MineCell[CurrentCellIndex] = MinesweeperCell(BombPosition.Contains(CurrentCellIndex) ? ECellType::ECellType_Bomb : ECellType::ECellType_NotBomb);

				GridPanel->AddSlot(Width, Height).AttachWidget(
					SNew(SBox).WidthOverride(20).HeightOverride(20)
					[
						SAssignNew(ButtonCell[CurrentCellIndex],SButton)
						.ButtonColorAndOpacity_Raw(this, &MinesweeperUI::ChangeColor, CurrentCellIndex)
						.OnPressed_Raw(this, &MinesweeperUI::PressedButton, CurrentCellIndex)
						.OnReleased_Raw(this, &MinesweeperUI::CheckWinLose, CurrentCellIndex)
						.Text_Raw(this, &MinesweeperUI::SetButtonText, CurrentCellIndex)
						
					]
				);
			}
		}
		SetAdjacentBombForAllCell();
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, InputNotValid, &InputNotValidTitle);
	}
	return FReply::Handled();
}

void MinesweeperUI::CheckWinLose(int32 InIndex)
{
	if (MineCell[InIndex].FlagState == ECellFlag::ECellFlag_Flagged || GameData.FlaggedClick || !GameData.Playing)
	{
		return;
	}

	if (MineCell[InIndex].Type == ECellType::ECellType_Bomb)
	{
		FMessageDialog::Open(EAppMsgType::Ok, Lose, &LoseTitle);
		GameData.Playing = false;
		ShowAll();
	}
	else
	{
		CheckWin();
	}
	
}

void MinesweeperUI::ResetCheckedState()
{
	for (int32 CellNumber = 0; CellNumber < MineCell.Num(); CellNumber++)
	{
		MineCell[CellNumber].Checked = false;
	}
}

void MinesweeperUI::ShowAll()
{
	for (int32 CellNumber = 0; CellNumber < MineCell.Num(); CellNumber++)
	{
		if (MineCell[CellNumber].Type == ECellType::ECellType_Bomb)
		{
			MineCell[CellNumber].State = ECellState::ECellState_Clicked;
			MineCell[CellNumber].FlagState = ECellFlag::ECellFlag_NotFlagged;
		}
	}
}

void MinesweeperUI::CheckWin()
{
	int CountClicked = 0;
	for (int32 CellNumber = 0; CellNumber < MineCell.Num(); CellNumber++)
	{
		if (MineCell[CellNumber].Type == ECellType::ECellType_NotBomb && MineCell[CellNumber].State == ECellState::ECellState_Clicked)
		{
			CountClicked++;
		}
	}
	if (CountClicked == (GameData.Height * GameData.Width) - GameData.NumberOfBombs)
	{
		FMessageDialog::Open(EAppMsgType::Ok, Win, &WinTitle);
		GameData.Playing = false;
	}
}

FText MinesweeperUI::SetButtonText(int32 InIndex) const
{
	FText ButtonText;
	if (MineCell[InIndex].AdjacentBombsNumber != 0 && MineCell[InIndex].State == ECellState::ECellState_Clicked)
	{
		ButtonText = FText::Format(LOCTEXT("AdjacentBomb", "{0}"), MineCell[InIndex].AdjacentBombsNumber);
	}
	return ButtonText;
}

void MinesweeperUI::CheckUncheckFlaggedClick(ECheckBoxState InState)
{
	GameData.FlaggedClick = InState == ECheckBoxState::Checked;
}

void MinesweeperUI::SetAdjacentBombForAllCell()
{
	int CurrentCellIndex = 0;
	for (int32 Height = 0; Height < GameData.Height; Height++)
	{
		for (int32 Width = 0; Width < GameData.Width; Width++)
		{
			CurrentCellIndex = Height * GameData.Width + Width;
			if (MineCell[CurrentCellIndex].Type == ECellType::ECellType_Bomb)
			{
				continue;
			}
			CheckAndAddAdjacent(&MineCell[CurrentCellIndex], Height, Width + 1);
			CheckAndAddAdjacent(&MineCell[CurrentCellIndex], Height, Width - 1);

			CheckAndAddAdjacent(&MineCell[CurrentCellIndex], Height + 1, Width);
			CheckAndAddAdjacent(&MineCell[CurrentCellIndex], Height - 1, Width);

			CheckAndAddAdjacent(&MineCell[CurrentCellIndex], Height - 1, Width + 1);
			CheckAndAddAdjacent(&MineCell[CurrentCellIndex], Height - 1, Width - 1);

			CheckAndAddAdjacent(&MineCell[CurrentCellIndex], Height + 1, Width + 1);
			CheckAndAddAdjacent(&MineCell[CurrentCellIndex], Height + 1, Width - 1);
		}
	}
}

void MinesweeperUI::CheckAdjacent(MinesweeperCell* InCellToCheck)
{
	if (InCellToCheck->State == ECellState::ECellState_NotClicked && 
		InCellToCheck->FlagState == ECellFlag::ECellFlag_NotFlagged &&
		InCellToCheck->Type == ECellType::ECellType_NotBomb &&
		InCellToCheck->Checked == false)
	{
		if (InCellToCheck->AdjacentBombsNumber == 0)
		{
			for (int32 AdjacentCellNumber = 0; AdjacentCellNumber < InCellToCheck->AdjacentCell.Num(); AdjacentCellNumber++)
			{
				InCellToCheck->Checked = true;
				CheckAdjacent(InCellToCheck->AdjacentCell[AdjacentCellNumber]);
			}
		}
		InCellToCheck->State = ECellState::ECellState_Clicked;
	}
	InCellToCheck->Checked = true;
}

void MinesweeperUI::CheckAndAddAdjacent(MinesweeperCell* CellToAddAdjacent, int32 InY, int32 InX)
{
	if (InX >= 0 && InX < GameData.Width &&
		InY >= 0 && InY < GameData.Height)
	{
		MinesweeperCell* CellToAdd = &MineCell[InY * GameData.Width + InX];
		if (CellToAdd->Type == ECellType::ECellType_Bomb)
		{
			CellToAddAdjacent->AdjacentBombsNumber += 1;
		}
		CellToAddAdjacent->AdjacentCell.Add(&MineCell[InY * GameData.Width + InX]);

	}

}

void MinesweeperUI::PressedButton(int32 InIndex)
{
	if ((MineCell[InIndex].FlagState == ECellFlag::ECellFlag_Flagged && !GameData.FlaggedClick) || 
		MineCell[InIndex].State == ECellState::ECellState_Clicked || 
		!GameData.Playing)
	{
		return;
	}


	if (GameData.FlaggedClick)
	{
		if (MineCell[InIndex].FlagState == ECellFlag::ECellFlag_NotFlagged)
		{
			MineCell[InIndex].FlagState = ECellFlag::ECellFlag_Flagged;
		}
		else
		{
			MineCell[InIndex].FlagState = ECellFlag::ECellFlag_NotFlagged;
		}
		return;
	}

	TSharedPtr<SButton> ButtonClicked = ButtonCell[InIndex];

	if (MineCell[InIndex].Type == ECellType::ECellType_Bomb)
	{
		ButtonClicked->SetBorderBackgroundColor(FLinearColor(1, 0, 0));
	}
	else
	{
		ButtonClicked->SetBorderBackgroundColor(FLinearColor(0, 1, 0));
		CheckAdjacent(&MineCell[InIndex]);
		ResetCheckedState();
	}

}

FSlateColor MinesweeperUI::ChangeColor(int32 InIndex) const
{
	if (MineCell[InIndex].FlagState == ECellFlag::ECellFlag_Flagged)
	{
		return FSlateColor(FLinearColor(1, 1, 0));
	}
	if (MineCell[InIndex].State == ECellState::ECellState_Clicked)
	{
		if (MineCell[InIndex].Type == ECellType::ECellType_Bomb)
		{
			return FSlateColor(FLinearColor(1, 0, 0));
		}
		else
		{
			return FSlateColor(FLinearColor(0, 1, 0));
		}
	}
	return FSlateColor(FLinearColor(0.5, 0.5, 0.5));
}

void MinesweeperUI::PopulateBombPositionArray()
{
	int32 RandomNumber = 0; 
	for (int32 NumberOfbombs = 0; NumberOfbombs < GameData.NumberOfBombs; NumberOfbombs++)
	{
		do
		{
			RandomNumber = FMath::RandRange(0, (GameData.Height * GameData.Width) - 1);

		} while (BombPosition.Contains(RandomNumber));
		BombPosition.Add(RandomNumber);
	}
}

void MinesweeperUI::Reset() 
{
	if (GridPanel.IsValid())
	{
		GridPanel->ClearChildren();
	}
	MineCell.Empty();
	BombPosition.Empty();
	ButtonCell.Empty();
	GameData.Playing = true;
}

void MinesweeperUI::OnDockTabClosing(TSharedRef<SDockTab> InTab)
{
	Reset();
	GameData.Reset();
	GameData.FlaggedClick = false;
}


#undef LOCTEXT_NAMESPACE