/*
*   This file is part of DSVier
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

#include "colors.hpp"
#include "gui.hpp"
#include "lang.hpp"

#include <stack>
#include <vector>

std::stack<std::unique_ptr<Screen>> screens;
std::array<int, 4> selectorspr;
Image BG, Field, Chip[2], ChipSelector[2];

void Gui::DrawScreen() {
	clearScreen(false, true);
	clearScreen(true, true);
	screens.top()->Draw();
}

void Gui::mainLoop(u16 hDown, touchPosition touch) { screens.top()->Logic(hDown, touch); }

void Gui::setScreen(std::unique_ptr<Screen> screen) { screens.push(std::move(screen)); }

void Gui::screenBack() { screens.pop(); }

/* Basic GUI Stuff. */
void Gui::DrawTop(bool useBars) {
	if (useBars) {
		drawRectangle(0, 0, 256, 20, DARKERER_GRAY, true, false);
		drawRectangle(0, 20, 256, 152, DARKER_GRAY, true, false);
		drawRectangle(0, 172, 256, 20, DARKERER_GRAY, true, false);

	} else {
		drawRectangle(0, 0, 256, 192, DARKER_GRAY, true, false);
	}
}

void Gui::DrawBottom(bool useBars) {
	if (useBars) {
		drawRectangle(0, 0, 256, 20, DARKERER_GRAY, false, false);
		drawRectangle(0, 20, 256, 152, DARKER_GRAY, false, false);
		drawRectangle(0, 172, 256, 20, DARKERER_GRAY, false, false);

	} else {
		drawRectangle(0, 0, 256, 192, DARKER_GRAY, false, false);
	}
}

void Gui::clearScreen(bool top, bool layer) {
	drawRectangle(0, 0, 256, 192, CLEAR, top, layer);
}

void Gui::loadGraphics() {
	Image selector = loadImage("/graphics/corner.gfx");

	/* Load game assets. */
	BG = loadImage("/graphics/ui/bg.gfx");
	Field = loadImage("/graphics/ui/field.gfx");
	Chip[0] = loadImage("/graphics/ui/chip_p1.gfx");
	Chip[1] = loadImage("/graphics/ui/chip_p2.gfx");
	ChipSelector[0] = loadImage("/graphics/ui/selector_p1.gfx");
	ChipSelector[1] = loadImage("/graphics/ui/selector_p2.gfx");

	/* Init selector. */
	for (int i = 0; i < (int)selectorspr.size();i++) {
		selectorspr[i] = initSprite(false, SpriteSize_16x16);
		prepareSprite(selectorspr[i], false, 0, 0, 0, i % 2, i / 2);
		fillSpriteImage(selectorspr[i], false, 16, 0, 0, selector);
	}
}

void Gui::DrawButton(ButtonStruct btn, TextColor color) {
	drawRectangle(btn.x, btn.y, btn.xSize, btn.ySize, btn.colorIndex, false, btn.layer);
	printTextCenteredTintedMaxW(Lang::get(btn.Text), btn.xSize-5, 1, color, btn.x - 128 + (btn.xSize/2), btn.y + (btn.ySize/2) - 10, false, true);
}