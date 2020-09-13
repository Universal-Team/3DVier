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
#include "gameScreen.hpp"
#include "selector.hpp"

extern bool Buttontouching(ButtonStruct button);
extern std::unique_ptr<Selector> selector;
extern Image BG, Field, Chip[2], ChipSelector[2];

/* Constructor. */
GameScreen::GameScreen() {
	this->currentGame = std::make_unique<Game>();

	for (int i = 0; i < 21; i++) {
		this->indexes[i] = maxSpriteMain + i;
		this->indexes[i] = initSprite(true, SpriteSize_32x32);
		prepareSprite(this->indexes[i], true, GamePos[i].X - 1, GamePos[i].Y - 1, 0);
		setSpriteVisibility(this->indexes[i], true, false);
		fillSpriteImage(this->indexes[i], true, 32, 0, 0, Chip[0]);
	}

	for (int i = 0; i < 21; i++) {
		this->indexes[21 + i] = maxSpriteMain + i;
		this->indexes[21 + i] = initSprite(true, SpriteSize_32x32);
		prepareSprite(this->indexes[21 + i], true, GamePos[21 + i].X - 1, GamePos[21 + i].Y - 1, 0);
		setSpriteVisibility(this->indexes[21 + i], true, false);
		fillSpriteImage(this->indexes[21 + i], true, 32, 0, 0, Chip[1]);
	}

	for (int i = 0; i < 2; i++) {
		this->selectorChip[i] = maxSpriteMain + i;
		this->selectorChip[i] = initSprite(true, SpriteSize_32x32);
		prepareSprite(this->selectorChip[i], true, 5, 5, 0);
		setSpriteVisibility(this->selectorChip[i], true, false);

		fillSpriteImage(this->selectorChip[i], true, 32, 0, 0, _DSVier_Helper::GetChipSelector(i + 1));
	}

	for (int i = 0; i < 4; i++) {
		this->matchingFour[i] = maxSpriteMain + i;
		this->matchingFour[i] = initSprite(true, SpriteSize_32x32);
		prepareSprite(this->matchingFour[i], true, 5, 5, 0);
		setSpriteVisibility(this->matchingFour[i], true, false);
	}


	setSpriteVisibility(this->selectorChip[0], true, true);
	this->Refresh();
}

void GameScreen::destruct() {
	/* Free allocated Chips. */
	for (int i = 0; i < 42; i++) {
		if (spritesMain[this->indexes[i]].gfx) {
			oamFreeGfx(&oamMain, spritesMain[this->indexes[i]].gfx);
			setSpriteVisibility(this->indexes[i], true, false);
			maxSpriteMain--;
		}
	}

	/* Free allocated selectors. */
	for (int i = 0; i < 2; i++) {
		if (spritesMain[this->selectorChip[i]].gfx) {
			oamFreeGfx(&oamMain, spritesMain[this->selectorChip[i]].gfx);
			setSpriteVisibility(this->selectorChip[i], true, false);
			maxSpriteMain--;
		}
	}

	/* Free allocated Matching selectors. */
	for (int i = 0; i < 4; i++) {
		if (spritesMain[this->matchingFour[i]].gfx) {
			oamFreeGfx(&oamMain, spritesMain[this->matchingFour[i]].gfx);
			setSpriteVisibility(this->matchingFour[i], true, false);
			maxSpriteMain--;
		}
	}

	updateOam();
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

	this->updateTexts();
}

/* Text draw, which needs to be updated on every play for the slot amount + wins. */
void GameScreen::updateTexts(void) const {
	Gui::clearScreen(false, true);

	if (this->results == GameRes::Over) {
		char message [100];
		snprintf(message, sizeof(message), Lang::get("PLAYER_WON_ROUND").c_str(), this->GetName(this->currentGame->GetCurrentPlayer()).c_str());
		printTextCentered(message, 0, 1, false, true);

	} else if (this->results == GameRes::AllUsed) {
		printTextCentered(Lang::get("ALL_SLOTS_USED"), 0, 1, false, true);
	}

	if (this->results != GameRes::NotOver) printTextCentered(Lang::get("A_CONTINUE"), 0, 178, false, true);

	/* Drawing Text. */
	printTextTinted(std::to_string(this->currentGame->GetAvailableChips(1)), TextColor::gray, 10, 34, false, true);
	printTextTinted(std::to_string(this->currentGame->GetWins(1)), TextColor::gray, 10, 155, false, true);

	printTextTinted(std::to_string(this->currentGame->GetAvailableChips(2)), TextColor::gray, 236, 34, false, true);
	printTextTinted(std::to_string(this->currentGame->GetWins(2)), TextColor::gray, 239, 155, false, true);
}

/* For now just set the chip to the right position. TODO; Animation? */
void GameScreen::MoveChip(int chip, int position) {
	if (this->currentGame->GetCurrentPlayer() == 1) {
		setSpritePosition(this->indexes[chip], true, GamePos[position].X-1, GamePos[position].Y-1);
		setSpriteVisibility(this->indexes[chip], true, true);
		oamUpdate(&oamMain);

	} else {
		setSpritePosition(this->indexes[21 + chip], true, GamePos[position].X-1, GamePos[position].Y-1);
		setSpriteVisibility(this->indexes[21 + chip], true, true);
		oamUpdate(&oamMain);
	}
}

void GameScreen::clearGame() {
	/* Clear Gamefield etc and make ready for next round. */
	for (int i = 0; i < 42; i++) {
		setSpriteVisibility(this->indexes[i], true, false);
	}

	for (int i = 0; i < 4; i++) {
		setSpriteVisibility(this->matchingFour[i], true, false);
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


/* Refresh drop selection. */
void GameScreen::Refresh() {
	/* Here we set the "dropSelection". */
	if (!this->currentGame->GetChip(this->rowSelection)) {
		this->dropSelection = this->rowSelection;
		goto refresh;
	}

	if (!this->currentGame->GetChip(this->rowSelection + 7)) {
		this->dropSelection = this->rowSelection + 7;
		goto refresh;
	}

	if (!this->currentGame->GetChip(this->rowSelection + 14)) {
		this->dropSelection = this->rowSelection + 14;
		goto refresh;
	}

	if (!this->currentGame->GetChip(this->rowSelection + 21)) {
		this->dropSelection = this->rowSelection + 21;
		goto refresh;
	}

	if (!this->currentGame->GetChip(this->rowSelection + 28)) {
		this->dropSelection = this->rowSelection + 28;
		goto refresh;
	}

	if (!this->currentGame->GetChip(this->rowSelection + 35)) {
		this->dropSelection = this->rowSelection + 35;
		goto refresh;
	}

	/* Display indicator on the last position, if row is full. */
	if (this->currentGame->GetChip(this->rowSelection + 35)) {
		this->dropSelection = this->rowSelection + 35;
		goto refresh;
	}

	refresh:
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
	setSpritePosition(this->matchingFour[0], true, GamePos[this->matches.Pos1].X-1, GamePos[this->matches.Pos1].Y-1);
	setSpritePosition(this->matchingFour[1], true, GamePos[this->matches.Pos2].X-1, GamePos[this->matches.Pos2].Y-1);
	setSpritePosition(this->matchingFour[2], true, GamePos[this->matches.Pos3].X-1, GamePos[this->matches.Pos3].Y-1);
	setSpritePosition(this->matchingFour[3], true, GamePos[this->matches.Pos4].X-1, GamePos[this->matches.Pos4].Y-1);

	for (int i = 0; i < 4; i++) {
		fillSpriteImage(this->matchingFour[i], true, 32, 0, 0, _DSVier_Helper::GetChipSelector(this->currentGame->GetCurrentPlayer()));
		setSpriteVisibility(this->matchingFour[i], true, true);
	}

	oamUpdate(&oamMain);
}


/* Game's Logic. */
void GameScreen::Logic(u16 hDown, touchPosition touch) {
	/* In case the game is *not* over. */
	if (this->results == GameRes::NotOver) {
		if (hDown & KEY_A) {
			/* Check if playable. */
			if (this->currentGame->Playable(this->rowSelection, this->currentGame->GetCurrentPlayer()).first == GameResult::Good) {

				/* Because playable, Play! */
				this->currentGame->Play(this->rowSelection, this->currentGame->GetCurrentPlayer());

				std::pair<bool, ChipMatches> tempMatch;
				tempMatch = this->currentGame->CheckMatches(this->currentGame->GetCurrentPlayer());

				/* Move the chip to the position. */
				this->MoveChip(20 - (this->currentGame->GetAvailableChips(this->currentGame->GetCurrentPlayer())), this->dropSelection);

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