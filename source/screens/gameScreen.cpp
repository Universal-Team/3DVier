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
	this->avatar1 = _3DVier_Helper::selectAvatar(0);
	this->avatar2 = _3DVier_Helper::selectAvatar(0);
	this->p1Name = Keyboard::getString(10, "Enter Player 1's name.", 0.6f);
	this->p2Name = Keyboard::getString(10, "Enter Player 2's name.", 0.6f);
	this->winAmount = 3; // For now just 3.
	this->currentGame = std::make_unique<Game>();
	this->Refresh();
}

std::string GameScreen::getName(int Player) const {
	if (Player == 1)	return this->p1Name;
	else 				return this->p2Name;
}

int GameScreen::getAvatar(int Player) const {
	if (Player == 1)	return this->avatar1;
	else 				return this->avatar2;
}

void GameScreen::Draw(void) const {
	if (!this->isStats) {
		GFX::DrawTop(false);
		Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.
		GFX::DrawRaster(114, 5);
		// Draw current Player and the chip color.
		GFX::DrawChar(this->getAvatar(this->currentGame->currentPlayer()), -5, 35, 1, 1);
		GFX::DrawChip(50, 160, 1, 1, this->currentGame->currentPlayer());
		for (int i = 0; i < (int)this->currentGame->getField().size(); i++) {
			if (this->currentGame->getField()[i] != 0) {
				GFX::DrawChip(GamePos[i].X, GamePos[i].Y, 1, 1, this->currentGame->getField()[i]);
			}
		}
		// Draw the temporary chip.
		GFX::DrawSelectedChip(GamePos[this->dropSelection].X, GamePos[this->dropSelection].Y);

		GFX::DrawBottom();
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.
		Gui::DrawStringCentered(0, 0, 0.9f, config->textColor(), "Current Player: " + this->getName(this->currentGame->currentPlayer()), 320);
	} else {
		GFX::DrawTop();
		Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.
		Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), "Needed Wins to win: " + std::to_string(this->winAmount), 320);

		GFX::DrawChar(this->getAvatar(1), 70, 35, 1, 1);
		Gui::DrawString(75, 160, 0.6f, config->textColor(), this->getName(1), 320);
		Gui::DrawString(75, 180, 0.6f, config->textColor(), "Wins: " + std::to_string(this->currentGame->getScore(1)), 320);

		GFX::DrawChar(this->getAvatar(2), 200, 35, 1, 1);
		Gui::DrawString(205, 160, 0.6f, config->textColor(), this->getName(2), 320);
		Gui::DrawString(205, 180, 0.6f, config->textColor(), "Wins: " + std::to_string(this->currentGame->getScore(2)), 320);

		GFX::DrawBottom();
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.
	}
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

	if (this->isStats) {
		if (hDown & KEY_L)	this->isStats = false;
	}

	if (!this->isStats) {
		if (hDown & KEY_A) {
			if (this->currentGame->setChip(this->dropSelection, this->currentGame->currentPlayer()) == true) {
				if (this->currentGame->checkMatches(this->currentGame->currentPlayer()) == true) {
					if (this->currentGame->getScore(this->currentGame->currentPlayer()) < this->winAmount) {
						Msg::DisplayWaitMsg(this->getName(this->currentGame->currentPlayer()) + " wins this match!\nNeeded Wins to win this game: " + std::to_string(this->winAmount) + ".");
						this->currentGame->clearField();
						this->rowSelection = 3;
						this->Refresh();
						this->currentGame->currentPlayer(1);
						return;
					} else {
						Msg::DisplayWaitMsg(this->getName(this->currentGame->currentPlayer()) + " wins this game!\nResult: "
						+ this->getName(1) + ": " + std::to_string(this->currentGame->getScore(1)) + " | " + this->getName(2) + ": "
						+ std::to_string(this->currentGame->getScore(2)));
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
					Msg::DisplayWaitMsg("Game Over... all slots are used.\nWould you like to rematch?");
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

		if (hDown & KEY_R) {
			this->isStats = true;
		}

		if (hDown & KEY_LEFT) {
			if (this->rowSelection > 0) {
				this->rowSelection--;
				this->Refresh();
			}
		}
	}
}