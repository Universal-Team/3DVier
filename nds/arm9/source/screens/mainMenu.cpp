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

#include "gameScreen.hpp"
#include "mainMenu.hpp"
#include "selector.hpp"

extern bool exiting;
extern std::unique_ptr<Selector> selector;
extern bool Buttontouching(ButtonStruct button);

MainMenu::MainMenu() { }

void MainMenu::Draw(void) const {
	Gui::DrawTop(true);
	printTextCentered("DSVier - " + Lang::get("MAINMENU"), 0, 1, true, true);

	Gui::DrawBottom(true);

	for (int i = 0; i < 3; i++) {
		Gui::DrawButton(this->buttonPos[i]);
	}
}

void MainMenu::Logic(u16 hDown, touchPosition touch) {
	u16 hRepeat = keysDownRepeat();

	if (doUpdate) {
		selector->move(this->buttonPos[this->selection].x, this->buttonPos[this->selection].y);
		selector->update();
	}

	if (hDown & KEY_A) {
		switch(this->selection) {
			case 0:
				Gui::setScreen(std::make_unique<GameScreen>());
				Gui::DrawScreen();
				selector->hide();
				updateOam();
				break;

			case 1:
				break; // Settings: TODO.

			case 2:
				Overlays::CreditsOverlay();
				break;
		}
	}

	if (hDown & KEY_START) {
		exiting = true;
	}

	if (hDown & KEY_TOUCH) {
		if (Buttontouching(this->buttonPos[0])) {
			Gui::setScreen(std::make_unique<GameScreen>());
			Gui::DrawScreen();
			selector->hide();
			updateOam();

		} else if (Buttontouching(this->buttonPos[1])) {
			/* Settings: TODO. */

		} else if (Buttontouching(this->buttonPos[2])) {
			Overlays::CreditsOverlay();
		}
	}

	if (hRepeat & KEY_DOWN) {
		if (this->selection < 2) {
			this->selection++;
			doUpdate = true;
		}
	}

	if (hRepeat & KEY_UP) {
		if (this->selection > 0) {
			this->selection--;
			doUpdate = true;
		}
	}
}