/*
*   This file is part of 3DVier
*   Copyright (C) 2020 SuperSaiyajinStackZ
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
#include "gameScreen.hpp"

extern std::unique_ptr<Config> config;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

GameScreen::GameScreen() {
	this->currentGame = std::make_unique<Game>();
	this->Refresh();
}

void GameScreen::Draw(void) const {
	GFX::DrawTop();
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.
	GFX::DrawRaster(64, 5);
	for (int i = 0; i < (int)this->currentGame->getField().size(); i++) {
		if (this->currentGame->getField()[i] != 0) {
			GFX::DrawChip(GamePos[i].X, GamePos[i].Y, 1, 1, this->currentGame->getField()[i]);
		}
	}
	// Draw the temporary chip.
	GFX::DrawSelectedChip(GamePos[this->dropSelection].X, GamePos[this->dropSelection].Y);

	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.
	Gui::DrawStringCentered(0, 0, 0.9f, config->textColor(), "Current Player: " + std::to_string(this->currentGame->currentPlayer()), 320);
}

void GameScreen::Refresh() {
	// Here we set the "dropSelection".
	if (!this->currentGame->isUsed(rowSelection)) {
		this->dropSelection = this->rowSelection;
		return;
	}

	if (!this->currentGame->isUsed(rowSelection + 7)) {
		this->dropSelection = this->rowSelection + 7;
		return;
	}

	if (!this->currentGame->isUsed(rowSelection + 14)) {
		this->dropSelection = this->rowSelection + 14;
		return;
	}

	if (!this->currentGame->isUsed(rowSelection + 21)) {
		this->dropSelection = this->rowSelection + 21;
		return;
	}

	if (!this->currentGame->isUsed(rowSelection + 28)) {
		this->dropSelection = this->rowSelection + 28;
		return;
	}

	if (!this->currentGame->isUsed(rowSelection + 35)) {
		this->dropSelection = this->rowSelection + 35;
		return;
	}
	// Display indicator on the last position, if row is full.
	if (this->currentGame->isUsed(rowSelection + 35)) {
		this->dropSelection = this->rowSelection + 35;
		return;
	}
}


void GameScreen::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_A) {
		if (this->currentGame->setChip(this->dropSelection, this->currentGame->currentPlayer()) == true) {
			if (this->currentGame->checkMatches(this->currentGame->currentPlayer()) == true) {
				Msg::DisplayWaitMsg("Player " + std::to_string(this->currentGame->currentPlayer()) + " wins!");
				Gui::screenBack();
				return;
			} else {
				if (this->currentGame->currentPlayer() == 1) {
					this->currentGame->currentPlayer(2);
				} else {
					this->currentGame->currentPlayer(1);
				}
			}
			this->rowSelection = 3;
			this->Refresh();
		}
	}

	if (hDown & KEY_RIGHT) {
		if (this->rowSelection < 6) {
			this->rowSelection++;
			this->Refresh();
		}
	}

	if (hDown & KEY_LEFT) {
		if (this->rowSelection > 0) {
			this->rowSelection--;
			this->Refresh();
		}
	}
}