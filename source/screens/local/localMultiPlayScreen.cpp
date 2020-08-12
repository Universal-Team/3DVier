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
#include "localMultiPlayScreen.hpp"

extern std::unique_ptr<Config> config;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

LocalMultiPlayScreen::LocalMultiPlayScreen(std::shared_ptr<LocalNetwork> &network) {
	this->room = network;
	this->avatar1 = 0;
	this->avatar2 = 5;
	this->pullNames();
	this->currentGame = std::make_shared<Game>();
	this->winAmount = 3;
	this->Refresh();
}

// Pulls the names of the network.
void LocalMultiPlayScreen::pullNames() {
	std::vector<std::string> temp = this->room->getPlayerNames();
	if (temp.size() == 2) {
		this->p1Name = temp[0];
		this->p2Name = temp[1];
	} else {
		this->p1Name = "?";
		this->p2Name = "?";
	}
}

std::string LocalMultiPlayScreen::getName(int Player) const {
	if (Player == 1) return this->p1Name;
	else return this->p2Name;
}

int LocalMultiPlayScreen::getAvatar(int Player) const {
	if (Player == 1)	return this->avatar1;
	else				return this->avatar2;
}

void LocalMultiPlayScreen::Draw(void) const {
	GFX::DrawTop(false);
	if (config->darkenScreen())	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.
	GFX::DrawRaster(114, 5);
	// Draw current Player and the chip color.
	GFX::DrawChar(this->getAvatar(this->currentGame->currentPlayer()), -5, 35, 1, 1);
	GFX::DrawChip(45, 165, 1, 1, this->currentGame->currentPlayer());
	if (this->delay > 0) {
		Gui::DrawString(30, 200, 0.5f, config->textColor(), Lang::get("DELAY") + std::to_string(this->delay), 80);
	} else {
		Gui::DrawString(30, 200, 0.5f, config->textColor(), Lang::get("PLAY"), 80);
	}


	for (int i = 0; i < (int)this->currentGame->getField().size(); i++) {
		if (this->currentGame->getField()[i] != 0) {
			GFX::DrawChip(GamePos[i].X, GamePos[i].Y, 1, 1, this->currentGame->getField()[i]);
		}
	}

	// Draw the temporary chip.
	if (this->currentGame->currentPlayer() == this->room->getPosition()) {
		GFX::DrawSelectedChip(GamePos[this->dropSelection].X, GamePos[this->dropSelection].Y);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();
	if (config->darkenScreen())	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.
	Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), Lang::get("WINS_TO_WIN") + " " + std::to_string(this->winAmount), 320);

	GFX::DrawChar(this->getAvatar(1), 30, 35, 1, 1);
	Gui::DrawString(40, 160, 0.6f, config->textColor(), this->getName(1), 320);
	Gui::DrawString(40, 180, 0.6f, config->textColor(), Lang::get("WINS") + " " + std::to_string(this->currentGame->getScore(1)), 320);

	GFX::DrawChar(this->getAvatar(2), 170, 35, 1, 1);
	Gui::DrawString(175, 160, 0.6f, config->textColor(), this->getName(2), 320);
	Gui::DrawString(175, 180, 0.6f, config->textColor(), Lang::get("WINS") + " " + std::to_string(this->currentGame->getScore(2)), 320);

	if (this->currentGame->currentPlayer() != this->room->getPosition()) {
		Gui::DrawStringCentered(0, 216, 0.7, config->textColor(), Lang::get("WAITING_FOR") + this->getName(this->currentGame->currentPlayer()) + Lang::get("TURN"));
	} else {
		Gui::DrawStringCentered(0, 216, 0.7, config->textColor(), Lang::get("YOUR_TURN"));
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void LocalMultiPlayScreen::Refresh() {
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

void LocalMultiPlayScreen::sendPlay() {
	if (this->currentGame->canDrop(this->dropSelection) == true) {
		this->playPkg = {(u8)this->dropSelection, (u8)this->rowSelection, (u8)this->currentGame->currentPlayer(), true};
		this->room->sendPlay(playPkg);
		this->lastPlayer = this->playPkg.LastPlayer;
	}
}

// ONLY DO REFRESH IF IT REALLY MISSED ONE PLAY!!! Otherwise it will cause buggy stuff and you have to restart 3DVier.
void LocalMultiPlayScreen::sendLastPlay() {
	this->room->sendPlay(playPkg, true);
}

// Display drop effect.
void LocalMultiPlayScreen::displayAnimation(int dropSel) {
	// Down speed.
	int downSpeed = 5;
	bool dropped = true;
	while(dropped) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		GFX::DrawTop(false);
		if (config->darkenScreen())	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.
		GFX::DrawChip(GamePos[dropSel].X, this->dropPos, 1, 1, this->currentGame->currentPlayer());
		GFX::DrawRaster(114, 5);
		// Draw current Player and the chip color.
		GFX::DrawChar(this->getAvatar(this->currentGame->currentPlayer()), -5, 35, 1, 1);
		GFX::DrawChip(45, 165, 1, 1, this->currentGame->currentPlayer());
		for (int i = 0; i < (int)this->currentGame->getField().size(); i++) {
			if (this->currentGame->getField()[i] != 0) {
				GFX::DrawChip(GamePos[i].X, GamePos[i].Y, 1, 1, this->currentGame->getField()[i]);
			}
		}

		GFX::DrawBottom();
		if (config->darkenScreen())	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.
		Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), Lang::get("WINS_TO_WIN") + " " + std::to_string(this->winAmount), 320);

		GFX::DrawChar(this->getAvatar(1), 30, 35, 1, 1);
		Gui::DrawString(40, 160, 0.6f, config->textColor(), this->getName(1), 320);
		Gui::DrawString(40, 180, 0.6f, config->textColor(), Lang::get("WINS") + " " + std::to_string(this->currentGame->getScore(1)), 320);

		GFX::DrawChar(this->getAvatar(2), 170, 35, 1, 1);
		Gui::DrawString(175, 160, 0.6f, config->textColor(), this->getName(2), 320);
		Gui::DrawString(175, 180, 0.6f, config->textColor(), Lang::get("WINS") + " " + std::to_string(this->currentGame->getScore(2)), 320);
		C3D_FrameEnd(0);

		// Mode 0 would be "bouncy".
		if (config->dropMode() == 0) {
			if (this->dropPos < GamePos[this->dropSelection].Y) {
				downSpeed = downSpeed * 1.4;
				this->dropPos = this->dropPos + downSpeed;
			} else {
				this->dropPos = 0;
				dropped = false;
			}

			// Mode 1 would be "Smooth".
		} else if (config->dropMode() == 1) {
			if (this->dropPos < GamePos[this->dropSelection].Y) {
				this->dropPos += 9;
			} else {
				this->dropPos = 0;
				dropped = false;
			}
		}
	}
}

// Clear's the field by dropping the chips.
void LocalMultiPlayScreen::clearField() {
	bool hasWon = true;
	int Pos = 0;
	int max = 0;
	// Down speed.
	int downSpeed = 5;

	while(hasWon) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		GFX::DrawTop(false);
		if (config->darkenScreen())	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.
		// We draw this before the raster cause it looks weird after the raster.
		for (int i = 0; i < (int)this->currentGame->getField().size(); i++) {
			if (this->currentGame->getField()[i] != 0) {
				GFX::DrawChip(GamePos[i].X, GamePos[i].Y+Pos, 1, 1, this->currentGame->getField()[i]);
			}
		}
		GFX::DrawRaster(114, 5);
		// Draw current Player and the chip color.
		GFX::DrawChar(this->getAvatar(this->currentGame->currentPlayer()), -5, 35, 1, 1);
		GFX::DrawChip(45, 165, 1, 1, this->currentGame->currentPlayer());


		GFX::DrawBottom();
		if (config->darkenScreen())	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.
		Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), Lang::get("WINS_TO_WIN") + " " + std::to_string(this->winAmount), 320);

		GFX::DrawChar(this->getAvatar(1), 30, 35, 1, 1);
		Gui::DrawString(40, 160, 0.6f, config->textColor(), this->getName(1), 320);
		Gui::DrawString(40, 180, 0.6f, config->textColor(), Lang::get("WINS") + " " + std::to_string(this->currentGame->getScore(1)), 320);

		GFX::DrawChar(this->getAvatar(2), 170, 35, 1, 1);
		Gui::DrawString(175, 160, 0.6f, config->textColor(), this->getName(2), 320);
		Gui::DrawString(175, 180, 0.6f, config->textColor(), Lang::get("WINS") + " " + std::to_string(this->currentGame->getScore(2)), 320);
		C3D_FrameEnd(0);

		for (int i = 0; i < (int)this->currentGame->getField().size(); i++) {
			if (this->currentGame->getField()[i] != 0) {
				// Here we get the highest pos.
				if (i > max) {
					max = i;
				}
			}
		}

		if (GamePos[max].Y + Pos < 240) {
			Pos = Pos + downSpeed;
		} else {
			hasWon = false;
		}
	}
}

void LocalMultiPlayScreen::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (this->delay > 0)	this->delay--;

	if (this->currentGame->currentPlayer() != this->room->getPosition()) {
		if (this->room->getRound().Done != true) {
			this->room->receivePlay(); // Receive the play data...
		}
	}

	if (hDown & KEY_SELECT) {
		if (Msg::promptMsg(Lang::get("REFRESH_WARN"))) {
			this->sendLastPlay();
		}
	}

	if (this->room->getRound().Done) {
		// Make sure to get ready.
		this->room->sendDone(this->room->getPosition()-1, true);
		while(!this->room->everyoneDone()) {
			this->room->receiveDone();
		}

		this->room->sendDone(this->room->getPosition()-1, false);
		while(this->room->everyoneDone()) {
			this->room->receiveDone();
		}

		if (config->allowDrop()) {
			this->displayAnimation(this->room->getRound().Row);
		}

		if (this->currentGame->setChip(this->room->getRound().Position, this->currentGame->currentPlayer()) == true) {
			if (this->currentGame->checkMatches(this->currentGame->currentPlayer()) == true) {
				if (this->currentGame->getScore(this->currentGame->currentPlayer()) < this->winAmount) {

					if (config->allowDrop()) {
						this->clearField();
					}

					char message [100];
					snprintf(message, sizeof(message), Lang::get("GAME_WIN_ROUND").c_str(), this->getName(this->currentGame->currentPlayer()).c_str(), this->winAmount - this->currentGame->getScore(this->currentGame->currentPlayer()));
					Msg::DisplayMultiPlayMsg(message);

					this->currentGame->clearField();
					this->rowSelection = 3;
					this->currentGame->currentPlayer(1);
					// Refresh selector.
					this->Refresh();
				} else {
					if (config->allowDrop()) {
						this->clearField();
					}
					
					char message [100];
					snprintf(message, sizeof(message), Lang::get("GAME_RESULT").c_str(), this->getName(this->currentGame->currentPlayer()).c_str(),
					 this->getName(1).c_str(), this->currentGame->getScore(1), this->getName(2).c_str(), this->currentGame->getScore(2));
					Msg::DisplayMultiPlayMsg(message);

					// Send not ready here and receive.
					this->room->sendReady(this->room->getPosition()-1, false);
					while(this->room->everyoneReady()) {
						this->room->receiveReady();
					}

					// Set the round stuff to false, so it won't conflict.
					this->room->sendPlay({(u8)-1, (u8)-1, false});
					while(this->room->getRound().Done == true) {
						this->room->receivePlay(); // Receive the play data...
					}
					Gui::screenBack(true);
					return;
				}
			} else {
				this->rowSelection = 3;
				this->Refresh();
				if (this->currentGame->allUsed()) {
					Msg::DisplayMultiPlayMsg(Lang::get("GAME_ALL_USED"));
					this->currentGame->clearField();
					this->rowSelection = 3;
					this->Refresh();
					this->currentGame->currentPlayer(1);
				}
			}

			// Send confirmation about done and receive.
			this->room->sendDone(this->room->getPosition()-1, true);
			while(!this->room->everyoneDone()) {
				this->room->receiveDone();
			}

			// Set the round stuff to false, so it won't conflict.
			this->room->sendPlay({(u8)-1, (u8)-1, false});
			while(this->room->getRound().Done == true) {
				this->room->receivePlay(); // Receive the play data...
			}

			// Here we send and receive "done" -> false.. to basically reset it.
			this->room->sendDone(this->room->getPosition()-1, false);
			while(this->room->everyoneDone()) {
				this->room->receiveDone();
			}

			// Switch player.
			if (this->currentGame->currentPlayer() == 1) {
				this->currentGame->currentPlayer(2);
			} else {
				this->currentGame->currentPlayer(1);
			}
			// Do the delay to 120 to avoid desync.
			this->delay = 120;
		}
	}

	// Only do this when Player Amount is 2.
	if (this->room->getPlayerCount() == 2) {
		if (this->room->isPlayer(this->currentGame->currentPlayer())) {
			if (hDown & KEY_A) {
				if (!this->delay) {
					this->sendPlay();
				} else {
					Msg::DisplayMultiPlayMsg(Lang::get("TOO_FAST"));
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
		}
	} else {
		Msg::DisplayWaitMsg(Lang::get("SOMEONE_LEFT"));
		Gui::screenBack(true);
		return;
	}
}