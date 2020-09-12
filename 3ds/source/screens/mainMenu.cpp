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

#include "GameScreen.hpp"
#include "mainMenu.hpp"
#include "settingsScreen.hpp"

extern bool exiting;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

void MainMenu::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.8f, TEXT_COLOR, "3DVier - " + Lang::get("MAINMENU"));
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	GFX::DrawBottom();

	for (int i = 0; i < 3; i++) {
		if (this->Selection == i) Gui::drawAnimatedSelector(mainButtons[i].x, mainButtons[i].y, 140, 40, .030f, SELECTED_BUTTON_COLOR, BUTTON_COLOR);
		else Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, BUTTON_COLOR);
	}

	Gui::DrawStringCentered(0, mainButtons[0].y + 10, 0.6f, TEXT_COLOR, Lang::get("NEW_GAME"));
	Gui::DrawStringCentered(0, mainButtons[1].y + 10, 0.6f, TEXT_COLOR, Lang::get("SETTINGS"));
	Gui::DrawStringCentered(0, mainButtons[2].y + 10, 0.6f, TEXT_COLOR, Lang::get("CREDITS"));

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}


void MainMenu::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_UP) {
		if (this->Selection > 0) this->Selection--;
	}
	
	if (hDown & KEY_DOWN) {
		if (this->Selection < 2) this->Selection++;
	}

	if (hDown & KEY_A) {
		switch(this->Selection) {
			case 0:
				Gui::setScreen(std::make_unique<GameScreen>(), true, true);
				break;

			case 1:
				/* Settings. */
				Gui::setScreen(std::make_unique<SettingsScreen>(), true, true);
				break;

			case 2:
				/* Credits. */
				Overlays::CreditsOverlay();
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			Gui::setScreen(std::make_unique<GameScreen>(), true, true);

		} else if (touching(touch, mainButtons[1])) {
			Gui::setScreen(std::make_unique<SettingsScreen>(), true, true);

		} else if (touching(touch, mainButtons[2])) {
			Overlays::CreditsOverlay();
		}
	}


	if (hDown & KEY_START) {
		fadeout = true;
		fadecolor = 0;
		exiting = true;
	}
}