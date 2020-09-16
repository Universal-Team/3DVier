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
static bool configLoaded = false;

/* Change these definitions, when needed for defaults. */
#ifdef _3DS
	#define SETTINGS_FILE "sdmc:/3ds/3DVier/Settings.json"
	#define SPEED_PLUS 4
	#define DROP_SPEED 3
	#define CLEAR_SPEED 2

#elif _NDS
	#define SETTINGS_FILE sdFound() ? "sd:/_nds/DSVier/Settings.json" : "fat:/_nds/DSVier/Settings.json"
	#define SPEED_PLUS 1
	#define DROP_SPEED 3
	#define CLEAR_SPEED 2

#else
	#define SETTINGS_FILE "/3DVier/Settings.json"
	#define SPEED_PLUS 2
	#define DROP_SPEED 3
	#define CLEAR_SPEED 2
#endif

/* Language. */
int Settings::language() {
	if (!configLoaded) return 1;

	if (config.contains("Language")) return config["Language"];
	else return 1; // English.
}
void Settings::language(int lang) {
	if (!configLoaded) return;

	config["Language"] = lang;
	if (!configChanged) configChanged = true;
}

/* Dropspeed. */
int Settings::dropSpeed() {
	if (!configLoaded) return DROP_SPEED;

	if (config.contains("DropSpeed")) return config["DropSpeed"];
	else return DROP_SPEED;
}
void Settings::dropSpeed(int speed) {
	if (!configLoaded) return;

	config["DropSpeed"] = speed;
	if (!configChanged) configChanged = true;
}

/* Clear speed. */
int Settings::clearSpeed() {
	if (!configLoaded) return CLEAR_SPEED;
	
	if (config.contains("ClearSpeed")) return config["ClearSpeed"];
	else return CLEAR_SPEED;
}
void Settings::clearSpeed(int speed) {
	if (!configLoaded) return;

	config["ClearSpeed"] = speed;
	if (!configChanged) configChanged = true;
}

/* Speed Plus. */
int Settings::speedPlus() {
	if (!configLoaded) return SPEED_PLUS;

	if (config.contains("SpeedPlus")) return config["SpeedPlus"];
	else return SPEED_PLUS;
}
void Settings::speedPlus(int speed) {
	if (!configLoaded) return;

	config["SpeedPlus"] = speed;
	if (!configChanged) configChanged = true;
}

/* Read Config. */
void Settings::Read() {
	if (access(SETTINGS_FILE, F_OK) != 0) {
		FILE *file = fopen(SETTINGS_FILE, "w");

		config["Language"] = 1;
		config["DropSpeed"] = DROP_SPEED;
		config["ClearSpeed"] = CLEAR_SPEED;
		config["SpeedPlus"] = SPEED_PLUS;

		const std::string dump = config.dump(1, '\t');
		fwrite(dump.c_str(), 1, config.dump(1, '\t').size(), file);
		fclose(file); // Now we have the file and can properly access it.
	}

	FILE* file = fopen(SETTINGS_FILE, "r");
	config = nlohmann::json::parse(file, nullptr, false);
	configLoaded = true;
	fclose(file);
}

/* Save Config. */
void Settings::Save() {
	if (!configLoaded) return;

	if (configChanged) {
		FILE *file = fopen(SETTINGS_FILE, "wb");
		const std::string dump = config.dump(1, '\t');
		fwrite(dump.c_str(), 1, config.dump(1, '\t').size(), file);
		fclose(file);
	}
}