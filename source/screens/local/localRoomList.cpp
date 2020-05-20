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

#include "config.hpp"
#include "init.hpp"
#include "localRoomList.hpp"
#include "localRoomScreen.hpp"

#include <vector>

extern std::unique_ptr<Config> config;
extern bool touching(touchPosition touch, Structs::ButtonPos button);

LocalRoomList::LocalRoomList() {
	Init::initUDS();
	this->refreshList();
}

LocalRoomList::~LocalRoomList() {
	Init::exitUDS();
}

void LocalRoomList::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), "3DVier - " + Lang::get("ROOM_LIST"), 400);
	Gui::DrawStringCentered(0, 215, 0.8f, config->textColor(), Lang::get("REFRESH_ROOMS"), 400);
	GFX::DrawBottom();

	for (int i = 0; i < NetworkListRoomsPerScreen; i++) {
		Gui::Draw_Rect(0, 40+(i*57), 320, 45, config->barColor());
	}


	Gui::DrawStringCentered(0, 0, 0.8f, config->textColor(), Lang::get("AVAILABLE_ROOMS") + std::to_string((int)this->rooms.size()));

	for(int i = this->roomsScroll; i < NetworkListRoomsPerScreen && i < (int)this->rooms.size(); i++) {
		if (this->selectedRoom == this->roomsScroll + i)	Gui::Draw_Rect(0, 40+(i*57), 320, 45, config->selectorColor());
		if (i >= (int)this->rooms.size())	break;

		int cleanI = i-this->roomsScroll;
		float y = 50 + cleanI * 57;

		std::string roomName = this->rooms[i]->getName();
		u8 roomPlayers = this->rooms[i]->getPlayerCount();
		char roomNameAndPlayers[128] = {0};
		snprintf(roomNameAndPlayers, 127, "%s: %d | %d", roomName.c_str(), roomPlayers, 2);
		Gui::DrawStringCentered(0, y, 0.7f, config->textColor(), roomNameAndPlayers);
	}
}

void LocalRoomList::joinRoom(int i) {
	if (static_cast<size_t>(i) >= this->rooms.size())	return;

	int selected = i + this->roomsScroll;
	if (this->selectedRoom == selected)	this->joinSelectedRoom();
	else								this->selectedRoom = selected;
}

void LocalRoomList::refreshList() {
	this->rooms.clear();
	this->rooms = getRooms();
	this->selectedRoom = 0;
	this->roomsScroll = 0;
}

void LocalRoomList::joinSelectedRoom()
{
	Gui::setScreen(std::make_unique<LocalRoomScreen>(this->rooms[this->selectedRoom]->getPlayerCount(), this->rooms[this->selectedRoom]));
	this->rooms[this->selectedRoom]->join();
}

void LocalRoomList::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (this->rooms.size() != 0)
	{
		int MaxRoom = static_cast<int>(this->rooms.size()-1);
		int MinRoom = 0;
		int MaxScroll = this->rooms.size()-NetworkListRoomsPerScreen;
		int MinScroll = 0;
		if (hDown & KEY_A)
		{
			this->joinSelectedRoom();
		}

		if (hDown & KEY_DOWN)
		{
			this->selectedRoom++;
			if (this->selectedRoom > MaxRoom)	this->selectedRoom = MaxRoom;

			if (this->roomsScroll+this->selectedRoom >= NetworkListRoomsPerScreen && this->selectedRoom == this->roomsScroll+NetworkListRoomsPerScreen)
			{
				this->roomsScroll++;
				if (this->roomsScroll > MaxScroll)	this->roomsScroll = MaxScroll;
			}
		}

		if (hDown & KEY_UP)
		{
			this->selectedRoom--;
			if (this->selectedRoom < MinRoom)	this->selectedRoom = MinRoom;

			if (this->roomsScroll > MinScroll && this->selectedRoom == this->roomsScroll-1)
			{
				this->roomsScroll--;
				if (this->roomsScroll < MinScroll)	this->roomsScroll = MinScroll;
			}
		}
	}

	if (hDown & KEY_SELECT) {
		this->refreshList();
	}

	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}
}