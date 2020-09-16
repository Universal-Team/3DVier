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

void GFX::DrawTop(bool useBars) {
	Gui::ScreenDraw(Top);
	if (useBars) {
		Gui::Draw_Rect(0, 0, 400, 25, BAR_COLOR);
		Gui::Draw_Rect(0, 25, 400, 190, BG_COLOR);
		Gui::Draw_Rect(0, 215, 400, 25, BAR_COLOR);

	} else {
		Gui::Draw_Rect(0, 0, 400, 240, BG_COLOR);
	}
}

void GFX::DrawBottom(bool useBars) {
	Gui::ScreenDraw(Bottom);
	if (useBars) {
		Gui::Draw_Rect(0, 0, 320, 25, BAR_COLOR);
		Gui::Draw_Rect(0, 25, 320, 190, BG_COLOR);
		Gui::Draw_Rect(0, 215, 320, 25, BAR_COLOR);

	} else {
		Gui::Draw_Rect(0, 0, 320, 240, BG_COLOR);
	}
}

extern C2D_SpriteSheet characters, sprites;

void GFX::DrawSprite(int index, int x, int y, float ScaleX, float ScaleY) {
	Gui::DrawSprite(sprites, index, x, y, ScaleX, ScaleY);
}

/* Player Character. */
void GFX::DrawChar(int image, int x, int y, float ScaleX, float ScaleY) {
	Gui::DrawSprite(characters, image, x, y, ScaleX, ScaleY);
}

void GFX::DrawPlayer(int x, int y, float ScaleX, float ScaleY, int player) {
	switch (player) {
		case 0:
			DrawChar(chars_char1_idx, x, y, ScaleX, ScaleY);
			break;

		case 1:
			DrawChar(chars_char2_idx, x, y, ScaleX, ScaleY);
			break;

		case 2:
			DrawChar(chars_char3_idx, x, y, ScaleX, ScaleY);
			break;

		case 3:
			DrawChar(chars_char4_idx, x, y, ScaleX, ScaleY);
			break;

		case 4:
			DrawChar(chars_char5_idx, x, y, ScaleX, ScaleY);
			break;

		case 5:
			DrawChar(chars_char6_idx, x, y, ScaleX, ScaleY);
			break;

		case 6:
			DrawChar(chars_char7_idx, x, y, ScaleX, ScaleY);
			break;

		case 7:
			DrawChar(chars_char8_idx, x, y, ScaleX, ScaleY);
			break;
	}
}

void GFX::Button(const ButtonStruct btn) {
	Gui::Draw_Rect(btn.X, btn.Y, btn.xSize, btn.ySize, BUTTON_COLOR);
	Gui::DrawStringCentered(btn.X - 160 + (btn.xSize / 2), btn.Y + (btn.ySize / 2) - 10, 0.6f, TEXT_COLOR, btn.Text, btn.X - 10, btn.Y - 5);
}