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

#include "ai.hpp"
#include <cassert>

/* Inside bounds checks. */
static bool inside(int y, int x) {
	return y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH;
}

/* Get the score board. */
static int ScoreBoard(const Game& game) {
	int counters[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};


	/* Horizontal spans. */
	for(int y = 0; y < HEIGHT; y++) {
		int score = game.MiniMaxHelper(y, 0) + game.MiniMaxHelper(y, 1) + game.MiniMaxHelper(y, 2);
		for(int x = 3; x < WIDTH; x++) {
			assert(inside(y, x));
			score += game.MiniMaxHelper(y, x);
			counters[score + 4]++;
			assert(inside(y, x - 3));
			score -= game.MiniMaxHelper(y, x - 3);
		}
	}

	/* Vertical spans. */
	for(int x = 0; x < WIDTH; x++) {
		int score = game.MiniMaxHelper(0, x) + game.MiniMaxHelper(1, x) + game.MiniMaxHelper(2, x);
		for(int y = 3; y < HEIGHT; y++) {
			assert(inside(y, x));
			score += game.MiniMaxHelper(y, x);
			counters[score + 4]++;
			assert(inside(y - 3, x));
			score -= game.MiniMaxHelper(y - 3, x);
		}
	}

	/* Down-right (and up-left) diagonals. */
	for(int y = 0; y < HEIGHT - 3; y++) {
		for(int x = 0; x < WIDTH - 3; x++) {
			int score = 0;
			for(int idx = 0; idx < 4; idx++) {
				int yy = y + idx;
				int xx = x + idx;
				assert(inside(yy, xx));
				score += game.MiniMaxHelper(yy, xx);
			}

			counters[score + 4]++;
		}
	}

	/* Up-right (and down-left) diagonals. */
	for(int y = 3; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH - 3; x++) {
			int score = 0;
			for(int idx = 0; idx < 4; idx++) {
				int yy = y - idx;
				int xx = x + idx;
				assert(inside(yy, xx));
				score += game.MiniMaxHelper(yy, xx);
			}

			counters[score + 4]++;
		}
	}

	/* Do we need that at all? */
	if (counters[0] != 0) {
		return Player1Wins;

	} else if (counters[8] != 0) {
		return Player2Wins;

	} else {
		/* Some fancy stuff, i guess. */
		return
			counters[5] + 2 * counters[6] + 5 * counters[7] -
			counters[3] - 2 * counters[2] - 5 * counters[1];
	}
}

/* Do Player to chip conversion for the methods. */
static int conversion(uint8_t player) {
	if (player == 1) return -1;
	else return 1;
}

/* The MiniMax method. */
void AI::MiniMax(bool maximizeOrMinimize, int player, int depth, int &move, int &score, Game &game, bool ab) {
	int bestScore = maximizeOrMinimize ? Player1Wins : Player2Wins;
	int bestMove =-1;

	/* Checking... */
	for (int column = 0; column < WIDTH; column++) {
		if (game.MiniMaxHelper(0, column) != EMPTY) continue; // If that is not empty, we'll continue with our checks.
		const int rowFilled = game.MiniMaxRowHelper(column, player);
		if (rowFilled == -1) continue; // If our row is full, we'll continue?

		int s = ScoreBoard(game);

		if (s == (maximizeOrMinimize ? Player2Wins : Player1Wins)) {
			bestMove = column;
			bestScore = s;
			game.SetChip(rowFilled, column, EMPTY); // Set the chip back to empty.
			break;
		}

		int moveInner, scoreInner;

		/* If our depth is larger than 1, we'll do the check again. That also means, the larger it is, the longer the loading times. */
		if (depth > 1) {
			AI::MiniMax(!maximizeOrMinimize, player == 1 ? 2 : 1, depth - 1, moveInner, scoreInner, game, ab);

		} else {
			moveInner = -1;
			scoreInner = s;
		}

		game.SetChip(rowFilled, column, EMPTY); // Set the chip back to empty.

		if (ab) {
			/* when loss is certain, avoid forfeiting the match, by shifting scores by depth. */
			if (scoreInner == Player2Wins || scoreInner == Player1Wins) scoreInner -= depth * conversion(player);
		}

		if (maximizeOrMinimize) {
			if (scoreInner >= bestScore) {
				bestScore = scoreInner;
				bestMove = column;
			}

		} else {
			if (scoreInner <= bestScore) {
				bestScore = scoreInner;
				bestMove = column;
			}
		}
	}

	move = bestMove;
	score = bestScore;
}

/* The ABMegaMax method. On default, this is used with depth == 5. */
void AI::ABNegaMax(int player, int depth, int& move, int& score, int a, int b, Game& game) {
	int bestScore = a;	
	int bestMove = -1;

	for (int column = 0; column < WIDTH; column++) {
		if (game.MiniMaxHelper(0, column) != EMPTY) continue; // If that is not empty, we'll continue with our checks.
		const int rowFilled = game.MiniMaxRowHelper(column, player);
		if (rowFilled == -1) continue; // If our row is full, we'll continue?

		int s = ScoreBoard(game);

		if (player == 2 && s == Player2Wins) {
			bestMove = column;
			bestScore = s;
			game.SetChip(rowFilled, column, EMPTY); // Set the chip back to empty.
			break;
		}

		int moveInner, scoreInner;

		/* If our depth is larger than 1, we'll do the check again. That also means, the larger it is, the longer the loading times. */
		if (depth > 1) {
			AI::ABNegaMax(player == 2 ? 1 : 2, depth - 1, moveInner, scoreInner, -b, -bestScore, game);

		} else {
			moveInner = -1;
			scoreInner = s;
		}

		game.SetChip(rowFilled, column, EMPTY); // Set the chip back to empty.

		if (scoreInner > bestScore)  {
			bestScore = scoreInner;

			if (bestScore >= b) break; // pruning.

			/* write best move on top of tree. */
			if (depth == 5) { // 5 is the max depth. Possibly change that?
				bestMove = column;
			}
		}
	}

	move = bestMove;
	score = bestScore * -1;
}

/* The NegaMax method. */
void AI::NegaMax(int player, int depth, int& move, int& score, Game& game) {
	int bestScore = Player1Wins;
	int bestMove = -1;

	for (int column = 0; column < WIDTH; column++) {
		if (game.MiniMaxHelper(0, column) != EMPTY) continue; // If that is not empty, we'll continue with our checks.
		const int rowFilled = game.MiniMaxRowHelper(column, player);
		if (rowFilled == -1) continue; // If our row is full, we'll continue?

		int s = ScoreBoard(game);

		if (player == 2 && s == Player2Wins) {
			bestMove = column;
			bestScore = s;
			game.SetChip(rowFilled, column, EMPTY); // Set the chip back to empty.
			break;
		}

		int moveInner, scoreInner;

		/* If our depth is larger than 1, we'll do the check again. That also means, the larger it is, the longer the loading times. */
		if (depth > 1) {
			AI::NegaMax(player == 2 ? 1 : 2, depth - 1, moveInner, scoreInner, game);

		} else {
			moveInner = -1;
			scoreInner = s;
		}

		game.SetChip(rowFilled, column, EMPTY); // Set the chip back to empty.

		if (scoreInner > bestScore) {
			bestScore = scoreInner;

			if (depth == 5) { // 5 is the max depth. Possibly change that?
				bestMove = column;
			}
		}

	}

	move = bestMove;
	score = bestScore * -1;
}