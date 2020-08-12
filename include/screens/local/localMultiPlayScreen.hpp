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

#ifndef _3DVIER_LOCAL_MULTI_PLAY_SCREEN_HPP
#define _3DVIER_LOCAL_MULTI_PLAY_SCREEN_HPP

#include "common.hpp"
#include "game.hpp"
#include "localNetwork.hpp"
#include "structs.hpp"

#include <vector>

class LocalMultiPlayScreen : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	LocalMultiPlayScreen(std::shared_ptr<LocalNetwork> &network);
private:
	std::shared_ptr<Game> currentGame; // Our game pointer.
	PlayPackage playPkg = {0, 0, 1, false};
	std::shared_ptr<LocalNetwork> room;
	int rowSelection = 3; // To select the Row.
	int dropSelection = 0; // Where to drop.
	bool dropped = false; // Chip is dropped.
	int dropPos = 0; // Chip animation position.
	int delay = 60;
	void displayAnimation(int dropSel); // The Drop animation!
	void Refresh(); // Refresh the dropSelection.
	void clearField(); // Clear the field and make ready for next round.
	int Position; // Needed?
	int lastPlayer = 1; // Probably not needed at all.
	void sendPlay(); // Send your play.
	void sendLastPlay(); // Basically refresh the receive console using this.

	// Player Names etc.
	int avatar1, avatar2, winAmount;
	std::string p1Name, p2Name;
	std::string getName(int Player) const;
	int getAvatar(int Player) const;

	void pullNames();

	const std::vector<ChipIcn> GamePos = {
		// First Row -> 0-6.
		{115, 205},
		{155, 205},
		{195, 205},
		{235, 205},
		{275, 205},
		{315, 205},
		{355, 205},
		// Second Row -> 7-13.
		{115, 165},
		{155, 165},
		{195, 165},
		{235, 165},
		{275, 165},
		{315, 165},
		{355, 165},
		// Third Row -> 14-20.
		{115, 125},
		{155, 125},
		{195, 125},
		{235, 125},
		{275, 125},
		{315, 125},
		{355, 125},
		// Fourth Row -> 21-27.
		{115, 85},
		{155, 85},
		{195, 85},
		{235, 85},
		{275, 85},
		{315, 85},
		{355, 85},
		// Fifth Row -> 28-35.
		{115, 45},
		{155, 45},
		{195, 45},
		{235, 45},
		{275, 45},
		{315, 45},
		{355, 45},
		// Sixth Row -> 36-42.
		{115, 5},
		{155, 5},
		{195, 5},
		{235, 5},
		{275, 5},
		{315, 5},
		{355, 5}
	};
};

#endif