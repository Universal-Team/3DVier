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

#include "game.hpp"

Game::Game() {
	this->clearField();
	this->currentPlayer(1); // For now Player 1 by default.
}

void Game::clearField() {
	this->field.clear();
	this->count = 0;
	for (int i = 0; i < 42; i++) {
		this->field.push_back({0});
	}
}


bool Game::setChip(const int Position, const int Player) {
	// Here we check if the Position is already used.
	if (this->field[Position] != 0) {
		return false;
	}

	// Correct Row just in case.
	this->field[Position] = Player;
	this->count++;
	return true;
}

bool Game::canDrop(const int Position) {
	// Here we check if the Position is already used.
	if (this->field[Position] != 0) {
		return false;
	}
	return true;
}

bool Game::checkMatches(int Player) {
	bool matches = false;
	for(uint i = 0; i < posMatches.size(); i++) {
		if (this->field[posMatches[i].Pos1] == Player && this->field[posMatches[i].Pos2] == Player && this->field[posMatches[i].Pos3] == Player && this->field[posMatches[i].Pos4] == Player) {
			matches = true;
		}
	}

	// Cause it matches, give a ++ to the specific player. (+1 win.)
	if (matches) {
		if (Player == 1)	this->v_p1Wins++;
		else				this->v_p2Wins++;
	}
	return matches;
}

bool Game::isUsed(const int Position) const {
	if (this->field[Position] != 0)	return true;
	else							return false;
}

bool Game::allUsed() const {
	if (this->count > 41)	return true;
	else						return false;
}

const int Game::getScore(int Player) {
	if (Player == 1)	return this->v_p1Wins;
	else				return this->v_p2Wins;
}