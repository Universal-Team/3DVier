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

#include "game.hpp"
#include <unistd.h>

Game::Game(bool load, uint8_t slot) {
	bool couldImport = false;

	if (load) {
		couldImport = this->LoadSlot(slot);
		if (couldImport) return;
	}
	
	this->StartFromScratch();
}

/* Load from a slot. */
bool Game::LoadSlot(uint8_t slot) {
	if (access(Slots[slot].c_str(), F_OK) != 0) return false;
	if (slot > 2) return false;

	if (!this->gameData) this->gameData = std::unique_ptr<uint8_t[]>(new uint8_t[GAME_DATA_SIZE]);

	FILE *file = fopen(Slots[slot].c_str(), "r");

	fread(this->gameData.get(), 1, GAME_DATA_SIZE, file);
	fclose(file);
	return true;
}

/* Save to a slot. */
void Game::SaveSlot(uint8_t slot) {
	if (slot > 2) return;

	FILE *file = fopen(Slots[slot].c_str(), "w");

	fwrite(this->gameData.get(), 1, GAME_DATA_SIZE, file);
	fclose(file);
}

/* Restart a game, but keep win amount + wins. */
void Game::RestartGame() {
	this->gameData.get()[GAME_CURRENT_PLAYER] = 1; // Player 1's turn.

	/* Field data. */
	for (uint8_t i = 0x0; i < GAME_FIELD_SIZE; i++) {
		gameData.get()[i + GAME_FIELD_START] = 0x0; // Empty positions.
	}

	/* Available Chips. */
	for (uint8_t i = 0; i < GAME_AVAILABLE_CHIPS_SIZE; i++) {
		this->gameData.get()[i + GAME_AVAILABLE_CHIPS_START] = 0x15; // 21 chips each player available.
	}
}


/* Start a new game from scratch. */
void Game::StartFromScratch() {
	if (this->gameData) this->gameData = nullptr;
	this->gameData = std::unique_ptr<uint8_t[]>(new uint8_t[GAME_DATA_SIZE]);


	this->gameData.get()[GAME_CURRENT_PLAYER] = 1; // Player 1's turn.

	/* Field data. */
	for (uint8_t i = 0x0; i < GAME_FIELD_SIZE; i++) {
		this->gameData.get()[i + GAME_FIELD_START] = 0x0; // Empty positions.
	}

	/* Wins. */
	for (uint8_t i = 0; i < GAME_WIN_SIZE; i++) {
		this->gameData.get()[i + GAME_WIN_START] = 0x0;
	}

    this->gameData.get()[GAME_WIN_AMOUNT] = 3; // Win amount: 3.

	/* Available Chips. */
	for (uint8_t i = 0; i < GAME_AVAILABLE_CHIPS_SIZE; i++) {
		this->gameData.get()[i + GAME_AVAILABLE_CHIPS_START] = 0x15; // 21 chips each player available.
	}
}


/* Set a chip and return the result. */
std::pair<GameResult, uint8_t > Game::Playable(uint8_t slot, uint8_t player) {
	if (!this->gameData) return {GameResult::NoData, 0};

	/* Check, if size is larger than 0. */
	if (this->gameData.get()[GAME_AVAILABLE_CHIPS_START + (player - 1)] > 0) {

		/* Check, if slot is not larger than 7 / 6. */
		if (slot < 7) {

			if (this->gameData.get()[GAME_FIELD_START + slot] == 0) {
				return {GameResult::Good, slot};
			}

			if (this->gameData.get()[GAME_FIELD_START + slot + 7] == 0) {
				return {GameResult::Good, slot + 7};
			}

			if (this->gameData.get()[GAME_FIELD_START + slot + 14] == 0) {
				return {GameResult::Good, slot + 14};
			}

			if (this->gameData.get()[GAME_FIELD_START + slot + 21] == 0) {
				return {GameResult::Good, slot + 21};
			}

			if (this->gameData.get()[GAME_FIELD_START + slot + 28] == 0) {
				return {GameResult::Good, slot + 28};
			}

			if (this->gameData.get()[GAME_FIELD_START + slot + 35] == 0) {
				return {GameResult::Good, slot + 35};
			}

		} else {
			return {GameResult::InvalidRow, 0};
		}

	} else {
		return {GameResult::NoChips, 0};
	}

	return {GameResult::AllUsed, 0};
}

void Game::Play(uint8_t slot, uint8_t player) {
	std::pair<GameResult, uint8_t> result = {GameResult::NoData, 0};

	result = this->Playable(slot, player);

	if (result.first == GameResult::Good) {
		this->gameData.get()[GAME_FIELD_START + result.second] = player;
		this->gameData.get()[GAME_AVAILABLE_CHIPS_START + (player - 1)]--;
	}
}

/* Get a chip from a slot of the gamefield. */
uint8_t Game::GetChip(uint8_t slot) {
	if (!this->gameData) return 0;

	return this->gameData.get()[GAME_FIELD_START + slot];
}

/* Get all available chips from a player. */
uint8_t Game::GetAvailableChips(uint8_t player) {
	if (!this->gameData) return 0;
	return this->gameData.get()[GAME_AVAILABLE_CHIPS_START + (player - 1)];
}

/* Check, if all chips are used. */
bool Game::IsOver() {
	if (!this->gameData) return true;

	/* Check if both player have 0 slots. */
	if (this->gameData.get()[GAME_AVAILABLE_CHIPS_START] == 0 && this->gameData.get()[GAME_AVAILABLE_CHIPS_START + 0x1] == 0) return true;
	else return false;
}

/* Check, if already won. */
std::pair<bool, ChipMatches> Game::CheckMatches(uint8_t player) {
	std::pair<bool, ChipMatches> result = {false, {0, 0, 0, 0}};
	if (!this->gameData) return result;

	for(uint8_t i = 0; i < (uint8_t)posMatches.size(); i++) {
		if (this->gameData.get()[GAME_FIELD_START + posMatches[i].Pos1] == player && this->gameData.get()[GAME_FIELD_START + posMatches[i].Pos2] == player
		 && this->gameData.get()[GAME_FIELD_START + posMatches[i].Pos3] == player && this->gameData.get()[GAME_FIELD_START + posMatches[i].Pos4] == player) {
			result.first = true;
			result.second = {posMatches[i].Pos1, posMatches[i].Pos2, posMatches[i].Pos3, posMatches[i].Pos4};
			break;
		}
	}

	/* Cause it matches, give a ++ to the specific player. (+1 win.) */
	if (result.first) {
		this->gameData.get()[GAME_WIN_START + (player - 1)]++;
	}

	return result;
}

uint8_t Game::GetCurrentPlayer() { return this->gameData.get()[GAME_CURRENT_PLAYER]; }
uint8_t Game::GetWins(uint8_t player) { return this->gameData.get()[GAME_WIN_START + player - 1]; }
uint8_t Game::GetWinAmount() { return this->gameData.get()[GAME_WIN_AMOUNT]; }
void Game::SetCurrentPlayer(uint8_t player) { this->gameData.get()[GAME_CURRENT_PLAYER] = player; }