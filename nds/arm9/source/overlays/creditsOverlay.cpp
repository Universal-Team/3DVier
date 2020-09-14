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
extern Image StackZ, UT;

static void Draw() {
	Gui::clearScreen(true, true);
	Gui::clearScreen(false, true);

	Gui::DrawTop(true);
	printTextCentered("DSVier - " + Lang::get("CREDITS"), 0, 1, true, true);

	printTextCentered(Lang::get("DEVELOPED_BY"), 0, 23, true, true);
	printTextCentered(Lang::get("MAIN_DEV"), 0, 35, true, true);
	drawImage(15, 52, StackZ, true, true);

	drawImage(190, 110, UT, true, true);

	printTextCentered(Lang::get("CURRENT_VERSION") + VER_NUMBER, 0, 173, true, true);

	Gui::DrawBottom(true);

	printTextCentered("devkitPro", 0, 30, false, true);
	printTextCentered(Lang::get("DEVKITPRO"), 0, 45, false, true);

	printTextCentered("SuperSaiyajinStackZ", 0, 80, false, true);
	printTextCentered(Lang::get("DEVELOPING_APP"), 0, 95, false, true);
}

void Overlays::CreditsOverlay() {
	selector->hide();
	doUpdate = true;
	selector->update();

	bool isOut = false;

	Draw();

	swiWaitForVBlank();
	while(!isOut) {
		scanKeys();
		u16 hDown = keysDown();

		if (hDown) isOut = true;

	}

	Gui::DrawScreen();
	selector->show();
	doUpdate = true;
	selector->update();
}