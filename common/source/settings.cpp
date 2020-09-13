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

#ifdef _NDS
	#include "flashcard.hpp"
#endif

#include "json.hpp"
#include "settings.hpp"
#include <unistd.h>

static nlohmann::json config;
static bool configChanged = false;

#ifdef _3DS
	#define SETTINGS_FILE "sdmc:/3ds/3DVier/Settings.json"

#elif _NDS
	#define SETTINGS_FILE sdFound() ? "sd:/_nds/DSVier/Settings.json" : "fat:/_nds/DSVier/Settings.json"
	
#else
	#define SETTINGS_FILE "/3DVier/Settings.json"
#endif

/* Language. */
int Settings::language() {
	if (config.contains("Language")) return config["Language"];
	else return 1; // English.
}
void Settings::language(int lang) {
	config["Language"] = lang;
	if (!configChanged) configChanged = true;
}

/* Dropspeed. */
int Settings::dropSpeed() {
	if (config.contains("DropSpeed")) return config["DropSpeed"];
	else return 3;
}
void Settings::dropSpeed(int speed) {
	config["DropSpeed"] = speed;
	if (!configChanged) configChanged = true;
}

/* Clear speed. */
int Settings::clearSpeed() {
	if (config.contains("ClearSpeed")) return config["ClearSpeed"];
	else return 2;
}
void Settings::clearSpeed(int speed) {
	config["ClearSpeed"] = speed;
	if (!configChanged) configChanged = true;
}

/* Speed Multiplier. */
float Settings::speedMultiplier() {
	if (config.contains("SpeedMultiplier")) return config["SpeedMultiplier"];
	else return 1.4;
}
void Settings::speedMultiplier(float multi) {
	config["SpeedMultiplier"] = multi;
	if (!configChanged) configChanged = true;
}

/* Read Config. */
void Settings::Read() {
	if (access(SETTINGS_FILE, F_OK) != 0) {
		FILE *file = fopen(SETTINGS_FILE, "w");

		config["Language"] = 1;
		config["DropSpeed"] = 3;
		config["ClearSpeed"] = 2;
		config["SpeedMultiplier"] = 1.4;

		const std::string dump = config.dump(1, '\t');
		fwrite(dump.c_str(), 1, config.dump(1, '\t').size(), file);
		fclose(file); // Now we have the file and can properly access it.
	}

	FILE* file = fopen(SETTINGS_FILE, "r");
	config = nlohmann::json::parse(file, nullptr, false);
	fclose(file);
}

/* Save Config. */
void Settings::Save() {
	if (configChanged) {
		FILE *file = fopen(SETTINGS_FILE, "wb");
		const std::string dump = config.dump(1, '\t');
		fwrite(dump.c_str(), 1, config.dump(1, '\t').size(), file);
		fclose(file);
	}
}