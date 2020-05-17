/*
*   This file is part of 3DVier
*   Copyright (C) 2020 SuperSaiyajinStackZ
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

#include "common.hpp"
#include "game.hpp"
#include "structs.hpp"

#include <vector>

class GameScreen : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	GameScreen();
private:
	std::unique_ptr<Game> currentGame; // Our game pointer.
	int rowSelection = 3; // To select the Row.
	int dropSelection = 0; // Where to drop.
	void Refresh(); // Refresh the dropSelection.

	std::vector<ChipIcn> GamePos = {
		// First Row -> 0-6.
		{65, 205},
		{105, 205},
		{145, 205},
		{185, 205},
		{225, 205},
		{265, 205},
		{305, 205},
		// Second Row -> 7-13.
		{65, 165},
		{105, 165},
		{145, 165},
		{185, 165},
		{225, 165},
		{265, 165},
		{305, 165},
		// Third Row -> 14-20.
		{65, 125},
		{105, 125},
		{145, 125},
		{185, 125},
		{225, 125},
		{265, 125},
		{305, 125},
		// Fourth Row -> 21-27.
		{65, 85},
		{105, 85},
		{145, 85},
		{185, 85},
		{225, 85},
		{265, 85},
		{305, 85},
		// Fifth Row -> 28-35.
		{65, 45},
		{105, 45},
		{145, 45},
		{185, 45},
		{225, 45},
		{265, 45},
		{305, 45},
		// Sixth Row -> 36-42.
		{65, 5},
		{105, 5},
		{145, 5},
		{185, 5},
		{225, 5},
		{265, 5},
		{305, 5}
	};
};

#endif