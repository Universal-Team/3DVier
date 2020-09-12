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

#include "_3DVier_Helper.hpp"
#include "overlay.hpp"

static const std::vector<ChipIcn> FieldPos = {
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

static void DrawAnimDrop(int &dropPos) {
	int downSpeed = Settings::dropSpeed();

	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	GFX::DrawTop(false);
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.

	GFX::DrawSprite(sprites_bg_idx, 79, 16);
	_3DVier_Helper::DrawChip(FieldPos[3].X + 4, dropPos, 1);

	_3DVier_Helper::DrawField(79, 16);

	GFX::DrawBottom(true);

	/* Drop Anim speed. */
	Gui::DrawStringCentered(0, 30, 0.6, TEXT_COLOR, Lang::get("DROP_SPEED"));
	for (int i = 0; i < 9; i++) {
		if (i + 1 == Settings::dropSpeed()) Gui::drawAnimatedSelector(20 + 30 * i, 70, 30, 30, .030, SELECTED_BUTTON_COLOR, BUTTON_COLOR);
		else Gui::Draw_Rect(20 + 30 * i, 70, 30, 30, BUTTON_COLOR);

		Gui::DrawString(30 + i * 30, 74, 0.6, TEXT_COLOR, std::to_string(1 + i));
	}

	/* Clear Anim speed. */
	Gui::DrawStringCentered(0, 130, 0.6, TEXT_COLOR, Lang::get("CLEAR_SPEED"));
	for (int i = 0; i < 9; i++) {
		if (i + 1 == Settings::clearSpeed()) Gui::drawAnimatedSelector(20 + 30 * i, 170, 30, 30, .030, SELECTED_BUTTON_COLOR, BUTTON_COLOR);
		else Gui::Draw_Rect(20 + 30 * i, 170, 30, 30, BUTTON_COLOR);

		Gui::DrawString(30 + i * 30, 174, 0.6, TEXT_COLOR, std::to_string(1 + i));
	}

	C3D_FrameEnd(0);

	/* Logic for drop. */
	if (dropPos < FieldPos[3].Y + 4) {
		downSpeed = downSpeed * Settings::speedMultiplier();
		dropPos = dropPos + downSpeed;

	} else {
		dropPos = 5;
		downSpeed = Settings::dropSpeed();
	}
}

static void DrawClearAnim(int &pos) {
	int downSpeed = Settings::clearSpeed();

	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));

	GFX::DrawTop(false);
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.

	GFX::DrawSprite(sprites_bg_idx, 79, 16);

	for (int i = 0; i < 42; i++) {
		_3DVier_Helper::DrawChip(FieldPos[i].X + 4, FieldPos[i].Y + 4 + pos, 1);
	}

	_3DVier_Helper::DrawField(79, 16);

	GFX::DrawBottom(true);

	/* Drop Anim speed. */
	Gui::DrawStringCentered(0, 30, 0.6, TEXT_COLOR, Lang::get("DROP_SPEED"));
	for (int i = 0; i < 9; i++) {
		if (i + 1 == Settings::dropSpeed()) Gui::drawAnimatedSelector(20 + 30 * i, 70, 30, 30, .030, SELECTED_BUTTON_COLOR, BUTTON_COLOR);
		else Gui::Draw_Rect(20 + 30 * i, 70, 30, 30, BUTTON_COLOR);

		Gui::DrawString(30 + i * 30, 74, 0.6, TEXT_COLOR, std::to_string(1 + i));
	}
	
	/* Clear Anim speed. */
	Gui::DrawStringCentered(0, 130, 0.6, TEXT_COLOR, Lang::get("CLEAR_SPEED"));
	for (int i = 0; i < 9; i++) {
		if (i + 1 == Settings::clearSpeed()) Gui::drawAnimatedSelector(20 + 30 * i, 170, 30, 30, .030, SELECTED_BUTTON_COLOR, BUTTON_COLOR);
		else Gui::Draw_Rect(20 + 30 * i, 170, 30, 30, BUTTON_COLOR);

		Gui::DrawString(30 + i * 30, 174, 0.6, TEXT_COLOR, std::to_string(1 + i));
	}

	C3D_FrameEnd(0);

	/* Logic for clear. */
	if (FieldPos[42].Y + pos < 240) {
		downSpeed = downSpeed * Settings::speedMultiplier();
		pos = pos + downSpeed;

	} else {
		pos = 0;
		downSpeed = Settings::clearSpeed();
	}
}

void Overlays::SpeedOverlay() {
	bool doOut = false;
	int mode = 0, slMode = 0;
	int pos = 5;

	while(!doOut) {
		if (mode == 0) DrawAnimDrop(pos);
		else DrawClearAnim(pos);

		u32 hDown = hidKeysDown();
		hidScanInput();

		if (hDown & KEY_B) doOut = true;

		if (hDown & KEY_RIGHT) {
			if (slMode == 0) {
				if (Settings::dropSpeed() < 9) Settings::dropSpeed(Settings::dropSpeed() + 1);

			} else {
				if (Settings::clearSpeed() < 9) Settings::clearSpeed(Settings::clearSpeed() + 1);
			}
		}

		if (hDown & KEY_LEFT) {
			if (slMode == 0) {
				if (Settings::dropSpeed() > 0) Settings::dropSpeed(Settings::dropSpeed() - 1);

			} else {
				if (Settings::clearSpeed() > 0) Settings::clearSpeed(Settings::clearSpeed() - 1);
			}
		}

		if (hDown & KEY_UP) {
			if (slMode == 1) {
				slMode = 0;
				mode = 0;
			}
		}

		if (hDown & KEY_DOWN) {
			if (slMode == 0) {
				slMode = 1;
				mode = 1;
			}
		}
	}
}