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

#include "_3DVier_Helper.hpp"
#include "config.hpp"
#include "gameScreen.hpp"
#include "keyboard.hpp"

extern std::unique_ptr<Config> config;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

GameScreen::GameScreen() {
	// First we select the Avatars.
	// Then the names.
	this->avatar1 = _3DVier_Helper::selectAvatar(0, Lang::get("PLAYER1_CHAR"));
	this->avatar2 = _3DVier_Helper::selectAvatar(0, Lang::get("PLAYER2_CHAR"));
	this->p1Name = Keyboard::getString(10, Lang::get("PLAYER1_NAME"), 0.6f);
	this->p2Name = Keyboard::getString(10, Lang::get("PLAYER2_NAME"), 0.6f);
	int Temp = Keyboard::getUint(255, Lang::get("ENTER_WIN_AMOUNT"));
	if (Temp > 0)	this->winAmount = Temp;
	else			this->winAmount = 3; // 3 Default.

	this->currentGame = std::make_unique<Game>();
	this->Refresh();
}

std::string GameScreen::getName(int Player) const {
	if (Player == 1)	return this->p1Name;
	else 				return this->p2Name;
}

int GameScreen::getAvatar(int Player) const {
	if (Player == 1)	return this->avatar1;
	else				return this->avatar2;
}


// Should be basically a helper for the AI in the future.
int GameScreen::getValue(int row) {
	if (!this->currentGame->isUsed(row)) {
		return row;
	}

	if (!this->currentGame->isUsed(row + 7)) {
		return row + 7;
	}

	if (!this->currentGame->isUsed(row + 14)) {
		return row + 14;
	}

	if (!this->currentGame->isUsed(row + 21)) {
		return row + 21;
	}

	if (!this->currentGame->isUsed(row + 28)) {
		return row + 28;
	}

	if (!this->currentGame->isUsed(row + 35)) {
		return row + 35;
	}

	if (this->currentGame->isUsed(row + 35)) {
		return -1;
	}

	return -1;
}

// TODO.
int GameScreen::handleAI() {
	return 0;
}


void GameScreen::Draw(void) const {
GFX::DrawTop(false);
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.
	GFX::DrawRaster(114, 5);
	// Draw current Player and the chip color.
	GFX::DrawChar(this->getAvatar(this->currentGame->currentPlayer()), -5, 35, 1, 1);
	GFX::DrawChip(45, 165, 1, 1, this->currentGame->currentPlayer());
	for (int i = 0; i < (int)this->currentGame->getField().size(); i++) {
		if (this->currentGame->getField()[i] != 0) {
			GFX::DrawChip(GamePos[i].X, GamePos[i].Y, 1, 1, this->currentGame->getField()[i]);
		}
	}
	// Draw the temporary chip.
	GFX::DrawSelectedChip(GamePos[this->dropSelection].X, GamePos[this->dropSelection].Y);

	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.
	Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), Lang::get("WINS_TO_WIN") + " " + std::to_string(this->winAmount), 320);

	GFX::DrawChar(this->getAvatar(1), 30, 35, 1, 1);
	Gui::DrawString(40, 160, 0.6f, config->textColor(), this->getName(1), 320);
	Gui::DrawString(40, 180, 0.6f, config->textColor(), Lang::get("WINS") + " " + std::to_string(this->currentGame->getScore(1)), 320);

	GFX::DrawChar(this->getAvatar(2), 170, 35, 1, 1);
	Gui::DrawString(175, 160, 0.6f, config->textColor(), this->getName(2), 320);
	Gui::DrawString(175, 180, 0.6f, config->textColor(), Lang::get("WINS") + " " + std::to_string(this->currentGame->getScore(2)), 320);
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
	if (hDown & KEY_START) {
		if (Msg::promptMsg(Lang::get("GAME_EXIT"))) {
			Gui::screenBack();
			return;
		}
	}

	if (hDown & KEY_A) {
		if (this->currentGame->setChip(this->dropSelection, this->currentGame->currentPlayer()) == true) {
			if (this->currentGame->checkMatches(this->currentGame->currentPlayer()) == true) {
				if (this->currentGame->getScore(this->currentGame->currentPlayer()) < this->winAmount) {
					char message [100];
					snprintf(message, sizeof(message), Lang::get("GAME_WIN_ROUND").c_str(), this->getName(this->currentGame->currentPlayer()).c_str(), this->winAmount - this->currentGame->getScore(this->currentGame->currentPlayer()));
					Msg::DisplayWaitMsg(message);
					this->currentGame->clearField();
					this->rowSelection = 3;
					this->Refresh();
					this->currentGame->currentPlayer(1);
					return;
				} else {
					char message [100];
					snprintf(message, sizeof(message), Lang::get("GAME_RESULT").c_str(), this->getName(this->currentGame->currentPlayer()).c_str(),
					 this->getName(1).c_str(), this->currentGame->getScore(1), this->getName(2).c_str(), this->currentGame->getScore(2));
					Msg::DisplayWaitMsg(message);
					Gui::screenBack();
					return;
				}
			} else {
				if (this->currentGame->currentPlayer() == 1) {
					this->currentGame->currentPlayer(2);
				} else {
					this->currentGame->currentPlayer(1);
				}
			}
			this->rowSelection = 3;
			this->Refresh();
			if (this->currentGame->allUsed()) {
				Msg::DisplayWaitMsg(Lang::get("GAME_ALL_USED"));
				this->currentGame->clearField();
				this->rowSelection = 3;
				this->Refresh();
				this->currentGame->currentPlayer(1);
				return;
			}
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

	if (hDown & KEY_R) {
		this->rowSelection = 6;
		this->Refresh();
	}

	if (hDown & KEY_L) {
		this->rowSelection = 0;
		this->Refresh();
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("GAME_INSTR"));
	}
}