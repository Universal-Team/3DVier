/*
*   This file is part of 3DVier
*   Copyright (C) 2020 DeadPhoenix8091, Epicpkmn11, Flame, RocketRobz, StackZ, TotallyNotGuy
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

#ifndef _3DVIER_GAMESELECT_MODE_HPP
#define _3DVIER_GAMESELECT_MODE_HPP

#include "common.hpp"
#include "structs.hpp"

#include <vector>

class GameSelect : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
private:
	int mainSelection = 0;
	int createSelection = 0;
	int typeSelection = 0;
	int Mode = 0;

	// Screen draws.
	void DrawFirstSelection(void) const;
	void DrawTypeSelection(void) const;
	void DrawCreateSelection(void) const;
	// Screen Logics.
	void firstLogic(u32 hDown, u32 hHeld, touchPosition touch);
	void typeLogic(u32 hDown, u32 hHeld, touchPosition touch);
	void createLogic(u32 hDown, u32 hHeld, touchPosition touch);

	std::vector<ButtonStruct> mainButtons = {
		{90, 60, 140, 40, "SINGLE_PLAYER"},
		{90, 130, 140, 40, "MULTI_PLAYER"}
	};

	std::vector<ButtonStruct> createMode = {
		{90, 60, 140, 40, "CREATE_ROOM"},
		{90, 130, 140, 40, "JOIN_ROOM"}
	};

	std::vector<ButtonStruct> typeBtn = {
		{90, 40, 140, 40, "SINGLE_CONSOLE"},
		{90, 100, 140, 40, "LOCAL_CONSOLE"},
		{90, 160, 140, 40, "ONLINE_CONSOLE"}
	};
};

#endif