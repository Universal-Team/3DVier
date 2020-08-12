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

#include "common.hpp"
#include "config.hpp"

extern std::unique_ptr<Config> config;
int animated_chipYPos[2] = {0};
bool animated_chipMoveDelay = false;

void Animation::DrawSubBG(bool isTop) {
	if (!animated_chipMoveDelay) {
		animated_chipYPos[0] -= 2;
		animated_chipYPos[1] += 2;
		if (animated_chipYPos[0] <= -240) animated_chipYPos[0] = 0;
		if (animated_chipYPos[1] >= +240) animated_chipYPos[1] = 0;
	}

	animated_chipMoveDelay = !animated_chipMoveDelay;

	// BG.
	isTop ? Gui::ScreenDraw(Top) : Gui::ScreenDraw(Bottom);
	Gui::Draw_Rect(0, 25, isTop ? 400 : 320, 190, config->bgColor());
	// First Chips.
	GFX::DrawChip(20, animated_chipYPos[0], 1, 1,  1);
	GFX::DrawChip(20, animated_chipYPos[0]+240, 1, 1,  1);
	// Second Chips.
	GFX::DrawChip(120, animated_chipYPos[1], 1, 1,  2);
	GFX::DrawChip(120, animated_chipYPos[1]-240, 1, 1,  2);
	// Third Chips.
	GFX::DrawChip(220, animated_chipYPos[0], 1, 1,  1);
	GFX::DrawChip(220, animated_chipYPos[0]+240, 1, 1,  1);
	// Fourth Chips.
	GFX::DrawChip(320, animated_chipYPos[1], 1, 1,  2);
	GFX::DrawChip(320, animated_chipYPos[1]-240, 1, 1,  2);
	// Draw Bars with 225 Alpha, so it looks pretty nice.
	Gui::Draw_Rect(0, 0, isTop ? 400 : 320, 25, config->barColor() - C2D_Color32(0, 0, 0, 30));
	Gui::Draw_Rect(0, 215, isTop ? 400 : 320, 25, config->barColor() - C2D_Color32(0, 0, 0, 30));
	GFX::DrawSprite(isTop ? sprites_top_screen_top_idx : sprites_bottom_screen_top_idx, 0, 0);
	GFX::DrawSprite(isTop ? sprites_top_screen_bot_idx : sprites_bottom_screen_bot_idx, 0, 215);
}