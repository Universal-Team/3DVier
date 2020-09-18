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

static void DrawAnimDrop(int &dropPos, int &frameCount, int &frameDrops, int &mode) {
	int downSpeed = Settings::dropSpeed();
	int speedPlus = Settings::speedPlus();
	

	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, TRANSPARENT);
	C2D_TargetClear(Bottom, TRANSPARENT);

	GFX::DrawTop(false);
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.

	GFX::DrawSprite(sprites_bg_idx, 79, 16);

	if (dropPos < FieldPos[3].Y + 4) {
		_3DVier_Helper::DrawChip(FieldPos[3].X + 4, dropPos, 1);
	}

	_3DVier_Helper::DrawField(79, 16);

	GFX::DrawBottom(true);

	/* Drop Anim speed. */
	Gui::DrawStringCentered(0, 30, 0.6, TEXT_COLOR, Lang::get("DROP_SPEED"));

	for (int i = 0; i < 9; i++) {
		if (mode == 0) {
			if (i + 1 == Settings::dropSpeed()) Gui::drawAnimatedSelector(20 + 30 * i, 50, 30, 30, .030, SELECTED_BUTTON_COLOR, BUTTON_COLOR);
			else Gui::Draw_Rect(20 + 30 * i, 50, 30, 30, BUTTON_COLOR);

		} else {
			Gui::Draw_Rect(20 + 30 * i, 50, 30, 30, BUTTON_COLOR);
		}
		
		Gui::DrawString(30 + i * 30, 55, 0.6, TEXT_COLOR, std::to_string(1 + i));
	}

	/* Per each 5 frame speed. */
	Gui::DrawStringCentered(0, 80, 0.6, TEXT_COLOR, Lang::get("SPEED_PLUS"));

	for (int i = 0; i < 9; i++) {
		/* If mode == 1, we'd display the selector here. */
		if (mode == 1) {
			if (i + 1 == Settings::speedPlus()) Gui::drawAnimatedSelector(20 + 30 * i, 100, 30, 30, .030, SELECTED_BUTTON_COLOR, BUTTON_COLOR);
			else Gui::Draw_Rect(20 + 30 * i, 100, 30, 30, BUTTON_COLOR);

		} else {
			Gui::Draw_Rect(20 + 30 * i, 100, 30, 30, BUTTON_COLOR);
		}

		Gui::DrawString(30 + i * 30, 105, 0.6, TEXT_COLOR, std::to_string(1 + i));
	}

	/* Clear Anim speed. */
	Gui::DrawStringCentered(0, 130, 0.6, TEXT_COLOR, Lang::get("CLEAR_SPEED"));

	for (int i = 0; i < 9; i++) {
		/* If mode == 2, we'd display the selector here. */
		if (mode == 2) {
			if (i + 1 == Settings::clearSpeed()) Gui::drawAnimatedSelector(20 + 30 * i, 150, 30, 30, .030, SELECTED_BUTTON_COLOR, BUTTON_COLOR);
			else Gui::Draw_Rect(20 + 30 * i, 150, 30, 30, BUTTON_COLOR);

		} else {
			Gui::Draw_Rect(20 + 30 * i, 150, 30, 30, BUTTON_COLOR);
		}

		Gui::DrawString(30 + i * 30, 155, 0.6, TEXT_COLOR, std::to_string(1 + i));
	}

	C3D_FrameEnd(0);

	/* Logic for drop. */
	if (dropPos < FieldPos[3].Y + 4) {
		frameCount++;

		if (frameCount == 5) {
			frameDrops += speedPlus;
			frameCount = 0;
		}

		dropPos = dropPos + downSpeed + frameDrops;
		gspWaitForVBlank();

	} else {
		downSpeed = Settings::dropSpeed();
		speedPlus = Settings::speedPlus();
		frameDrops = 0;
		frameCount = 0;
		dropPos = 2;
	}
}

static void DrawClearAnim(int &pos, int &frameCount, int &frameDrops, int &mode) {
	int downSpeed = Settings::clearSpeed();
	int speedPlus = Settings::speedPlus();

	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, TRANSPARENT);
	C2D_TargetClear(Bottom, TRANSPARENT);

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
		/* If mode == 0, we'd display the selector here. */
		if (mode == 0) {
			if (i + 1 == Settings::dropSpeed()) Gui::drawAnimatedSelector(20 + 30 * i, 50, 30, 30, .030, SELECTED_BUTTON_COLOR, BUTTON_COLOR);
			else Gui::Draw_Rect(20 + 30 * i, 50, 30, 30, BUTTON_COLOR);

		} else {
			Gui::Draw_Rect(20 + 30 * i, 50, 30, 30, BUTTON_COLOR);
		}

		Gui::DrawString(30 + i * 30, 55, 0.6, TEXT_COLOR, std::to_string(1 + i));
	}

	/* Per each 5 frame speed. */
	Gui::DrawStringCentered(0, 80, 0.6, TEXT_COLOR, Lang::get("SPEED_PLUS"));

	for (int i = 0; i < 9; i++) {
		/* If mode == 1, we'd display the selector here. */
		if (mode == 1) {
			if (i + 1 == Settings::speedPlus()) Gui::drawAnimatedSelector(20 + 30 * i, 100, 30, 30, .030, SELECTED_BUTTON_COLOR, BUTTON_COLOR);
			else Gui::Draw_Rect(20 + 30 * i, 100, 30, 30, BUTTON_COLOR);

		} else {
			Gui::Draw_Rect(20 + 30 * i, 100, 30, 30, BUTTON_COLOR);
		}

		Gui::DrawString(30 + i * 30, 105, 0.6, TEXT_COLOR, std::to_string(1 + i));
	}

	/* Clear Anim speed. */
	Gui::DrawStringCentered(0, 130, 0.6, TEXT_COLOR, Lang::get("CLEAR_SPEED"));
	
	for (int i = 0; i < 9; i++) {
		/* If mode == 2, we'd display the selector here. */
		if (mode == 2) {
			if (i + 1 == Settings::clearSpeed()) Gui::drawAnimatedSelector(20 + 30 * i, 150, 30, 30, .030, SELECTED_BUTTON_COLOR, BUTTON_COLOR);
			else Gui::Draw_Rect(20 + 30 * i, 150, 30, 30, BUTTON_COLOR);

		} else {
			Gui::Draw_Rect(20 + 30 * i, 150, 30, 30, BUTTON_COLOR);
		}

		Gui::DrawString(30 + i * 30, 155, 0.6, TEXT_COLOR, std::to_string(1 + i));
	}

	C3D_FrameEnd(0);

	/* Logic for clear. */
	if (FieldPos[42].Y + pos < 240) {
		frameCount++;

		if (frameCount == 5) {
			frameDrops += speedPlus;
			frameCount = 0;
		}

		pos = pos + downSpeed + frameDrops;
		gspWaitForVBlank();

	} else {
		downSpeed = Settings::clearSpeed();
		speedPlus = Settings::speedPlus();
		frameDrops = 0;
		frameCount = 0;
		pos = 0;
	}
}

void Overlays::SpeedOverlay() {
	bool doOut = false;
	int mode = 0, slMode = 0;
	int pos = 5;
	int frameCount = 0, frameDrops = 0;

	while(!doOut) {
		if (mode == 0) DrawAnimDrop(pos, frameCount, frameDrops, slMode);
		else DrawClearAnim(pos, frameCount, frameDrops, slMode);

		u32 hDown = hidKeysDown();
		u32 hRepeat = hidKeysDownRepeat();
		hidScanInput();

		if (hDown & KEY_B) doOut = true;

		if (hRepeat & KEY_RIGHT) {
			if (slMode == 0) {
				if (Settings::dropSpeed() < 9) Settings::dropSpeed(Settings::dropSpeed() + 1);

			} else if (slMode == 1) {
				if (Settings::speedPlus() < 9) Settings::speedPlus(Settings::speedPlus() + 1);

			} else {
				if (Settings::clearSpeed() < 9) Settings::clearSpeed(Settings::clearSpeed() + 1);
			}
		}

		if (hRepeat & KEY_LEFT) {
			if (slMode == 0) {
				if (Settings::dropSpeed() > 0) Settings::dropSpeed(Settings::dropSpeed() - 1);

			} else if (slMode == 1) {
				if (Settings::speedPlus() > 0) Settings::speedPlus(Settings::speedPlus() - 1);

			} else {
				if (Settings::clearSpeed() > 0) Settings::clearSpeed(Settings::clearSpeed() - 1);
			}
		}

		if (hRepeat & KEY_UP) {
			if (slMode > 0) {
				slMode--;
				if (slMode == 0) mode = 0;
			}
		}

		if (hRepeat & KEY_DOWN) {
			if (slMode < 2) {
				slMode++;

				if (slMode == 2) mode = 1;
			}
		}
	}
}