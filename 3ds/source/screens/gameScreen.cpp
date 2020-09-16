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

#include "gameScreen.hpp"
#include "keyboard.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);

GameScreen::GameScreen() {
	this->avatar1 = Overlays::AvatarOverlay(0, Lang::get("PLAYER1_CHAR"));
	this->avatar2 = Overlays::AvatarOverlay(0, Lang::get("PLAYER2_CHAR"));
	this->p1Name = Keyboard::setString(12, Lang::get("PLAYER") + " " + std::to_string(1), Lang::get("PLAYER1_NAME"));
	this->p2Name = Keyboard::setString(12, Lang::get("PLAYER") + " " + std::to_string(2), Lang::get("PLAYER2_NAME"));

	this->currentGame = std::make_unique<Game>(); // Start a new game.
	this->Refresh();
}

std::string GameScreen::getName(int Player) const {
	if (Player == 1) return this->p1Name;
	else return this->p2Name;
}

int GameScreen::getAvatar(int Player) const {
	if (Player == 1) return this->avatar1;
	else return this->avatar2;
}

/* Display Sub screen. */
void GameScreen::displaySub(void) const {
	GFX::DrawTop(false);
	Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.

	/* Display BG image. */
	GFX::DrawSprite(sprites_bg_idx, 79, 16);

	for (int i = 0; i < 42; i++) {
		if (this->currentGame->GetChip(i) != 0) {
			_3DVier_Helper::DrawChip(GamePos[i].X + 4, GamePos[i].Y + 4, this->currentGame->GetChip(i));
		}
	}

	_3DVier_Helper::DrawField(79, 16);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

	GFX::DrawBottom(true);

	/* Buttons. */
	for (int i = 0; i < 3; i++) {
		if (this->subSel == i) Gui::drawAnimatedSelector(subBtn[i].x, subBtn[i].y, 140, 40, .030f, SELECTED_BUTTON_COLOR, BUTTON_COLOR);
		else Gui::Draw_Rect(subBtn[i].x, subBtn[i].y, subBtn[i].w, subBtn[i].h, BUTTON_COLOR);
	}

	switch(this->subMode) {
		case 0:
			Gui::DrawStringCentered(0, subBtn[0].y + 10, 0.6f, TEXT_COLOR, Lang::get("LOAD_SLOT") + "...");
			Gui::DrawStringCentered(0, subBtn[1].y + 10, 0.6f, TEXT_COLOR, Lang::get("SAVE_SLOT") + "...");
			Gui::DrawStringCentered(0, subBtn[2].y + 10, 0.6f, TEXT_COLOR, Lang::get("EXIT_GAME"));
			break;

		case 1:
			Gui::DrawStringCentered(0, subBtn[0].y + 10, 0.6f, TEXT_COLOR, Lang::get("LOAD_SLOT") + " " + std::to_string(1));
			Gui::DrawStringCentered(0, subBtn[1].y + 10, 0.6f, TEXT_COLOR, Lang::get("LOAD_SLOT") + " " + std::to_string(2));
			Gui::DrawStringCentered(0, subBtn[2].y + 10, 0.6f, TEXT_COLOR, Lang::get("LOAD_SLOT") + " " + std::to_string(3));
			break;

		case 2:
			Gui::DrawStringCentered(0, subBtn[0].y + 10, 0.6f, TEXT_COLOR, Lang::get("SAVE_SLOT") + " " + std::to_string(1));
			Gui::DrawStringCentered(0, subBtn[1].y + 10, 0.6f, TEXT_COLOR, Lang::get("SAVE_SLOT") + " " + std::to_string(2));
			Gui::DrawStringCentered(0, subBtn[2].y + 10, 0.6f, TEXT_COLOR, Lang::get("SAVE_SLOT") + " " + std::to_string(3));
			break;
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

/* Draw Main Screen. */
void GameScreen::Draw(void) const {
	if (this->isSub) {
		this->displaySub();

	} else {
		GFX::DrawTop(false);
		Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.

		/* Display BG image. */
		GFX::DrawSprite(sprites_bg_idx, 79, 16);

		for (int i = 0; i < 42; i++) {
			if (this->currentGame->GetChip(i) != 0) {
				_3DVier_Helper::DrawChip(GamePos[i].X + 4, GamePos[i].Y + 4, this->currentGame->GetChip(i));
			}
		}

		_3DVier_Helper::DrawField(79, 16);

		if (this->results == GameRes::Over) {
			_3DVier_Helper::SelectedChip(GamePos[this->matches.Pos1].X, GamePos[this->matches.Pos1].Y, this->currentGame->GetChip(this->matches.Pos1), .010f);
			_3DVier_Helper::SelectedChip(GamePos[this->matches.Pos2].X, GamePos[this->matches.Pos2].Y, this->currentGame->GetChip(this->matches.Pos2), .010f);
			_3DVier_Helper::SelectedChip(GamePos[this->matches.Pos3].X, GamePos[this->matches.Pos3].Y, this->currentGame->GetChip(this->matches.Pos3), .010f);
			_3DVier_Helper::SelectedChip(GamePos[this->matches.Pos4].X, GamePos[this->matches.Pos4].Y, this->currentGame->GetChip(this->matches.Pos4), .010f);
		}

		if (this->results == GameRes::NotOver) _3DVier_Helper::SelectedChip(GamePos[this->dropSelection].X, GamePos[this->dropSelection].Y, this->currentGame->GetCurrentPlayer(), .015f);

		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

		GFX::DrawBottom();
		Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.

		if (this->results == GameRes::Over) {
			char message [100];
			snprintf(message, sizeof(message), Lang::get("PLAYER_WON_ROUND").c_str(), this->getName(this->currentGame->GetCurrentPlayer()).c_str());
			Gui::DrawStringCentered(0, 2, 0.6, TEXT_COLOR, message);

		} else if (this->results == GameRes::AllUsed) {
			Gui::DrawStringCentered(0, 2, 0.6, TEXT_COLOR, Lang::get("ALL_SLOTS_USED"));
		}

		/* Display to press `A` to continue. */
		if (this->results != GameRes::NotOver) Gui::DrawStringCentered(0, 217, 0.6, TEXT_COLOR, Lang::get("A_CONTINUE"));

		/* Display avatars. */
		GFX::DrawChar(this->getAvatar(1), 40, 45);
		Gui::DrawStringCentered(40 - 160 + (120 / 2), 173, 0.6f, TEXT_COLOR, this->getName(1));

		GFX::DrawChar(this->getAvatar(2), 150, 45);
		Gui::DrawStringCentered(150 - 160 + (120 / 2), 173, 0.6f, TEXT_COLOR, this->getName(2));

		/* Display available chips. */
		_3DVier_Helper::DrawChip(5, 30, 1);
		Gui::DrawStringCentered(5 - 160 + (30 / 2), 35, 0.6f, C2D_Color32(0, 0, 0, 255), std::to_string(this->currentGame->GetAvailableChips(1)));

		_3DVier_Helper::DrawChip(285, 30, 2);
		Gui::DrawStringCentered(285 - 160 + (30 / 2), 35, 0.6f, C2D_Color32(0, 0, 0, 255), std::to_string(this->currentGame->GetAvailableChips(2)));

		/* Display Player Wins. */
		_3DVier_Helper::DrawChip(5, 180, 1);
		Gui::DrawStringCentered(5 - 160 + (30 / 2), 185, 0.6f, C2D_Color32(0, 0, 0, 255), std::to_string(this->currentGame->GetWins(1)));

		_3DVier_Helper::DrawChip(285, 180, 2);
		Gui::DrawStringCentered(285 - 160 + (30 / 2), 185, 0.6f, C2D_Color32(0, 0, 0, 255), std::to_string(this->currentGame->GetWins(2)));

		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	}
}

/* Animated chip drop. */
void GameScreen::drop() {
	this->dropped = true;
	int downSpeed = Settings::dropSpeed(), dropPos = 2, frameCount = 0, frameDrops = 0;
	int speedPlus = Settings::speedPlus();

	while(this->dropped) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop(false);
		Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.
		GFX::DrawSprite(sprites_bg_idx, 79, 16);

		/* Drawing the drop chip. */
		if (dropPos < GamePos[this->dropSelection].Y + 4) {
			_3DVier_Helper::DrawChip(GamePos[this->rowSelection].X + 4, dropPos, this->currentGame->GetCurrentPlayer());
		}

		for (int i = 0; i < 42; i++) {
			if (this->currentGame->GetChip(i) != 0) {
				_3DVier_Helper::DrawChip(GamePos[i].X + 4, GamePos[i].Y + 4, this->currentGame->GetChip(i));
			}
		}

		_3DVier_Helper::DrawField(79, 16);
		C3D_FrameEnd(0);

		if (dropPos < GamePos[this->dropSelection].Y) {
			frameCount++;

			if (frameCount == 5) {
				frameDrops += speedPlus;
				frameCount = 0;
			}

			dropPos = dropPos + downSpeed + frameDrops;
			gspWaitForVBlank();

		} else {
			this->dropped = false;
			this->currentGame->Play(this->rowSelection, this->currentGame->GetCurrentPlayer());
		}
	}
}

/* Clear's the field by dropping the chips. */
void GameScreen::clearField() {
	bool hasWon = true;
	int Pos = 0, frameCount = 0, frameDrops = 0, max = 0;

	int downSpeed = Settings::clearSpeed();
	int speedPlus = Settings::speedPlus();

	/* Get the highest chip pos. */
	for (int i = 41; i > 0; i--) {
		if (this->currentGame->GetChip(i) != 0) {

			/* Here we get the highest pos. */
			if (i < max) max = i;
		}
	}

	while(hasWon) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		GFX::DrawTop(false);
		Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, 210)); // Darken the screen.
		GFX::DrawSprite(sprites_bg_idx, 79, 16);

		for (int i = 0; i < 42; i++) {
			if (this->currentGame->GetChip(i) != 0) {
				_3DVier_Helper::DrawChip(GamePos[i].X + 4, GamePos[i].Y + 4 + Pos, this->currentGame->GetChip(i));
			}
		}

		_3DVier_Helper::DrawField(79, 16);
		C3D_FrameEnd(0);

		if (GamePos[max].Y + Pos < 240) {
			frameCount++;

			if (frameCount == 5) {
				frameDrops += speedPlus;
				frameCount = 0;
			}

			Pos = Pos + downSpeed + frameDrops;
			gspWaitForVBlank();

		} else {
			/* Clear Gamefield etc and make ready for next round. */
			this->results = GameRes::NotOver;
			this->matches = {0, 0, 0, 0};
			this->currentGame->RestartGame();
			this->rowSelection = 3;
			this->currentGame->SetCurrentPlayer(1);
			this->Refresh();
			hasWon = false;
		}
	}
}

/* Refresh drop selection. */
void GameScreen::Refresh() {
	/* Here we set the "dropSelection". */
	if (!this->currentGame->GetChipPos(5, this->rowSelection)) {
		this->dropSelection = this->rowSelection + 35;
		return;
	}

	if (!this->currentGame->GetChipPos(4, this->rowSelection)) {
		this->dropSelection = this->rowSelection + 28;
		return;
	}

	if (!this->currentGame->GetChipPos(3, this->rowSelection)) {
		this->dropSelection = this->rowSelection + 21;
		return;
	}

	if (!this->currentGame->GetChipPos(2, this->rowSelection)) {
		this->dropSelection = this->rowSelection + 14;
		return;
	}

	if (!this->currentGame->GetChipPos(1, this->rowSelection)) {
		this->dropSelection = this->rowSelection + 7;
		return;
	}

	if (!this->currentGame->GetChipPos(0, this->rowSelection)) {
		this->dropSelection = this->rowSelection;
		return;
	}

	/* Display indicator on the last position, if row is full. */
	this->dropSelection = this->rowSelection;
	return;
}

/* Sub Menu Logic. */
void GameScreen::subLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hRepeat & KEY_DOWN) {
		if (this->subSel < 2) this->subSel++;
	}

	if (hRepeat & KEY_UP) {
		if (this->subSel > 0) this->subSel--;
	}

	/* Main Mode. */
	if (this->subMode == 0) {
		if (hDown & KEY_B) {
			this->isSub = false;
		}

		if (hDown & KEY_A) {
			switch(this->subSel) {
				case 0:
					this->subSel = 0;
					this->subMode = 1;
					break;

				case 1:
					this->subSel = 0;
					this->subMode = 2;
					break;

				case 2:
					if (Msg::promptMsg(Lang::get("EXIT_GAME_PROMPT"))) {
						Gui::screenBack(true);
						return;
					}
			}
		}

		if (hDown & KEY_TOUCH) {
			if (touching(touch, subBtn[0])) {
				this->subSel = 0;
				this->subMode = 1;

			} else if (touching(touch, subBtn[1])) {
				this->subSel = 0;
				this->subMode = 2;

			} else if (touching(touch, subBtn[2])) {
				if (Msg::promptMsg(Lang::get("EXIT_GAME_PROMPT"))) {
					Gui::screenBack(true);
					return;
				}
			}
		}

		/* Loading a slot mode. */
	} else if (this->subMode == 1) {
		if (hDown & KEY_B) {
			this->subSel = 0;
			this->subMode = 0;
		}

		if (hDown & KEY_A) {
			Msg::DisplayMsg(Lang::get("LOADING_SLOT"));
			
			if (this->currentGame->LoadSlot(this->subSel)) Msg::DisplayWaitMsg(Lang::get("LOADED_SLOT_SCFLY"));
			else Msg::DisplayWaitMsg(Lang::get("LOADING_SLOT_ERROR"));
			this->Refresh();
		}

		if (hDown & KEY_TOUCH) {
			for (int i = 0; i < 3; i++) {
				if (touching(touch, subBtn[i])) {
					Msg::DisplayMsg(Lang::get("LOADING_SLOT"));
			
					if (this->currentGame->LoadSlot(i)) Msg::DisplayWaitMsg(Lang::get("LOADED_SLOT_SCFLY"));
					else Msg::DisplayWaitMsg(Lang::get("LOADING_SLOT_ERROR"));
					this->Refresh();
				}
			}
		}

		/* Saving a slot mode. */
	} else if (this->subMode == 2) {
		if (hDown & KEY_B) {
			this->subSel = 0;
			this->subMode = 0;
		}

		if (hDown & KEY_A) {
			Msg::DisplayMsg(Lang::get("SAVING_SLOT"));
			this->currentGame->SaveSlot(this->subSel);
			Msg::DisplayWaitMsg(Lang::get("SAVED_SLOT_SCFLY"));
		}

		if (hDown & KEY_TOUCH) {
			for (int i = 0; i < 3; i++) {
				if (touching(touch, subBtn[i])) {
					Msg::DisplayMsg(Lang::get("SAVING_SLOT"));
					this->currentGame->SaveSlot(i);
					Msg::DisplayWaitMsg(Lang::get("SAVED_SLOT_SCFLY"));
				}
			}
		}
	}
}

void GameScreen::AILogic() {
	int index = this->currentGame->AIPlay();

	if (index != -1) {
		this->rowSelection = index;
		this->Refresh();

		if (this->currentGame->Playable(this->rowSelection, this->currentGame->GetCurrentPlayer()).first == GameResult::Good) {
			this->drop();
			std::pair<bool, ChipMatches> tempMatch;
			tempMatch = this->currentGame->CheckMatches(this->currentGame->GetCurrentPlayer());

			if (tempMatch.first) {
				this->matches = tempMatch.second;
				this->results = GameRes::Over;
				return;

			} else {
				if (this->currentGame->GetCurrentPlayer() == 1) {
					this->currentGame->SetCurrentPlayer(2);

				} else {
					this->currentGame->SetCurrentPlayer(1);
				}
			}

			this->rowSelection = 3;
			this->Refresh();

			if (this->currentGame->IsOver()) {
				this->results = GameRes::AllUsed;
				return;
			}
		}

	} else {
		Msg::DisplayWaitMsg("Oh no, not playable!");
	}
}

void GameScreen::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (this->isSub) {
		this->subLogic(hDown, hHeld, touch);

	} else {
		u32 hRepeat = hidKeysDownRepeat();

		/* In case the game is *not* over. */
		if (this->results == GameRes::NotOver) {
			if (this->currentGame->GetCurrentPlayer() == 2) {
				this->AILogic();

			} else {
				if (hDown & KEY_START) {
					this->isSub = true;
				}

				if (hDown & KEY_A) {
					if (this->currentGame->Playable(this->rowSelection, this->currentGame->GetCurrentPlayer()).first == GameResult::Good) {
						this->drop();
						std::pair<bool, ChipMatches> tempMatch;
						tempMatch = this->currentGame->CheckMatches(this->currentGame->GetCurrentPlayer());

						this->rowSelection = 3;
						this->Refresh();

						if (tempMatch.first) {
							this->matches = tempMatch.second;
							this->results = GameRes::Over;
							return;

						} else {
							if (this->currentGame->GetCurrentPlayer() == 1) {
								this->currentGame->SetCurrentPlayer(2);

							} else {
								this->currentGame->SetCurrentPlayer(1);
							}
						}

						if (this->currentGame->IsOver()) {
							this->results = GameRes::AllUsed;
							return;
						}
					}
				}

				if (hRepeat & KEY_RIGHT) {
					if (this->rowSelection < 6) {
						this->rowSelection++;
						this->Refresh();
					}
				}

				if (hRepeat & KEY_LEFT) {
					if (this->rowSelection > 0) {
						this->rowSelection--;
						this->Refresh();
					}
				}

				if (hRepeat & KEY_R) {
					this->rowSelection = 6;
					this->Refresh();
				}

				if (hRepeat & KEY_L) {
					this->rowSelection = 0;
					this->Refresh();
				}
			}

			/* In case a game is over. */
		} else {
			/* Logic if a game is won. */
			if (hDown & KEY_A) {
				if (this->currentGame->GetWins(this->currentGame->GetCurrentPlayer()) < this->currentGame->GetWinAmount()) {
					this->clearField();

					return;

				} else {
					char message [100];
					snprintf(message, sizeof(message), Lang::get("GAME_RESULT").c_str(), this->getName(this->currentGame->GetCurrentPlayer()).c_str(),
					 this->getName(1).c_str(), this->currentGame->GetWins(1), this->getName(2).c_str(), this->currentGame->GetWins(2));
					Msg::DisplayWaitMsg(message);

					Gui::screenBack(true);
					return;
				}
			}
		}
	}
}