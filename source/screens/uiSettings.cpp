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

#include "colorChanger.hpp"
#include "config.hpp"
#include "langSelection.hpp"
#include "uiSettings.hpp"


extern std::unique_ptr<Config> config;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

void UISettings::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.9f, config->textColor(), "3DVier - " + Lang::get("UI_SETTINGS"));
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();

	for (int i = 0; i < 6; i++) {
		Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, config->buttonColor());
		if (this->Selection == i) {
			GFX::DrawButtonSelector(mainButtons[i].x, mainButtons[i].y);
		}
	}

	Gui::DrawStringCentered(-80, mainButtons[0].y+12, 0.6f, config->textColor(), Lang::get("COLOR_SETTINGS"), 130);
	Gui::DrawStringCentered(80, mainButtons[1].y+12, 0.6f, config->textColor(), Lang::get("LANGUAGE"), 130);
	Gui::DrawStringCentered(-80, mainButtons[2].y+12, 0.6f, config->textColor(), Lang::get("TOGGLE_DIMMED_SCREEN"), 130);
	Gui::DrawStringCentered(80, mainButtons[3].y+12, 0.6f, config->textColor(), Lang::get("ALLOW_DROPS"), 130);
	Gui::DrawStringCentered(-80, mainButtons[4].y+12, 0.6f, config->textColor(), Lang::get("ANIMATION_MODE"), 130);
	Gui::DrawStringCentered(80, mainButtons[5].y+12, 0.6f, config->textColor(), "???", 130);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}


void UISettings::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			Gui::setScreen(std::make_unique<ColorChanger>(), true, true);
		} else if (touching(touch, mainButtons[1])) {
			Gui::setScreen(std::make_unique<LangSelection>(), true, true);
		} else if (touching(touch, mainButtons[2])) {
			if (config->darkenScreen()) {
				if (Msg::promptMsg(Lang::get("DARKEN_MSG"))) {
					config->darkenScreen(false);
					Msg::DisplayWaitMsg(Lang::get("TURNED_OFF"));
				}
			} else {
				config->darkenScreen(true);
				Msg::DisplayWaitMsg(Lang::get("TURNED_ON"));
			}
		} else if (touching(touch, mainButtons[3])) {
			if (config->allowDrop()) {
				config->allowDrop(false);
				Msg::DisplayWaitMsg(Lang::get("TURNED_OFF"));
			} else {
				config->allowDrop(true);
				Msg::DisplayWaitMsg(Lang::get("TURNED_ON"));
			}
		} else if (touching(touch, mainButtons[4])) {
			if (config->allowDrop()) {
				std::vector<std::string> temp;
				temp.push_back({Lang::get("BOUNCY_ANIM")});
				temp.push_back({Lang::get("SMOOTH_ANIM")});
				config->dropMode(GFX::selectList(temp, Lang::get("SELECT_ANIM_MODE"), config->dropMode()));
			} else {
				Msg::DisplayWaitMsg(Lang::get("ANIMATION_DISABLED"));
			}
		}
	}

	if (hDown & KEY_UP) {
		if (this->Selection > 1)	this->Selection -= 2;
	}
	if (hDown & KEY_DOWN) {
		if (this->Selection < 4)	this->Selection += 2;
	}
	if (hDown & KEY_LEFT) {
		if (this->Selection%2) this->Selection--;
	}
	if (hDown & KEY_RIGHT) {
		if (!(this->Selection%2))	this->Selection++;
	}

	if (hDown & KEY_A) {
		if (this->Selection == 0) {
			Gui::setScreen(std::make_unique<ColorChanger>(), true, true);
		} else if (this->Selection == 1) {
			Gui::setScreen(std::make_unique<LangSelection>(), true, true);
		} else if (this->Selection == 2) {
			if (config->darkenScreen()) {
				if (Msg::promptMsg(Lang::get("DARKEN_MSG"))) {
					config->darkenScreen(false);
					Msg::DisplayWaitMsg(Lang::get("TURNED_OFF"));
				}
			} else {
				config->darkenScreen(true);
				Msg::DisplayWaitMsg(Lang::get("TURNED_ON"));
			}
		} else if (this->Selection == 3) {
			if (config->allowDrop()) {
				config->allowDrop(false);
				Msg::DisplayWaitMsg(Lang::get("TURNED_OFF"));
			} else {
				config->allowDrop(true);
				Msg::DisplayWaitMsg(Lang::get("TURNED_ON"));
			}
		} else if (this->Selection == 4) {
			if (config->allowDrop()) {
				std::vector<std::string> temp;
				temp.push_back({Lang::get("BOUNCY_ANIM")});
				temp.push_back({Lang::get("SMOOTH_ANIM")});
				config->dropMode(GFX::selectList(temp, Lang::get("SELECT_ANIM_MODE"), config->dropMode()));
			} else {
				Msg::DisplayWaitMsg(Lang::get("ANIMATION_DISABLED"));
			}
		}
	}

	if (hDown & KEY_B) {
		Gui::screenBack(true);
		return;
	}
}