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

#ifndef _3DVIER_LOCAL_ROOM_LIST_SCREEN_HPP
#define _3DVIER_LOCAL_ROOM_LIST_SCREEN_HPP

#include "common.hpp"
#include "localNetwork.hpp"
#include "structs.hpp"
#include <vector>

constexpr int NetworkListRoomsPerScreen = 3;

class LocalRoomList : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	LocalRoomList();
	~LocalRoomList();
private:
	void joinRoom(int i);
	void refreshList();
	void joinSelectedRoom();
	std::vector<std::shared_ptr<LocalNetwork>> rooms;
	int selectedRoom;
	int roomsScroll;
};

#endif