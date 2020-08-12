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

#include <3ds.h>
#include <vector>

// A "Helper" for the chips draw.
struct ChipIcn {
	int X;
	int Y;
};

struct ChipMatches {
	int Pos1;
	int Pos2;
	int Pos3;
	int Pos4;
};

class Game {
public:
	Game();
	// The current Player.
	const int currentPlayer() { return this->v_currentPlayer; }
	void currentPlayer(const int player) { this->v_currentPlayer = player; }
	// Clear the field for a new game.
	void clearField();
	// Set a chip to a specific pos.
	bool setChip(const int Position, const int Player);
	// Return the field.
	const std::vector<int> getField() const { return this->field; }
	// Check if chips matches.
	bool checkMatches(int Player);
	// Check if used.
	bool isUsed(const int Position) const;
	// Check if all are used.
	bool allUsed() const;
	// Get the Score / wins.
	const int getScore(int Player);
	// Return if chip can be dropped.
	bool canDrop(const int Position);
private:
	// Our matches field.
	const std::vector<ChipMatches> posMatches = {
		// +8 Right to Left up.
		{0, 8, 16, 24},
		{1, 9, 17, 25},
		{2, 10, 18, 26},
		{3, 11, 19, 27},
		// +8 Right to Left up.
		{7, 15, 23, 31},
		{8, 16, 24, 32},
		{9, 17, 25, 33},
		{10, 18, 26, 34},
		// +8 Right to Left up.
		{14, 22, 30, 38},
		{15, 23, 31, 39},
		{16, 24, 32, 40},
		{17, 25, 33, 41},
		// +6 Left to Right up.
		{3, 9, 15, 21},
		{4, 10, 16, 22},
		{5, 11, 17, 23},
		{6, 12, 18, 24},
		// +6 Left to Right up.
		{10, 16, 22, 28},
		{11, 17, 23, 29},
		{12, 18, 24, 30},
		{13, 19, 25, 31},
		// +6 Left to Right up.
		{17, 23, 29, 35},
		{18, 24, 30, 36},
		{19, 25, 31, 37},
		{20, 26, 32, 38},
		// +1
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
		// To the high. First row.
		{0, 7, 14, 21},
		{7, 14, 21, 28},
		{14, 21, 28, 35},
		// Second row.
		{1, 8, 15, 22},
		{8, 15, 22, 29},
		{15, 22, 29, 36},
		// Third row.
		{2, 9, 16, 23},
		{9, 16, 23, 30},
		{16, 23, 30, 37},
		// Fourth row.
		{3, 10, 17, 24},
		{10, 17, 24, 31},
		{17, 24, 31, 38},
		// Fifth row.
		{4, 11, 18, 25},
		{11, 18, 25, 32},
		{18, 25, 32, 39},
		// Sixth row.
		{5, 12, 19, 26},
		{12, 19, 26, 33},
		{19, 26, 33, 40},
		// Seventh row.
		{6, 13, 20, 27},
		{13, 20, 27, 34},
		{20, 27, 34, 41}
	};

	std::vector<int> field; // 42 max.
	int v_currentPlayer = 1;
	int count = 0; // Game over by 42.
	int v_p1Wins = 0, v_p2Wins = 0;
};

#endif