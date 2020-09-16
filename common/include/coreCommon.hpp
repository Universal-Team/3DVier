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

#ifndef _3DVIER_CORE_COMMON_HPP
#define _3DVIER_CORE_COMMON_HPP

#include <array>
#include <cstdlib> // For uint8_t etc.
#include <memory>
#include <string>
#include <vector>

#ifdef _NDS
	#include "flashcard.hpp"
#endif

#ifdef _3DS
	const std::array<std::string, 3> Slots = {{
		"sdmc:/3ds/3DVier/slot1.slot",
		"sdmc:/3ds/3DVier/slot2.slot",
		"sdmc:/3ds/3DVier/slot3.slot"
	}};

#elif _NDS
	const std::array<std::string, 3> Slots = {{
		sdFound() ? "sd:/_nds/DSVier/slot1.slot" : "fat:/_nds/DSVier/slot1.slot",
		sdFound() ? "sd:/_nds/DSVier/slot2.slot" : "fat:/_nds/DSVier/slot2.slot",
		sdFound() ? "sd:/_nds/DSVier/slot3.slot" : "fat:/_nds/DSVier/slot3.slot"
	}};

#else
	const std::array<std::string, 3> Slots = {{
		"/3DVier/slot1.slot",
		"/3DVier/slot2.slot",
		"/3DVier/slot3.slot"
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

/* The methods for the AI. */
enum class AIMethods {
	ABMiniMax,
	MiniMax,
	ABNegaMax,
	NegaMax
};

struct ChipMatches {
	uint8_t Pos1;
	uint8_t Pos2;
	uint8_t Pos3;
	uint8_t Pos4;
};

#define EMPTY 0
#define WIDTH 7
#define HEIGHT 6

const int Player2Wins = 1000000; // Player 2.
const int Player1Wins = -Player2Wins; // Player 1.

#endif