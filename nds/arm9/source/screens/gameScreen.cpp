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

#include "_DSVier_Helper.hpp"
#include "input.hpp"
#include "gameScreen.hpp"
#include "selector.hpp"

extern bool Buttontouching(ButtonStruct button);
extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern std::unique_ptr<Selector> selector;
extern Image BG, Field, Chip[2], ChipSelector[2], characters[8];

/* Constructor. */
GameScreen::GameScreen() {
	this->avatar1 = Overlays::AvatarOverlay(Lang::get("PLAYER1_CHAR"));
	this->avatar2 = Overlays::AvatarOverlay(Lang::get("PLAYER2_CHAR"));
	this->player1 = Input::getLine(Lang::get("PLAYER1_NAME"), 10, Lang::get("PLAYER") + " " + std::to_string(1), false, false);
	this->player2 = Input::getLine(Lang::get("PLAYER2_NAME"), 10, Lang::get("PLAYER") + " " + std::to_string(2), false, true);
	this->againstAI = Msg::promptMsg(Lang::get("PLAY_AGAINST_AI"));

	this->currentGame = std::make_unique<Game>();

	/* Initialize the gamefield chips. */
	for (int i = 0; i < 42; i++) {
		this->indexes[i] = maxSpriteMain + i;
		initSprite(true, SpriteSize_32x32, this->indexes[i]);
		prepareSprite(this->indexes[i], true, GamePos[i].X - 1, GamePos[i].Y - 1, 0);
		setSpriteVisibility(this->indexes[i], true, false);
	}

	/* Initialize the Selector chips. */
	for (int i = 0; i < 2; i++) {
		this->selectorChip[i] = maxSpriteMain + 42 + i;
		initSprite(true, SpriteSize_32x32, this->selectorChip[i]);
		prepareSprite(this->selectorChip[i], true, 5, 5, 0);
		setSpriteVisibility(this->selectorChip[i], true, false);

		fillSpriteImage(this->selectorChip[i], true, 32, 0, 0, _DSVier_Helper::GetChipSelector(i + 1));
	}

	/* Initialize the Matching Four chips. */
	for (int i = 0; i < 4; i++) {
		this->matchingFour[i] = maxSpriteMain + 44 + i;
		initSprite(true, SpriteSize_32x32, this->matchingFour[i]);
		prepareSprite(this->matchingFour[i], true, 5, 5, 0);
		setSpriteVisibility(this->matchingFour[i], true, false);
	}

	/* Initialize the drop chip. */
	this->dropChip = maxSpriteMain + 48;
	initSprite(true, SpriteSize_32x32, this->dropChip);
	prepareSprite(this->dropChip, true, 0, 0, 0);
	setSpriteVisibility(this->dropChip, true, false);


	setSpriteVisibility(this->selectorChip[0], true, true);
	this->Refresh();
}

void GameScreen::updateField() {
	/* Update Gamefield. */
	for (int i = 0; i < 42; i++) {
		if (this->currentGame->GetChip(i) != 0) {
			fillSpriteImage(this->indexes[i], true, 32, 0, 0, _DSVier_Helper::GetChipImage(this->currentGame->GetChip(i)));
			setSpriteVisibility(this->indexes[i], true, true);
		}
	}

	oamUpdate(&oamMain);
}


void GameScreen::destruct() {
	/* Free allocated Gamefield chips. */
	for (int i = 0; i < 42; i++) {
		if (spritesMain[this->indexes[i]].gfx) {
			oamFreeGfx(&oamMain, spritesMain[this->indexes[i]].gfx);
			setSpriteVisibility(this->indexes[i], true, false);
		}
	}

	/* Free allocated selectors. */
	for (int i = 0; i < 2; i++) {
		if (spritesMain[this->selectorChip[i]].gfx) {
			oamFreeGfx(&oamMain, spritesMain[this->selectorChip[i]].gfx);
			setSpriteVisibility(this->selectorChip[i], true, false);
		}
	}

	/* Free allocated Matching Four selectors. */
	for (int i = 0; i < 4; i++) {
		if (spritesMain[this->matchingFour[i]].gfx) {
			oamFreeGfx(&oamMain, spritesMain[this->matchingFour[i]].gfx);
			setSpriteVisibility(this->matchingFour[i], true, false);
		}
	}

	/* Free allocated drop chip. */
	if (spritesMain[this->dropChip].gfx) {
		oamFreeGfx(&oamMain, spritesMain[this->dropChip].gfx);
		setSpriteVisibility(this->dropChip, true, false);
	}

	updateOam();
}

/* Hide the gamefield. */
void GameScreen::hideField() {
	/* Hide Gamefield chips. */
	for (int i = 0; i < 42; i++) {
		setSpriteVisibility(this->indexes[i], true, false);
	}

	/* Hide selector chips. */
	for (int i = 0; i < 2; i++) {
		setSpriteVisibility(this->selectorChip[i], true, false);
	}

	oamUpdate(&oamMain);
}

/* Return Player names. */
std::string GameScreen::GetName(int player) const {
	if (player == 1) return this->player1;
	else return this->player2;
}

/* Main Draw, which wouldn't need a clear. */
void GameScreen::Draw(void) const {
	drawImage(26, 8.5, BG, true, false);
	drawImage(26, 8.5, Field, true, true);
	Gui::DrawBottom(true);

	/* Draw Chips. */
	drawImage(0, 20, Chip[0], false, false);
	drawImage(0, 140, Chip[0], false, false);

	drawImage(224, 20, Chip[1], false, false);
	drawImage(224, 140, Chip[1], false, false);

	/* Draw Characters. */
	drawImage(16, 22, characters[this->avatar1], false, false);
	drawImage(129, 22, characters[this->avatar2], false, false, 0x10);

	/* Draw Player names. */
	printTextTinted(this->player1, TextColor::black, 35, 140, false, false);
	printTextTinted(this->player2, TextColor::black, 140, 140, false, false);

	this->updateTexts(); // Update the texts.
}

/* Text draw, which needs to be updated on every play for the slot amount + wins. */
void GameScreen::updateTexts(void) const {
	Gui::clearScreen(false, true); // Clear bottom layer true for Text redraws.

	/* If the game is over, show the winner. */
	if (this->results == GameRes::Over) {
		char message [100];
		snprintf(message, sizeof(message), Lang::get("PLAYER_WON_ROUND").c_str(), this->GetName(this->currentGame->GetCurrentPlayer()).c_str());
		printTextCentered(message, 0, 1, false, true);

	/* If all slots are being used, show that all slots are used. */
	} else if (this->results == GameRes::AllUsed) {
		printTextCentered(Lang::get("ALL_SLOTS_USED"), 0, 1, false, true);
	}

	if (this->results != GameRes::NotOver) printTextCentered(Lang::get("A_CONTINUE"), 0, 174, false, true); // Show A to continue, if it's over.

	/* Display Chip amount + Wins for both players. */
	printTextCenteredTinted(std::to_string(this->currentGame->GetAvailableChips(1)), TextColor::black, 3 - 128 + (28 / 2), 28, false, true);
	printTextCenteredTinted(std::to_string(this->currentGame->GetWins(1)), TextColor::black, 3 - 128 + (28 / 2), 148, false, true);

	printTextCenteredTinted(std::to_string(this->currentGame->GetAvailableChips(2)), TextColor::black, 227 - 128 + (28 / 2), 28, false, true);
	printTextCenteredTinted(std::to_string(this->currentGame->GetWins(2)), TextColor::black, 227 - 128 + (28 / 2), 148, false, true);
}

/* Drop the chip into the slot. */
void GameScreen::drop() {
	this->dropped = true;
	int dropPos = 2, downSpeed = Settings::dropSpeed(), frameCount = 0, frameDrops = 0, speedPlus = Settings::speedPlus();

	/* Initialize the drop chip. */
	fillSpriteImage(this->dropChip, true, 32, 0, 0, _DSVier_Helper::GetChipImage(this->currentGame->GetCurrentPlayer()));
	setSpriteVisibility(this->dropChip, true, true);
	setSpritePosition(this->dropChip, true, GamePos[this->rowSelection].X-1, dropPos);
	oamUpdate(&oamMain);

	while(this->dropped) {
		if (dropPos < GamePos[this->dropSelection].Y - 1) {
			frameCount++;

			if (frameCount == 5) {
				frameDrops += speedPlus;
				frameCount = 0;
			}

			dropPos = dropPos + downSpeed + frameDrops;
			
			setSpritePosition(this->dropChip, true, GamePos[this->rowSelection].X-1, dropPos);
			oamUpdate(&oamMain);
			swiWaitForVBlank();

		} else {
			/* Make the drop chip invisible. */
			setSpriteVisibility(this->dropChip, true, false);
			oamUpdate(&oamMain);
			this->dropped = false;
		}
	}

	/* Play the chip and update field! */
	this->currentGame->Play(this->rowSelection, this->currentGame->GetCurrentPlayer());
	this->updateField();
}

/* Clear a game. */
void GameScreen::clearGame() {
	bool hasWon = true;
	int Pos = 0, frameCount = 0, frameDrops = 0, max = 35;

	int downSpeed = Settings::clearSpeed();
	int speedPlus = Settings::speedPlus();

	/* Hide matching four. */
	for (int i = 0; i < 4; i++) {
		setSpriteVisibility(this->matchingFour[i], true, false);
	}

	oamUpdate(&oamMain);

	/* Get the highest chip pos. */
	for (int i = 41; i >= 0; i--) {
		if (this->currentGame->GetChip(i) != 0) {
			/* Here we get the highest pos. */
			if (i < max) max = i;
		}
	}

	while(hasWon) {
		if (GamePos[max].Y + Pos < 192) {
			frameCount++;

			if (frameCount == 5) {
				frameDrops += speedPlus;
				frameCount = 0;
			}

			Pos = Pos + downSpeed + frameDrops;

			for (int i = 0; i < 42; i++) {
				/* Hide, when goes out of screen. */
				if (getSpriteInfo(this->indexes[i], true).y + Pos > 192) {
					setSpriteVisibility(this->indexes[i], true, false);

				} else {
					setSpritePosition(this->indexes[i], true, GamePos[i].X - 1, GamePos[i].Y - 1 + Pos);
				}
			}

			oamUpdate(&oamMain);
			swiWaitForVBlank();

		} else {
			hasWon = false;
		}
	}

	/* Clear Gamefield etc and make ready for next round. */
	for (int i = 0; i < 42; i++) {
		setSpritePosition(this->indexes[i], true, GamePos[i].X - 1, GamePos[i].Y - 1);
		setSpriteVisibility(this->indexes[i], true, false);
	}

	this->results = GameRes::NotOver;
	this->matches = {0, 0, 0, 0};
	this->currentGame->RestartGame();

	if (this->currentGame->GetCurrentPlayer() == 1) {
		setSpriteVisibility(this->selectorChip[0], true, true);
		setSpriteVisibility(this->selectorChip[1], true, false);

	} else {
		setSpriteVisibility(this->selectorChip[0], true, false);
		setSpriteVisibility(this->selectorChip[1], true, true);
	}

	oamUpdate(&oamMain);
	this->rowSelection = 3;
	this->Refresh();
	this->updateTexts();
}

void GameScreen::displaySub(void) const {
	Gui::clearScreen(false, true);
	Gui::DrawBottom(true);

	for (int i = 0; i < 3; i++) {
		drawRectangle(subPosMain[i].x, subPosMain[i].y, subPosMain[i].w, subPosMain[i].h, BUTTON_COLOR, false, true);
	}

	switch(this->subMode) {
		case 0:
			/* Mode 1: Main. */
			printTextCenteredTintedMaxW(Lang::get("LOAD_SLOT") + "...", subPosMain[0].w-5, 1, TextColor::white, subPosMain[0].x - 128 + (subPosMain[0].w/2), subPosMain[0].y + (subPosMain[0].h/2) - 10, false, true);
			printTextCenteredTintedMaxW(Lang::get("SAVE_SLOT") + "...", subPosMain[1].w-5, 1, TextColor::white, subPosMain[1].x - 128 + (subPosMain[1].w/2), subPosMain[1].y + (subPosMain[1].h/2) - 10, false, true);
			printTextCenteredTintedMaxW(Lang::get("EXIT_GAME") + "...", subPosMain[2].w-5, 1, TextColor::white, subPosMain[2].x - 128 + (subPosMain[2].w/2), subPosMain[2].y + (subPosMain[2].h/2) - 10, false, true);
			break;
		
		case 1:
			/* Mode 2: Load. */
			printTextCenteredTintedMaxW(Lang::get("LOAD_SLOT") + " " + std::to_string(1), subPosMain[0].w-5, 1, TextColor::white, subPosMain[0].x - 128 + (subPosMain[0].w/2), subPosMain[0].y + (subPosMain[0].h/2) - 10, false, true);
			printTextCenteredTintedMaxW(Lang::get("LOAD_SLOT") + " " + std::to_string(2), subPosMain[1].w-5, 1, TextColor::white, subPosMain[1].x - 128 + (subPosMain[1].w/2), subPosMain[1].y + (subPosMain[1].h/2) - 10, false, true);
			printTextCenteredTintedMaxW(Lang::get("LOAD_SLOT") + " " + std::to_string(3), subPosMain[2].w-5, 1, TextColor::white, subPosMain[2].x - 128 + (subPosMain[2].w/2), subPosMain[2].y + (subPosMain[2].h/2) - 10, false, true);
			break;

		case 2:
			/* Mode 3: Save. */
			printTextCenteredTintedMaxW(Lang::get("SAVE_SLOT") + " " + std::to_string(1), subPosMain[0].w-5, 1, TextColor::white, subPosMain[0].x - 128 + (subPosMain[0].w/2), subPosMain[0].y + (subPosMain[0].h/2) - 10, false, true);
			printTextCenteredTintedMaxW(Lang::get("SAVE_SLOT") + " " + std::to_string(2), subPosMain[1].w-5, 1, TextColor::white, subPosMain[1].x - 128 + (subPosMain[1].w/2), subPosMain[1].y + (subPosMain[1].h/2) - 10, false, true);
			printTextCenteredTintedMaxW(Lang::get("SAVE_SLOT") + " " + std::to_string(3), subPosMain[2].w-5, 1, TextColor::white, subPosMain[2].x - 128 + (subPosMain[2].w/2), subPosMain[2].y + (subPosMain[2].h/2) - 10, false, true);
			break;
	}
}

void GameScreen::subLogic(u16 hDown, touchPosition touch) {
	if (doUpdate) {
		selector->move(this->subPosMain[this->subSel].x, this->subPosMain[this->subSel].y);
		selector->update();
	}

	u16 hRepeat = keysDownRepeat();

	if (hRepeat & KEY_DOWN) {
		if (this->subSel < 2) {
			this->subSel++;
			doUpdate = true;
		}
	}

	if (hRepeat & KEY_UP) {
		if (this->subSel > 0) {
			this->subSel--;
			doUpdate = true;
		}
	}

	/* Main Mode. */
	if (this->subMode == 0) {
		if (hDown & KEY_B) {
			selector->hide();
			oamUpdate(&oamSub);
			this->isSub = false;
			this->Draw();
		}

		if (hDown & KEY_A) {
			switch(this->subSel) {
				case 0:
					/* Only allow, if has SD. */
					if (hasSD) {
						this->subSel = 0;
						this->subMode = 1;
						doUpdate = true;
						this->displaySub();
					}
					break;

				case 1:
					/* Only allow, if has SD. */
					if (hasSD) {
						this->subSel = 0;
						this->subMode = 2;
						doUpdate = true;
						this->displaySub();
					}
					break;

				case 2:
					this->destruct();
					Gui::screenBack();
					Gui::DrawScreen();
					doUpdate = true;
					return;
			}
		}

		if (hDown & KEY_TOUCH) {
			if (touching(touch, subPosMain[0])) {
				/* Only allow, if has SD. */
				if (hasSD) {
					this->subSel = 0;
					this->subMode = 1;
					doUpdate = true;
					this->displaySub();
				}

			} else if (touching(touch, subPosMain[1])) {
				/* Only allow, if has SD. */
				if (hasSD) {
					this->subSel = 0;
					this->subMode = 2;
					doUpdate = true;
					this->displaySub();
				}

			} else if (touching(touch, subPosMain[2])) {
				this->destruct();
				Gui::screenBack();
				Gui::DrawScreen();
				doUpdate = true;
				return;
			}
		}

		/* Loading a slot mode. */
	} else if (this->subMode == 1) {
		if (hDown & KEY_B) {
			this->subSel = 0;
			this->subMode = 0;
			doUpdate = true;
			this->displaySub();
		}

		if (hDown & KEY_A) {
			/* Hide field and selector. */
			this->hideField();
			selector->hide();
			oamUpdate(&oamSub);

			if (this->currentGame->LoadSlot(this->subSel)) Msg::DisplayWaitMsg(Lang::get("LOADED_SLOT_SCFLY"));
			else Msg::DisplayWaitMsg(Lang::get("LOADING_SLOT_ERROR"));

			this->Refresh();
			this->displaySub();
			this->updateField();

			if (this->currentGame->GetCurrentPlayer() == 1) {
				setSpriteVisibility(this->selectorChip[0], true, true);

			} else {
				setSpriteVisibility(this->selectorChip[1], true, true);
			}

			selector->show();
			updateOam();
		}

		if (hDown & KEY_TOUCH) {
			for (int i = 0; i < 3; i++) {
				if (touching(touch, subPosMain[i])) {
					/* Hide field and selector. */
					this->hideField();
					selector->hide();
					oamUpdate(&oamSub);

					if (this->currentGame->LoadSlot(i)) Msg::DisplayWaitMsg(Lang::get("LOADED_SLOT_SCFLY"));
					else Msg::DisplayWaitMsg(Lang::get("LOADING_SLOT_ERROR"));

					this->Refresh();
					this->displaySub();
					this->updateField();

					if (this->currentGame->GetCurrentPlayer() == 1) {
						setSpriteVisibility(this->selectorChip[0], true, true);

					} else {
						setSpriteVisibility(this->selectorChip[1], true, true);
					}
					
					selector->show();
					updateOam();
				}
			}
		}

		/* Saving a slot mode. */
	} else if (this->subMode == 2) {
		if (hDown & KEY_B) {
			this->subSel = 0;
			this->subMode = 0;
			doUpdate = true;
			this->displaySub();
		}

		if (hDown & KEY_A) {
			this->currentGame->SaveSlot(this->subSel);

			/* Hide field and selector. */
			selector->hide();
			oamUpdate(&oamSub);
			this->hideField();

			Msg::DisplayWaitMsg(Lang::get("SAVED_SLOT_SCFLY"));
			this->displaySub();
			this->updateField();

			if (this->currentGame->GetCurrentPlayer() == 1) {
				setSpriteVisibility(this->selectorChip[0], true, true);

			} else {
				setSpriteVisibility(this->selectorChip[1], true, true);
			}

			selector->show();
			updateOam();
		}

		if (hDown & KEY_TOUCH) {
			for (int i = 0; i < 3; i++) {
				if (touching(touch, subPosMain[i])) {
					this->currentGame->SaveSlot(i);

					/* Hide field and selector. */
					selector->hide();
					oamUpdate(&oamSub);
					this->hideField();

					Msg::DisplayWaitMsg(Lang::get("SAVED_SLOT_SCFLY"));
					this->displaySub();
					this->updateField();

					if (this->currentGame->GetCurrentPlayer() == 1) {
						setSpriteVisibility(this->selectorChip[0], true, true);

					} else {
						setSpriteVisibility(this->selectorChip[1], true, true);
					}

					selector->show();
					updateOam();
				}
			}
		}
	}
}


/* Refresh drop selection. */
void GameScreen::Refresh() {
	this->dropSelection = this->rowSelection;

	/* Here we set the "dropSelection". */
	for (int i = HEIGHT - 1; i >= 0; i--) {
		if (!this->currentGame->GetChipPos(i, this->rowSelection)) {
			this->dropSelection = this->rowSelection + (i * 7);
			break;
		}
	}
	
	if (this->currentGame->GetCurrentPlayer() == 1) {
		setSpritePosition(this->selectorChip[0], true, GamePos[this->dropSelection].X-1, GamePos[this->dropSelection].Y-1);
		oamUpdate(&oamMain);

	} else {
		setSpritePosition(this->selectorChip[1], true, GamePos[this->dropSelection].X-1, GamePos[this->dropSelection].Y-1);
		oamUpdate(&oamMain);
	}
}

/* Display the matching four chips. */
void GameScreen::SetMatchingFour() {
	/* Set Matching Four positions. */
	setSpritePosition(this->matchingFour[0], true, GamePos[this->matches.Pos1].X-1, GamePos[this->matches.Pos1].Y-1);
	setSpritePosition(this->matchingFour[1], true, GamePos[this->matches.Pos2].X-1, GamePos[this->matches.Pos2].Y-1);
	setSpritePosition(this->matchingFour[2], true, GamePos[this->matches.Pos3].X-1, GamePos[this->matches.Pos3].Y-1);
	setSpritePosition(this->matchingFour[3], true, GamePos[this->matches.Pos4].X-1, GamePos[this->matches.Pos4].Y-1);

	/* Make them visible and fill with image. */
	for (int i = 0; i < 4; i++) {
		fillSpriteImage(this->matchingFour[i], true, 32, 0, 0, _DSVier_Helper::GetChipSelector(this->currentGame->GetCurrentPlayer()));
		setSpriteVisibility(this->matchingFour[i], true, true);
	}

	oamUpdate(&oamMain);
}

void GameScreen::AILogic() {
	for (int i = 0; i < 2; i++) {
		setSpriteVisibility(this->selectorChip[i], true, false);
	}

	oamUpdate(&oamMain);

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
				this->SetMatchingFour();
				this->updateTexts();
				return;

			} else {
				setSpriteVisibility(this->selectorChip[1], true, false);
				this->currentGame->SetCurrentPlayer(1);
				setSpriteVisibility(this->selectorChip[0], true, true);
			}

			this->rowSelection = 3;
			this->Refresh();
			this->updateTexts();

			if (this->currentGame->IsOver()) {
				this->results = GameRes::AllUsed;
				return;
			}
		}

	} else {

		/* Hide field. */
		this->hideField();
		Msg::DisplayWaitMsg(Lang::get("AI_NOT_PLAYABLE"), true);
		this->updateField();

		if (this->currentGame->GetCurrentPlayer() == 1) {
			setSpriteVisibility(this->selectorChip[0], true, true);
		} else {
			setSpriteVisibility(this->selectorChip[1], true, true);
		}

		updateOam();
		this->clearGame();
	}
}

/* Game's Logic. */
void GameScreen::Logic(u16 hDown, touchPosition touch) {
	if (this->isSub) {
		this->subLogic(hDown, touch);

	} else {
		u16 hRepeat = keysDownRepeat();

		/* In case the game is *not* over. */
		if (this->results == GameRes::NotOver) {

			/* Do AI's Logic if second player. */
			if (this->currentGame->GetCurrentPlayer() == 2 && this->againstAI) {
				this->AILogic();

			} else {
				/* Go to sub menu. */
				if (hDown & KEY_START) {
					this->isSub = true;
					selector->show();
					doUpdate = true;
					this->displaySub();
				}

				if (hDown & KEY_A) {
					/* Check if playable. */
					if (this->currentGame->Playable(this->rowSelection, this->currentGame->GetCurrentPlayer()).first == GameResult::Good) {
						/* Drop the chip to the position. */
						this->drop();

						std::pair<bool, ChipMatches> tempMatch;
						tempMatch = this->currentGame->CheckMatches(this->currentGame->GetCurrentPlayer());

						/* In case we have 4 matches. */
						if (tempMatch.first) {
							this->matches = tempMatch.second;
							this->results = GameRes::Over;
							this->SetMatchingFour();
							this->updateTexts();
							return;

						/* if not, we're switching the Players & Selectors. */
						} else {
							if (this->currentGame->GetCurrentPlayer() == 1) {
								setSpriteVisibility(this->selectorChip[0], true, false);
								this->currentGame->SetCurrentPlayer(2);
								setSpriteVisibility(this->selectorChip[1], true, true);

							} else {
								setSpriteVisibility(this->selectorChip[1], true, false);
								this->currentGame->SetCurrentPlayer(1);
								setSpriteVisibility(this->selectorChip[0], true, true);
							}
						}

						this->rowSelection = 3;
						this->Refresh();
						this->updateTexts();

						/* Oh no.. game over. */
						if (this->currentGame->IsOver()) {
							this->results = GameRes::AllUsed;
							this->updateTexts();
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

		} else {
			/* Logic if a game is won. */
			if (hDown & KEY_A) {
				if (this->currentGame->GetWins(this->currentGame->GetCurrentPlayer()) < this->currentGame->GetWinAmount()) {
					this->clearGame();

				} else {
					this->destruct();
					char message [100];
					snprintf(message, sizeof(message), Lang::get("GAME_RESULT").c_str(), this->GetName(this->currentGame->GetCurrentPlayer()).c_str(),
					 this->GetName(1).c_str(), this->currentGame->GetWins(1), this->GetName(2).c_str(), this->currentGame->GetWins(2));
					Msg::DisplayWaitMsg(message, false); // Do not redraw this screen.

					selector->show();
					updateOam();
					Gui::screenBack();
					Gui::DrawScreen();
					return;
				}
			}
		}
	}
}