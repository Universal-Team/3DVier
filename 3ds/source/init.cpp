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

#include "common.hpp"
#include "init.hpp"
#include "mainMenu.hpp"

#include <3ds.h>
#include <dirent.h>
#include <unistd.h>

bool exiting = false;
touchPosition touch;
u32 hDown, hHeld;

/* Include all spritesheet's. */
C2D_SpriteSheet characters, sprites;

/* If button Position pressed -> Do something. */
bool touching(touchPosition touch, Structs::ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h)) return true;
	else return false;
}

Result Init::Initialize() {
	gfxInitDefault();
	romfsInit();
	Gui::init();
	mkdir("sdmc:/3ds", 0777); // For DSP dump
	mkdir("sdmc:/3ds/3DVier", 0777); // main Path.

	Gui::loadSheet("romfs:/gfx/chars.t3x", characters);
	Gui::loadSheet("romfs:/gfx/sprites.t3x", sprites);

	osSetSpeedupEnable(true); // Enable speed-up for New 3DS users.
	Settings::Read();
	Lang::load(1);
	Overlays::SplashOverlay();
	Gui::setScreen(std::make_unique<MainMenu>(), false, true);
	return 0;
}

Result Init::MainLoop() {
	/* Initialize everything. */
	Initialize();

	fadealpha = 255;
	fadein = true;
	hidSetRepeatParameters(10, 10);

	/* Loop as long as the status is not exiting. */
	while(aptMainLoop() && !exiting) {
		/* Scan all the Inputs. */
		hidScanInput();
		hDown = hidKeysDown();
		hHeld = hidKeysHeld();
		hidTouchRead(&touch);
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
		C2D_TargetClear(Bottom, C2D_Color32(0, 0, 0, 0));
		Gui::clearTextBufs();
		Gui::DrawScreen(true);
		Gui::ScreenLogic(hDown, hHeld, touch, true, true);
		C3D_FrameEnd(0);

		if (exiting) {
			if (!fadeout) break;
		}

		Gui::fadeEffects(16, 16, true);
	}
	
	/* Exit all services and exit the app. */
	Exit();
	return 0;
}

Result Init::Exit() {
	Gui::exit();
	Gui::unloadSheet(characters);
	Gui::unloadSheet(sprites);
	Settings::Save();
	cfguExit();
	gfxExit();
	romfsExit();
	return 0;
}