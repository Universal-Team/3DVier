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

#ifndef _3DVIER_CONFIG_HPP
#define _3DVIER_CONFIG_HPP

#include "json.hpp"

#include <3ds.h>
#include <string>

class Config {
public:
	Config();
	void save();
	void initialize();

	// Player 1 Chip Color.
	u32 p1Chip() { return this->v_p1Chip; }
	void p1Chip(u32 v) { this->v_p1Chip = v; if (!this->changesMade)	this->changesMade = true; }
	// Player 2 Chip Color.
	u32 p2Chip() { return this->v_p2Chip; }
	void p2Chip(u32 v) { this->v_p2Chip = v; if (!this->changesMade)	this->changesMade = true; }
	// Raster color.
	u32 raster() { return this->v_raster; }
	void raster(u32 v) { this->v_raster = v; if (!this->changesMade)	this->changesMade = true; }
	// Darken Screen in game.
	bool darkenScreen() { return this->v_darkenScreen; }
	void darkenScreen(bool v) { this->v_darkenScreen = v; if (!this->changesMade)	this->changesMade = true; }
	// Allow drops.
	bool allowDrop() { return this->v_allowDrops; }
	void allowDrop(bool v) { this->v_allowDrops = v; if (!this->changesMade)	this->changesMade = true; }
	// Bar Color.
	u32 barColor() { return this->v_barColor; }
	void barColor(u32 v) { this->v_barColor = v; if (!this->changesMade)	this->changesMade = true; }
	// BG Color.
	u32 bgColor() { return this->v_bgColor; }
	void bgColor(u32 v) { this->v_bgColor = v; if (!this->changesMade)	this->changesMade = true; }
	// Text Color.
	u32 textColor() { return this->v_textColor; }
	void textColor(u32 v) { this->v_textColor = v; if (!this->changesMade)	this->changesMade = true; }
	// Button Color.
	u32 buttonColor() { return this->v_buttonColor; }
	void buttonColor(u32 v) { this->v_buttonColor = v; if (!this->changesMade)	this->changesMade = true; }
	// Selector Color.
	u32 selectorColor() { return this->v_selectorColor; }
	void selectorColor(u32 v) { this->v_selectorColor = v; if (!this->changesMade)	this->changesMade = true; }
	// Language.
	int language() { return this->v_language; }
	void language(int v) { this->v_language = v; if (!this->changesMade)	this->changesMade = true; }

	// Mainly helper.
	bool getBool(const std::string &key);
	void setBool(const std::string &key, bool v);
	int getInt(const std::string &key);
	void setInt(const std::string &key, int v);
	std::string getString(const std::string &key);
	void setString(const std::string &key, const std::string &v);
private:
	nlohmann::json json; // Our private JSON file.
	bool changesMade = false;

	// Color variables and more.
	u32 v_p1Chip;
	u32 v_p2Chip;
	u32 v_raster;
	bool v_darkenScreen;
	bool v_allowDrops;
	u32 v_barColor;
	u32 v_bgColor;
	u32 v_textColor;
	u32 v_buttonColor;
	u32 v_selectorColor;
	int v_language;
};

#endif