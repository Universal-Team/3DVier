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

#include "colors.hpp"
#include "gui.hpp"
#include "lang.hpp"
#include "msg.hpp"
#include "screenCommon.hpp"
#include "selector.hpp"

extern std::unique_ptr<Selector> selector;

void Msg::DisplayPlayerSwitch(std::string message, bool redraw) {
	Gui::clearScreen(true, true);
	Gui::clearScreen(false, true);

	Gui::DrawTop(true);
	Gui::DrawBottom(true);

	printTextCenteredTinted(message, TextColor::black, 0, 80, true, true);
	printTextCentered(Lang::get("Y_CONTINUE"), 0, 175, true, true);

	while(1) {
		scanKeys();
		if (keysDown() & KEY_Y)	break;
	}

	/* Redraw screen. */
	if (redraw) Gui::DrawScreen();
}

void Msg::DisplayWaitMsg(std::string message, bool redraw) {
	Gui::clearScreen(true, true);
	Gui::clearScreen(false, true);

	Gui::DrawTop(true);
	Gui::DrawBottom(true);

	printTextCenteredTinted(message, TextColor::black, 0, 80, true, true);
	printTextCentered(Lang::get("A_CONTINUE"), 0, 175, true, true);

	while(1) {
		scanKeys();
		if (keysDown() & KEY_A)	break;
	}

	/* Redraw screen. */
	if (redraw) Gui::DrawScreen();
}

const std::vector<ButtonStruct> promptBtns = {
	{20, 80, 88, 32, "YES", BUTTON_COLOR, true}, // Yep.
	{130, 80, 88, 32, "NO", BUTTON_COLOR, true} // Nope.
};

extern bool Buttontouching(ButtonStruct button);

bool Msg::promptMsg(std::string message, bool hideSelector, bool redraw) {
	int selection = 0;
	bool updateSelector = true;

	if (updateSelector) {
		selector->move(promptBtns[selection].x, promptBtns[selection].y);
		selector->show();
		doUpdate = true;
		selector->update();
	}

	Gui::clearScreen(true, true);
	Gui::clearScreen(false, true);

	Gui::DrawTop(true);
	printTextCenteredTinted(message, TextColor::black, 0, 80, true, true);

	Gui::DrawBottom(true);

	for (int i = 0; i < 2; i++) {
		Gui::DrawButton(promptBtns[i]);
	}

	while(1) {
		if (updateSelector) {
			selector->move(promptBtns[selection].x, promptBtns[selection].y);
			doUpdate = true;
			selector->update();
		}

		scanKeys();
		u16 hDown = keysDown();

		if (hDown & KEY_RIGHT) {
			if (selection == 0) {
				selection = 1;
				updateSelector = true;
			}
		}

		if (hDown & KEY_TOUCH) {
			if (Buttontouching(promptBtns[0])) {
				selection = 0;
				break;

			} else if (Buttontouching(promptBtns[1])) {
				selection = 1;
				break;
			}
		}


		if (hDown & KEY_LEFT) {
			if (selection == 1) {
				selection = 0;
				updateSelector = true;
			}
		}

		if (hDown & KEY_A) {
			break;
		}
	}

	/* Redraw screen. */
	if (redraw) Gui::DrawScreen();

	if (hideSelector) {
		selector->hide();
		doUpdate = true;
		selector->update();
	}

	return selection ? false : true;
}