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

#include "config.hpp"
#include "init.hpp"
#include "localMultiPlayScreen.hpp"
#include "localRoomScreen.hpp"

#include <vector>

extern std::unique_ptr<Config> config;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

LocalRoomScreen::LocalRoomScreen() {
	Init::initUDS();
	std::shared_ptr<LocalNetwork> room = std::make_shared<LocalNetwork>();
	this->room = room;
	this->playerID = 0;
	this->ready = false;
	this->ownerLeft = false;
}

LocalRoomScreen::~LocalRoomScreen() { Init::exitUDS(); }

LocalRoomScreen::LocalRoomScreen(int playerID, std::shared_ptr<LocalNetwork>& room) {
	this->room = room;
	this->playerID = playerID;
	this->ready = false;
	this->ownerLeft = false;
}

void LocalRoomScreen::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.8f, config->textColor(), "3DVier - " + Lang::get("WAITING_ROOM"), 400);
	Gui::DrawStringCentered(0, 215, 0.8f, config->textColor(), Lang::get("TOGGLE_READY"), 400);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();

	std::vector<std::string> playerNames = this->room->getPlayerNames();
	if (playerNames.size() == 0) {
		this->ownerLeft = true;
	}

	if (this->ownerLeft) {
		Gui::DrawStringCentered(0, 0, 0.7f, config->textColor(), Lang::get("OWNER_LEFT"));
		Gui::DrawStringCentered(0, 215, 0.7f, config->textColor(), Lang::get("B_BACK"));
	} else {
		float readyX = 180;
		for(int i = 0; i < (int)playerNames.size(); i++) {
			float y = 40 + i*25;
			Gui::DrawString(32, y, 0.7f, config->textColor(), playerNames[i], 90);
			if (this->room->isPlayerReady(i)) Gui::DrawString(readyX, y, 0.7f, config->textColor(), Lang::get("IM_READY"), 90);
		}
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void LocalRoomScreen::setReady() {
	this->ready = true;
	this->room->sendReady(this->playerID, this->ready);
}

void LocalRoomScreen::setNotReady() {
	this->ready = false;
	this->room->sendReady(this->playerID, this->ready);
}

void LocalRoomScreen::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (this->room->getPlayerCount() == 2) {
		this->room->receiveReady();
	
		if (hDown & KEY_X) {
			this->setReady();
		}

		if (hDown & KEY_Y) {
			this->setNotReady();
		}

		if (this->room->everyoneReady()) {
			Gui::setScreen(std::make_unique<LocalMultiPlayScreen>(this->room), true, true);
		}
	}

	if (hDown & KEY_B) {
		Gui::screenBack(true);
		return;
	}
}