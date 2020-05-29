/*
*   This file is part of 3DVier
*   Copyright (C) 2020 DeadPhoenix8091, Epicpkmn11, Flame, RocketRobz, StackZ, TotallyNotGuy
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
#include "credits.hpp"
#include "gameScreen.hpp"
#include "GameSelectMode.hpp"
#include "mainMenu.hpp"
#include "localRoomScreen.hpp"
#include "localRoomList.hpp"
#include "uiSettings.hpp"

extern std::unique_ptr<Config> config;
extern bool exiting;
extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern bool btnTouch(touchPosition touch, ButtonStruct button);

void GameSelect::Draw(void) const {
	if (this->Mode == 0) {
		this->DrawFirstSelection();
	} else if (this->Mode == 1) {
		this->DrawTypeSelection();
	} else if (this->Mode == 2) {
		this->DrawCreateSelection();
	}
}

void GameSelect::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (this->Mode == 0) {
		this->firstLogic(hDown, hHeld, touch);
	} else if (this->Mode == 1) {
		this->typeLogic(hDown, hHeld, touch);
	} else if (this->Mode == 2) {
		this->createLogic(hDown, hHeld, touch);
	}
}

void GameSelect::DrawFirstSelection(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), Lang::get("GAME_SELECT"), 390);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	for (int i = 0; i < 2; i++) {
		GFX::Button(this->mainButtons[i]);
	}
	GFX::DrawButtonSelector(mainButtons[this->mainSelection].X, mainButtons[this->mainSelection].Y);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void GameSelect::firstLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_DOWN) {
		if (this->mainSelection < (int)this->mainButtons.size()-1)	this->mainSelection++;
	}

	if (hDown & KEY_UP) {
		if (this->mainSelection > 0)	this->mainSelection--;
	}

	if (hDown & KEY_A) {
		if (this->mainSelection == 0) {
			Msg::NotImplementedYet();
		} else if (this->mainSelection == 1) {
			this->Mode = 1;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (btnTouch(touch, mainButtons[0])) {
			Msg::NotImplementedYet();
		} else if (btnTouch(touch, mainButtons[1])) {
			this->Mode = 1;
		}
	}

	if (hDown & KEY_B) {
		Gui::screenBack(true);
		return;
	}
}

void GameSelect::DrawTypeSelection(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), Lang::get("GAME_TYPE"), 390);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	for (int i = 0; i < 3; i++) {
		GFX::Button(this->typeBtn[i]);
	}
	GFX::DrawButtonSelector(typeBtn[this->typeSelection].X, typeBtn[this->typeSelection].Y);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void GameSelect::typeLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_DOWN) {
		if (this->typeSelection < (int)this->typeBtn.size()-1)	this->typeSelection++;
	}

	if (hDown & KEY_UP) {
		if (this->typeSelection > 0)	this->typeSelection--;
	}

	if (hDown & KEY_A) {
		if (this->typeSelection == 0) {
			Gui::setScreen(std::make_unique<GameScreen>());
		} else if (this->typeSelection == 1) {
			this->Mode = 2;
		} else if (this->typeSelection == 2) {
			Msg::NotImplementedYet();
		}
	}

	if (hDown & KEY_TOUCH) {
		if (btnTouch(touch, typeBtn[0])) {
			Gui::setScreen(std::make_unique<GameScreen>());
		} else if (btnTouch(touch, typeBtn[1])) {
			this->Mode = 2;
		} else if (btnTouch(touch, typeBtn[2])) {
			Msg::NotImplementedYet();
		}
	}

	if (hDown & KEY_B) {
		this->Mode = 0;
	}
}

void GameSelect::DrawCreateSelection(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), Lang::get("GAME_CREATE"), 390);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	for (int i = 0; i < 2; i++) {
		GFX::Button(this->createMode[i]);
	}
	GFX::DrawButtonSelector(createMode[this->createSelection].X, createMode[this->createSelection].Y);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void GameSelect::createLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_DOWN) {
		if (this->createSelection < (int)this->createMode.size()-1)	this->createSelection++;
	}

	if (hDown & KEY_UP) {
		if (this->createSelection > 0)	this->createSelection--;
	}

	if (hDown & KEY_A) {
		if (this->createSelection == 0) {
			Gui::setScreen(std::make_unique<LocalRoomScreen>());
		} else if (this->createSelection == 1) {
			Gui::setScreen(std::make_unique<LocalRoomList>());
		}
	}

	if (hDown & KEY_TOUCH) {
		if (btnTouch(touch, createMode[0])) {
			Gui::setScreen(std::make_unique<LocalRoomScreen>());
		} else if (btnTouch(touch, createMode[1])) {
			Gui::setScreen(std::make_unique<LocalRoomList>());
		}
	}

	if (hDown & KEY_B) {
		this->Mode = 1;
	}
}