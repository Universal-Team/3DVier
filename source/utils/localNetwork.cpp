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

#include "msg.hpp"
#include "localNetwork.hpp"

static u32 wlancommID = 0x7B85A311; // Our Unique ID.
#define appdataLen 0xc8
#define appdataSigLen 4
#define appdataStrLen (appdataLen-appdataSigLen)
static u8 appdata[appdataLen] = {0x69, 0x8a, 0x05, 0x5c};
#define appdataStr(ad) ((char*)ad+appdataSigLen)
#define usernameLength 0x1C
static u8 data_channel = 1;
static u32 recv_buffer_size = UDS_DEFAULT_RECVBUFSIZE;

LocalNetwork::LocalNetwork() {
	udsGenerateDefaultNetworkStruct(&this->network, wlancommID, 0, 2);

	u8 tmpUsername[usernameLength] = {0};
	// Get System Config information.
	Result ret = CFGU_GetConfigInfoBlk2(usernameLength, 0x000A0000, tmpUsername);
	if (ret != 0) {
	//	Msg::DisplayWaitMsg("CFGU_GetConfigInfoBlk2() failed!");
		return;
	}
	
	// Create the Network.
	ret = udsCreateNetwork(&this->network, "3DVier-Local", 12, &this->bindcnt, data_channel, recv_buffer_size);
	if (ret != 0) {
	//	Msg::DisplayWaitMsg("udsCreateLocalNetwork() failed!");
		return;
	}

	// Get user name.
	utf16_to_utf8((u8*)appdataStr(appdata), (u16*)tmpUsername, usernameLength/sizeof(u16));
	// Set App-data.
	ret = udsSetApplicationData(appdata, appdataLen*sizeof(u8));
	if (ret != 0) {
	//	Msg::DisplayWaitMsg("udsSetApplicationData() failed!");
		return;
	}
	// dont allow spectators.
	ret = udsEjectSpectator();
	if (ret != 0) {
	//	Msg::DisplayWaitMsg("udsEjectSpectator() failed!");
		return;
	}
	// Set all player's to not-ready.
	for(size_t i = 0; i < this->isReady.size(); i++)	this->isReady[i] = false;
	// Set roundDone to false.
	for(size_t i = 0; i < this->roundDone.size(); i++)	this->roundDone[i] = false;
	this->server = true; // We created the server, so we're the server "owner".
	this->position = 1; // First Position cause owner!
	this->isConnected = true; // We are connected!
}

LocalNetwork::LocalNetwork(udsNetworkStruct Network)
{
	this->network = Network;
	for(size_t i = 0; i < this->isReady.size(); i++)	this->isReady[i] = false;
	for(size_t i = 0; i < this->roundDone.size(); i++)	this->roundDone[i] = false;
	this->server = false;
	this->isConnected = false;
}

LocalNetwork::~LocalNetwork()
{
	if (this->server)	udsDestroyNetwork(); // If we're the server owner, we destroy our Network.
	else				udsDisconnectNetwork(); // Else we disconnect from it.
	udsUnbind(&this->bindcnt); // Of course unbind our context!
}

bool LocalNetwork::isPlayer(int i) {
	// If it's our turn, we return true, else false.
	if (this->position == i)	return true;
	return false;
}

void LocalNetwork::join() {
	for(int i = 0; i < 10; i++) { // Allow 10 tries as maximum for connecting.
		// We try to connect to our Network here.
		Result ret = udsConnectNetwork(&this->network, "3DVier-Local", 12, &this->bindcnt, UDS_BROADCAST_NETWORKNODEID, UDSCONTYPE_Client, data_channel, recv_buffer_size);
		if (R_FAILED(ret)) {
		//	Msg::DisplayWaitMsg("udsConnectNetwork() failed!");
			break;
		} else {
			this->position = 2; // Because we join, our position is 2.
			this->isConnected = true; // Yep, we're connected. :)
		//	Msg::DisplayWaitMsg("Successfully joined.");
			break;
		}
	}
}

// Get the name of the current device.
std::string LocalNetwork::getName() {
	return std::string(appdataStr(this->network.appdata), this->network.appdata_size-appdataSigLen);
}

// Return a vector of strings for the user-names.
std::vector<std::string> LocalNetwork::getPlayerNames() {
	std::vector<std::string> names;
	for(u8 i = 0; i < this->getPlayerCount(); i++) {
		udsNodeInfo node;
		Result ret = udsGetNodeInformation(i+1, &node);
		if (ret != 0) {
		//	Msg::DisplayWaitMsg("udsGetNodeInformation() failed!");
			return {"?"};
		}

		char username[256] = {0};
		ret = udsGetNodeInfoUsername(&node, username);
		if (ret != 0) {
		//	Msg::DisplayWaitMsg("udsGetNodeInfoUsername() failed!");
			return {"?"};
		}

		names.push_back(std::string(username));
	}
	return names;
}

// Get the amount of players, which are connected to the Network.
u8 LocalNetwork::getPlayerCount() {
	if (this->isConnected) {
		udsConnectionStatus constatus;
		udsGetConnectionStatus(&constatus);
		return constatus.total_nodes;
	} else {
		return this->network.total_nodes;
	}
}

// Send and receive u8* packages. (Maybe useful for other things in the future?)
Result LocalNetwork::receivePacket(u8 *package, size_t* actualSize, u16* sender) {
	return udsPullPacket(&this->bindcnt, package, sizeof(package), actualSize, sender);
}
Result LocalNetwork::sendPacket(u8 package) {
	return udsSendTo(UDS_BROADCAST_NETWORKNODEID, data_channel, UDS_SENDFLAG_Default, &package, sizeof(package));
}

// Send and receive the PlayPackage's.
Result LocalNetwork::receivePacketPlay(PlayPackage *package, size_t* actualSize, u16* sender) {
	return udsPullPacket(&this->bindcnt, package, sizeof(PlayPackage), actualSize, sender);
}
Result LocalNetwork::sendPacketPlay(PlayPackage package) {
	return udsSendTo(UDS_BROADCAST_NETWORKNODEID, data_channel, UDS_SENDFLAG_Default, &package, sizeof(PlayPackage));
}

// Return if all Player are ready!
bool LocalNetwork::everyoneReady() {
	// Get the amount of players here.
	size_t playerCount = this->getPlayerCount();
	if (playerCount == 0)	return false; // The amount of player's is 0, so not valid.
	for(size_t i = 0; i < playerCount; i++) {
		if (!this->isReady[i])	return false; // Nope, not all player are ready.
	}
	return true;
}

// Return, if a specific player is ready.
bool LocalNetwork::isPlayerReady(int playerID) {
	return this->isReady[playerID];
}

// Receive the Ready's of all other Players.
void LocalNetwork::receiveReady() {
	u8 ready;
	size_t actualSize = 0;
	u16 sender;

	Result ret = this->receivePacket(&ready, &actualSize, &sender);
	if (R_FAILED(ret)) {
	//	Msg::DisplayWaitMsg("this->receivePacket() failed!");
		return;
	}

	// Make sure if the actual received size matches the size of a boolean.
	if (actualSize == sizeof(bool)) {
		this->isReady[sender-1] = ready;
	}
}

// Send if we're ready.
void LocalNetwork::sendReady(int playerID, bool ready) {
	this->isReady[playerID] = ready;

	Result ret = this->sendPacket((u8)ready);
	if (R_FAILED(ret)) {
	//	Msg::DisplayWaitMsg("this->sendPacket() failed!");
		return;
	}
}

// Scan for LocalNetworks, so we can join on the RoomList screen.
std::vector<std::shared_ptr<LocalNetwork>> getRooms() {
	u32 tmpbuf_size = 0x4000;
	u32* tmpbuf = (u32*)malloc(tmpbuf_size);
	memset(tmpbuf, 0, tmpbuf_size);

	size_t total_LocalNetworks = 0;
	udsNetworkScanInfo *LocalNetworks = NULL;

	// Scan for available LocalNetworks.
	Result ret = udsScanBeacons(tmpbuf, tmpbuf_size, &LocalNetworks, &total_LocalNetworks, wlancommID, 0, NULL, false);
	if (R_FAILED(ret)) {
	//	Msg::DisplayWaitMsg("udsScanBeacons() failed!");
		return {nullptr};
	}
	// Free now unused buffer.
	free(tmpbuf);

	std::vector<std::shared_ptr<LocalNetwork>> rooms;
	// Get the amount of LocalNetworks & Init the LocalNetwork class through it.
	for(size_t i = 0; i < total_LocalNetworks; i++) {
		udsNetworkStruct network = LocalNetworks[i].network;
		if (!memcmp(network.appdata, appdata, appdataSigLen) && network.max_nodes == 2 && network.total_nodes < 2) {
			// Push back a LocalNetwork class.
			std::shared_ptr<LocalNetwork> room(new LocalNetwork(network));
			rooms.push_back(room);
		}
	}
	// Free now unused LocalNetwork and return the rooms.
	free(LocalNetworks);
	return rooms;
}

// Get if everyone's done.
bool LocalNetwork::everyoneDone() {
	// Get amount of players.
	int playerCount = this->getPlayerCount();
	if (playerCount == 0)	return false; // 0 -> Invalid.
	for(int i = 0; i < playerCount; i++) {
		if (!this->roundDone[i])	return false; // Not all are done.
	}
	return true;
}

// Return if specific player is done.
bool LocalNetwork::playerDone(int playerID) {
	return this->roundDone[playerID];
}

// Receive if players are done.
void LocalNetwork::receiveDone() {
	u8 ready;
	size_t actualSize = 0;
	u16 sender;

	// Receive done package.
	Result ret = this->receivePacket(&ready, &actualSize, &sender);
	if (R_FAILED(ret)) {
	//	Msg::DisplayWaitMsg("this->receiveDone() failed!");
		return;
	}
	// Make sure the received size matches a boolean.
	if (actualSize == sizeof(bool)) {
		this->roundDone[sender-1] = ready;
	}
}

// Send done package.
void LocalNetwork::sendDone(int playerID, bool ready) {
	// Set that we're done.
	this->roundDone[playerID] = ready;
	// Send our done package.
	Result ret = this->sendPacket((u8)ready);
	if (R_FAILED(ret)) {
	//	Msg::DisplayWaitMsg("this->sendPacket() failed!");
		return;
	}
}

// Receive the Play package.
void LocalNetwork::receivePlay() {
	PlayPackage PKG;
	size_t actualSize = 0;
	u16 sender;
	// Receive the Play Package.
	Result ret = this->receivePacketPlay(&PKG, &actualSize, &sender);
	if (R_FAILED(ret)) {
	//	Msg::DisplayWaitMsg("this->receivePacketPlay() failed!");
		return;
	}
	// Make sure the received data matches the PlayPackage size.
	if (actualSize == sizeof(PlayPackage)) {
		this->pkg = PKG;
	}
}

// Send our play package.
void LocalNetwork::sendPlay(PlayPackage PKG, bool refresh) {
	if (!refresh) {
		this->pkg = PKG;
	}
	// Send Play Package.
	Result ret = this->sendPacketPlay(PKG);
	if (R_FAILED(ret)) {
	//	Msg::DisplayWaitMsg("this->sendPacketPlay() failed!");
		return;
	}
}

// Get the PlayPackage for this round.
PlayPackage LocalNetwork::getRound() {
	return this->pkg;
}