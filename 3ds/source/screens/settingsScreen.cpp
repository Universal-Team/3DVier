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

#include "settingsScreen.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void SettingsScreen::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.8f, TEXT_COLOR, "3DVier - " + Lang::get("SETTINGS"));
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	GFX::DrawBottom();

	for (int i = 0; i < 2; i++) {
		if (this->Selection == i) Gui::drawAnimatedSelector(mainButtons[i].x, mainButtons[i].y, 140, 40, .030f, SELECTED_BUTTON_COLOR, BUTTON_COLOR);
		else Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, BUTTON_COLOR);
	}

	Gui::DrawStringCentered(0, mainButtons[0].y + 10, 0.6f, TEXT_COLOR, Lang::get("LANGUAGE"));
	Gui::DrawStringCentered(0, mainButtons[1].y + 10, 0.6f, TEXT_COLOR, Lang::get("ANIMATION"));

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}


void SettingsScreen::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack(true);
		return;
	}

	if (hDown & KEY_UP) {
		if (this->Selection > 0) this->Selection--;
	}
	
	if (hDown & KEY_DOWN) {
		if (this->Selection < 1) this->Selection++;
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			Msg::NotImplementedYet();

		} else if (touching(touch, mainButtons[1])) {
			Overlays::SpeedOverlay();
		}
	}

	if (hDown & KEY_A) {
		switch(this->Selection) {
			case 0:
				/* Language: TODO. */
				Msg::NotImplementedYet();
				break;

			case 1:
				Overlays::SpeedOverlay();
				break;
		}
	}
}