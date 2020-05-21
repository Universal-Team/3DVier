/*
*   This file is part of 3DVier
*   Copyright (C) 2020 DeadPhoenix8091, Epicpkmn11, Flame, RocketRobz, StackZ, TotallyNotGuy
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

#ifndef _3DVIER_LOCAL_NETWORK_HPP
#define _3DVIER_LOCAL_NETWORK_HPP

#include <3ds.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

// Sends and Receives if player's done & the position & Row.
struct PlayPackage {
	u8 Position; // The Position of the chip.
	u8 Row; // The Row where it should drop. (For the animation.)
	u8 LastPlayer;
	bool Done; // If you're done, set this to true on send.
};

class LocalNetwork {
public:
	// Constructor & Destructor.
	LocalNetwork();
	LocalNetwork(udsNetworkStruct Network);
	~LocalNetwork();
	// Join a network.
	void join();
	// Get Player Name & Count.
	std::string getName();
	std::vector<std::string> getPlayerNames();
	u8 getPlayerCount();
	// isReady functions.
	bool everyoneReady();
	bool isPlayerReady(int playerID);
	void receiveReady();
	void sendReady(int playerID, bool ready);
	// Packet Send & Receives.
	Result receivePacket(u8* package, size_t* actualSize, u16* sender);
	Result sendPacket(u8 package);
	Result receivePacketPlay(PlayPackage *package, size_t* actualSize, u16* sender);
	Result sendPacketPlay(PlayPackage package);
	// Send and receive, if round is done.
	void receiveDone();
	bool playerDone(int playerID);
	void sendDone(int playerID, bool ready);
	bool everyoneDone();
	// Send and receive game stuff.
	void receivePlay();
	void sendPlay(PlayPackage PKG, bool refresh = false);
	bool getPlay();
	PlayPackage getRound();
	// Returns the position of the current device.
	int getPosition() { return this->position; }
	// Return if current player is on the round.
	bool isPlayer(int i);
private:
	udsNetworkStruct network; // Our Network struct.
	udsBindContext bindcnt; // Our bind context.
	bool server; // If you're the host, this would be true.
	bool isConnected; // the connection state.
	std::array<bool, 2> isReady = {false}; // Set this to true, if player is ready for the game.
	std::array<bool, 2> roundDone = {false}; // Set to true, if round is done, so the game can work properly.
	PlayPackage pkg = {0, 0, 1, false}; // Play Package.
	int position = 0; // Device position.
};

std::vector<std::shared_ptr<LocalNetwork>> getRooms(); // Get a vector of available rooms for the RoomList.

#endif