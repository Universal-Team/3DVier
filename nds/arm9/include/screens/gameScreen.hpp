/*
*   This file is part of DSVier
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

#ifndef _DSVIER_GAME_SCREEN_HPP
#define _DSVIER_GAME_SCREEN_HPP

#include "game.hpp"
#include "gui.hpp"
#include "screenCommon.hpp"

#include "structs.hpp"
#include <vector>

struct ChipIcn {
	float X;
	float Y;
};

/* Game Results. */
enum class GameRes {
	AllUsed,
	NotOver,
	Over
};

class GameScreen : public Screen {
public:
	void Draw(void) const override;
	void Logic(u16 hDown, touchPosition touch) override;
	GameScreen();
private:
	std::unique_ptr<Game> currentGame = nullptr;

	std::string player1 = "Player 1", player2 = "Player 2";
	std::string GetName(int name) const;

	/* Sub Mode stuff. */
	int subSel = 0, subMode = 0;
	bool isSub = false, dropped = false;
	void displaySub(void) const;
	void subLogic(u16 hDown, touchPosition touch);

	/* Functions. */
	void clearGame();
	void destruct();
	void SetMatchingFour();
	void updateTexts(void) const;
	void MoveChip();
	void Refresh();
	void updateField();
	void hideField();

	/* Variables. */
	GameRes results = GameRes::NotOver; // Game is over or not.
	ChipMatches matches = {0, 0, 0, 0}; // Matching pos.
	int indexes[42] = {0}; // Indexes for the sprites.
	int selectorChip[2] = {0}; // Selectors.
	int matchingFour[4] = {0}; // For highlighting the matching fours.
	int dropChip = 0;
	int rowSelection = 3; // Row selection.
	int dropSelection = 3; // Drop Selection for previewing the positions.

	const std::vector<Structs::ButtonPos> subPosMain = {
		{80, 30, 88, 32, -1}, // Load a slot.
		{80, 80, 88, 32, -1}, // Save a slot.
		{80, 130, 88, 32, -1} // Exit game.
	};

	const std::vector<ChipIcn> GamePos = {
		/* First Row -> 0-6. */
		{26, 153.5},
		{55, 153.5},
		{84, 153.5},
		{113, 153.5},
		{142, 153.5},
		{171, 153.5},
		{200, 153.5},

		/* Second Row -> 7-13. */
		{26, 124.5},
		{55, 124.5},
		{84, 124.5},
		{113, 124.5},
		{142, 124.5},
		{171, 124.5},
		{200, 124.5},

		/* Third Row -> 14-20. */
		{26, 95.5},
		{55, 95.5},
		{84, 95.5},
		{113, 95.5},
		{142, 95.5},
		{171, 95.5},
		{200, 95.5},

		/* Fourth Row -> 21-27. */
		{26, 66.5},
		{55, 66.5},
		{84, 66.5},
		{113, 66.5},
		{142, 66.5},
		{171, 66.5},
		{200, 66.5},

		/* Fifth Row -> 28-35. */
		{26, 37.5},
		{55, 37.5},
		{84, 37.5},
		{113, 37.5},
		{142, 37.5},
		{171, 37.5},
		{200, 37.5},

		/* Sixth Row -> 36-42. */
		{26, 8.5},
		{55, 8.5},
		{84, 8.5},
		{113, 8.5},
		{142, 8.5},
		{171, 8.5},
		{200, 8.5}
	};
};

#endif