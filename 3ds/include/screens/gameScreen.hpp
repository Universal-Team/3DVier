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

#ifndef _3DVIER_GAME_SCREEN_HPP
#define _3DVIER_GAME_SCREEN_HPP

#include "_3DVier_Helper.hpp"
#include "common.hpp"
#include "game.hpp"
#include "structs.hpp"

#include <vector>

/* Game Results. */
enum class GameRes {
	AllUsed,
	NotOver,
	Over
};

class GameScreen : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	GameScreen();
private:
	/* Sub Mode stuff. */
	int subSel = 0, subMode = 0;
	bool isSub = false, dropped = false;
	void displaySub(void) const;
	void subLogic(u32 hDown, u32 hHeld, touchPosition touch);

	GameRes results = GameRes::NotOver; // Someone won?
	ChipMatches matches = {0, 0, 0, 0}; // Matching pos.
	std::unique_ptr<Game> currentGame; // Our game pointer.
	int rowSelection = 3; // To select the Row.
	int dropSelection = 0; // Where to drop.
	void Refresh(); // Refresh the dropSelection.

	/* Player Names etc. */
	int avatar1, avatar2, winAmount;
	std::string p1Name, p2Name;
	std::string getName(int Player) const;
	int getAvatar(int Player) const;

	void drop();
	void clearField();
	
	const std::vector<Structs::ButtonPos> subBtn = {
		{90, 40, 140, 40},
		{90, 100, 140, 40},
		{90, 160, 140, 40}
	};

	const std::vector<ChipIcn> GamePos = {
		/* First Row -> 0-6. */
		{79, 186},
		{113, 186},
		{147, 186},
		{181, 186},
		{215, 186},
		{249, 186},
		{283, 186},

		/* Second Row -> 7-13. */
		{79, 152},
		{113, 152},
		{147, 152},
		{181, 152},
		{215, 152},
		{249, 152},
		{283, 152},

		/* Third Row -> 14-20. */
		{79, 118},
		{113, 118},
		{147, 118},
		{181, 118},
		{215, 118},
		{249, 118},
		{283, 118},

		/* Fourth Row -> 21-27. */
		{79, 84},
		{113, 84},
		{147, 84},
		{181, 84},
		{215, 84},
		{249, 84},
		{283, 84},

		/* Fifth Row -> 28-35. */
		{79, 50},
		{113, 50},
		{147, 50},
		{181, 50},
		{215, 50},
		{249, 50},
		{283, 50},

		/* Sixth Row -> 36-42. */
		{79, 16},
		{113, 16},
		{147, 16},
		{181, 16},
		{215, 16},
		{249, 16},
		{283, 16}
	};
};

#endif