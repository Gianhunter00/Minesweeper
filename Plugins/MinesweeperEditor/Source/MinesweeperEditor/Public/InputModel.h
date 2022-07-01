#pragma once

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
	bool FlaggedClick;
	bool Playing;
};