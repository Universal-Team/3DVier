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
#include "keyboard.hpp"

/* 3DS Native keyboard. */
std::string Keyboard::setString(uint maxLength, std::string old, std::string Text) {
	/* Display Text on top screen. */
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	GFX::DrawTop(true);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.7f, Text))/2, 0.7f, TEXT_COLOR, Text, 385, 100);
	C3D_FrameEnd(0);

	SwkbdState state;
	swkbdInit(&state, SWKBD_TYPE_NORMAL, 2, maxLength);
	char temp[maxLength + 1] = {0};
	swkbdSetHintText(&state, Text.c_str());
	swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, SWKBD_FILTER_PROFANITY, 0);
	SwkbdButton ret = swkbdInputText(&state, temp, sizeof(temp));
	temp[maxLength] = '\0';

	return (ret == SWKBD_BUTTON_CONFIRM ? temp : old);
}

int Keyboard::setInt(int maxValue, std::string Text, int numAmount, int oldVal) {
	/* Display Text on top screen. */
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	GFX::DrawTop(true);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.7f, Text))/2, 0.7f, TEXT_COLOR, Text, 385, 100);
	C3D_FrameEnd(0);

	SwkbdState state;
	swkbdInit(&state, SWKBD_TYPE_NUMPAD, 2, numAmount);
	swkbdSetFeatures(&state, SWKBD_FIXED_WIDTH);
	swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, 0, 0);
	char input[numAmount + 1]   = {0};
	SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
	input[numAmount] = '\0';

	return (ret == SWKBD_BUTTON_CONFIRM ? (int)std::min(std::stoi(input), maxValue) : oldVal);
}