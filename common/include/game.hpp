/*
*   This file is part of 3DVier
*   Copyright (C) 2020 Universal-Team
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#ifndef _3DVIER_GAME_HPP
#define _3DVIER_GAME_HPP

#include <array>
#include <cstdlib> // For uint8_t etc.
#include <memory>
#include <string>
#include <vector>

#ifdef _3DS
	const std::array<std::string, 3> Slots = {{
		"sdmc:/3ds/3DVier/slot1.slot",
		"sdmc:/3ds/3DVier/slot2.slot",
		"sdmc:/3ds/3DVier/slot3.slot"
	}};

#elif _NDS
	const std::array<std::string, 3> Slots = {{
		"sd:/_nds/DSVier/slot1.slot",
		"sd:/_nds/DSVier/slot2.slot",
		"sd:/_nds/DSVier/slot3.slot"
	}};

#else
	const std::array<std::string, 3> Slots = {{
		"3DVier/slot1.slot",
		"3DVier/slot2.slot",
		"3DVier/slot3.slot"
	}};
#endif

#define GAME_CURRENT_PLAYER 0x0

#define GAME_FIELD_START 0x1
#define GAME_FIELD_SIZE 0x2A

#define GAME_WIN_START 0x2B
#define GAME_WIN_SIZE 0x2

#define GAME_WIN_AMOUNT 0x2D

#define GAME_AVAILABLE_CHIPS_START 0x2E
#define GAME_AVAILABLE_CHIPS_SIZE 0x2

#define GAME_DATA_SIZE 0x30

enum class GameResult : uint8_t {
	Good,
	InvalidRow,
	AllUsed,
	NoChips,
	NoData
};

struct ChipMatches {
	uint8_t Pos1;
	uint8_t Pos2;
	uint8_t Pos3;
	uint8_t Pos4;
};

/*
	GameData Struct Notes:
		- currentPlayer[0x1]: 0x0.
		- fieldData[0x2A]: 0x1 - 0x2A. -- 42 fields.
		- wins[0x2]: 0x2B - 0x2C. -- of both players.
		- winAmount[0x1]: 0x2D.
		- availableChips[0x2]: 0x2E - 0x2F. -- of both players.
		- size[0x30] | 48 bytes.
*/

class Game {
public:
	Game(bool load = false, uint8_t slot = 0);
	~Game() { if (this->gameData) this->gameData = nullptr; }

	/* Load and save slot. */
	bool LoadSlot(uint8_t slot);
	void SaveSlot(uint8_t slot);

	/* Restarting stuff. */
	void StartFromScratch();
	void RestartGame();

	/* Playable checks and play. */
	std::pair<GameResult, uint8_t> Playable(uint8_t slot, uint8_t player);
	void Play(uint8_t slot, uint8_t player);

	/* Some utility getter's. */
	uint8_t GetChip(uint8_t slot);
	uint8_t GetAvailableChips(uint8_t player);
	uint8_t GetCurrentPlayer();
	uint8_t GetWins(uint8_t player);
	uint8_t GetWinAmount();

	/* Setter. */
	void SetCurrentPlayer(uint8_t player);

	/* End Logic. */
	bool IsOver();
	std::pair<bool, ChipMatches> CheckMatches(uint8_t player);

	/* To transfer the uint8_t's gameData to another console, in case for multi-player. */
	uint8_t *ReturnGameData() const { return this->gameData.get(); }
private:
	std::unique_ptr<uint8_t[]> gameData = nullptr; // Game data.

	/* Our matches field. */
	const std::vector<ChipMatches> posMatches = {
		/* +8 Right to Left up. */
		{0, 8, 16, 24},
		{1, 9, 17, 25},
		{2, 10, 18, 26},
		{3, 11, 19, 27},

		/* +8 Right to Left up. */
		{7, 15, 23, 31},
		{8, 16, 24, 32},
		{9, 17, 25, 33},
		{10, 18, 26, 34},

		/* +8 Right to Left up. */
		{14, 22, 30, 38},
		{15, 23, 31, 39},
		{16, 24, 32, 40},
		{17, 25, 33, 41},

		/* +6 Left to Right up. */
		{3, 9, 15, 21},
		{4, 10, 16, 22},
		{5, 11, 17, 23},
		{6, 12, 18, 24},

		/* +6 Left to Right up. */
		{10, 16, 22, 28},
		{11, 17, 23, 29},
		{12, 18, 24, 30},
		{13, 19, 25, 31},

		/* +6 Left to Right up. */
		{17, 23, 29, 35},
		{18, 24, 30, 36},
		{19, 25, 31, 37},
		{20, 26, 32, 38},

		/* +1. */
		{0, 1, 2, 3},
		{1, 2, 3, 4},
		{2, 3, 4, 5},
		{3, 4, 5, 6},
		{7, 8, 9 , 10},
		{8, 9, 10, 11},
		{9, 10, 11, 12},
		{10, 11, 12, 13},
		{14, 15, 16, 17},
		{15, 16, 17, 18},
		{16, 17, 18, 19},
		{17, 18, 19, 20},
		{21, 22, 23, 24},
		{22, 23, 24, 25},
		{23, 24, 25, 26},
		{24, 25, 26, 27},
		{28, 29, 30, 31},
		{29, 30, 31, 32},
		{30, 31, 32, 33},
		{31, 32, 33, 34},
		{35, 36, 37, 38},
		{36, 37, 38, 39},
		{37, 38, 39, 40},
		{38, 39, 40, 41},

		/* To the high. First row. */
		{0, 7, 14, 21},
		{7, 14, 21, 28},
		{14, 21, 28, 35},

		/* Second row. */
		{1, 8, 15, 22},
		{8, 15, 22, 29},
		{15, 22, 29, 36},

		/* Third row. */
		{2, 9, 16, 23},
		{9, 16, 23, 30},
		{16, 23, 30, 37},

		/* Fourth row. */
		{3, 10, 17, 24},
		{10, 17, 24, 31},
		{17, 24, 31, 38},

		/* Fifth row. */
		{4, 11, 18, 25},
		{11, 18, 25, 32},
		{18, 25, 32, 39},

		/* Sixth row. */
		{5, 12, 19, 26},
		{12, 19, 26, 33},
		{19, 26, 33, 40},

		/* Seventh row. */
		{6, 13, 20, 27},
		{13, 20, 27, 34},
		{20, 27, 34, 41}
	};
};

#endif