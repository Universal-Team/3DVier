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

#include "coreCommon.hpp"

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
	std::pair<GameResult, uint8_t> Playable(uint8_t slot, uint8_t player) const;
	void Play(uint8_t slot, uint8_t player);

	/* Some utility getter's. */
	uint8_t GetChip(uint8_t slot) const;
	uint8_t GetChipPos(uint8_t row, uint8_t slot) const;
	uint8_t GetAvailableChips(uint8_t player) const;
	uint8_t GetCurrentPlayer() const;
	uint8_t GetWins(uint8_t player) const;
	uint8_t GetWinAmount() const;

	/* Setter. */
	void SetCurrentPlayer(uint8_t player);

	/* MiniMax Helpers. */
	int MiniMaxHelper(uint8_t row, uint8_t slot) const;
	int MiniMaxRowHelper(uint8_t slot, uint8_t player) const;
	void SetChip(uint8_t row, uint8_t slot, uint8_t player);

	/* End Logic. */
	bool IsOver() const;
	std::pair<bool, ChipMatches> CheckMatches(uint8_t player) const;

	int AIPlay(AIMethods method = AIMethods::ABNegaMax);

	/* To transfer the uint8_t's gameData to another console, in case for multi-player. */
	uint8_t *ReturnGameData() const { return this->gameData.get(); }
private:
	std::unique_ptr<uint8_t[]> gameData = nullptr; // Game data.

	/* Our matches field. */
	const std::vector<ChipMatches> posMatches = {
		/* -6 Right to Left up. */
		{35, 29, 23, 17},
		{36, 30, 24, 18},
		{37, 31, 25, 19},
		{38, 32, 26, 20},

		/* -6 Right to Left up. */
		{28, 22, 16, 10},
		{29, 23, 17, 11},
		{30, 24, 18, 12},
		{31, 25, 19, 13},

		/* -6 Right to Left up. */
		{21, 15, 9, 3},
		{22, 16, 10, 4},
		{23, 17, 11, 5},
		{24, 18, 12, 6},

		/* -8 Left to Right up. */
		{38, 30, 22, 14},
		{39, 31, 23, 15},
		{40, 32, 24, 16},
		{41, 33, 25, 17},

		/* -8 Left to Right up. */
		{31, 23, 15, 7},
		{32, 24, 16, 8},
		{33, 25, 17, 9},
		{34, 26, 18, 10},

		/* -8 Left to Right up. */
		{24, 16, 8, 0},
		{25, 17, 9, 1},
		{26, 18, 10, 2},
		{27, 19, 11, 3},

		/* +1. */
		{35, 36, 37, 38},
		{36, 37, 38, 39},
		{37, 38, 39, 40},
		{38, 39, 40, 41},

		{28, 29, 30, 31},
		{29, 30, 31, 32},
		{30, 31, 32, 33},
		{31, 32, 33, 34},

		{21, 22, 23, 24},
		{22, 23, 24, 25},
		{23, 24, 25, 26},
		{24, 25, 26, 27},

		{14, 15, 16, 17},
		{15, 16, 17, 18},
		{16, 17, 18, 19},
		{17, 18, 19, 20},

		{7, 8, 9, 10},
		{8, 9, 10, 11},
		{9, 10, 11, 12},
		{10, 11, 12, 13},

		{0, 1, 2, 3},
		{1, 2, 3, 4},
		{2, 3, 4, 5},
		{3, 4, 5, 6},

		/* To the high. First row. */
		{35, 28, 21, 14},
		{28, 21, 14, 7},
		{21, 14, 7, 0},

		/* Second row. */
		{36, 29, 22, 15},
		{29, 22, 15, 8},
		{22, 15, 8, 1},

		/* Third row. */
		{37, 30, 23, 16},
		{30, 23, 16, 9},
		{23, 16, 9, 2},

		/* Fourth row. */
		{38, 31, 24, 17},
		{31, 24, 17, 10},
		{24, 17, 10, 3},

		/* Fifth row. */
		{39, 32, 25, 18},
		{32, 25, 18, 11},
		{25, 18, 11, 4},

		/* Sixth row. */
		{40, 33, 26, 19},
		{33, 26, 19, 12},
		{26, 19, 12, 5},

		/* Seventh row. */
		{41, 34, 27, 20},
		{34, 27, 20, 13},
		{27, 20, 13, 6}
	};
};

#endif