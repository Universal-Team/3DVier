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

#include "overlay.hpp"
#include "selector.hpp"

extern std::unique_ptr<Selector> selector;
extern Image characters[8];

static void Draw(std::string message) {
	Gui::clearScreen(true, true);
	Gui::clearScreen(false, true);
	Gui::DrawTop(true);
	printTextCentered(message, 0, 1, true, false);
	Gui::DrawBottom(true);
}

static void AvatarUpdate(int selection) {
	Gui::clearScreen(true, true);

	drawImage(60, 52, characters[selection], true, true);
}

int Overlays::AvatarOverlay(std::string message, int temp) {
	int selection = 0;
	bool selectedAvatar = false, tempSelect = true;

	selector->hide();
	doUpdate = true;
	selector->update();

	Draw(message);
	AvatarUpdate(selection);

	swiWaitForVBlank();
	while(!selectedAvatar) {
		swiWaitForVBlank();
		
		scanKeys();
		u16 hRepeat = keysDownRepeat();

		if (hRepeat & KEY_R || hRepeat & KEY_RIGHT) {
			if (selection < 7) {
				selection++;
				AvatarUpdate(selection);
			}
		}

		if (hRepeat & KEY_L || hRepeat & KEY_LEFT) {
			if (selection > 0) {
				selection--;
				AvatarUpdate(selection);
			}
		}

		if (keysDown() & KEY_A) {
			tempSelect = false;
			selectedAvatar = true;
		}

		if (keysDown() & KEY_B) selectedAvatar = true;

	}

	Gui::clearScreen(true, false);
	return tempSelect ? temp : selection;
}