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

#include "_3DVier_Helper.hpp"
#include "common.hpp"

#include <3ds.h>

extern C2D_SpriteSheet sprites;

/* Draw Gamefield. */
void _3DVier_Helper::DrawField(int x, int y) {
	GFX::DrawSprite(sprites_field_idx, x, y);
}

void _3DVier_Helper::DrawChip(int x, int y, int player) {
	GFX::DrawSprite(player-1 ? sprites_chip_p1_idx : sprites_chip_p2_idx, x, y);
}

/* Dran an animated selected chip. */
void _3DVier_Helper::SelectedChip(int x, int y, int player, float speed) {
	static float timer			= 0.0f;
	float highlight_multiplier	= fmax(0.0, fabs(fmod(timer, 1.0) - 0.5) / 0.5);
	u8 r						= (player-1 ? PLAYER_1_CHIP : PLAYER_2_CHIP) & 0xFF;
	u8 g						= ((player-1 ? PLAYER_1_CHIP : PLAYER_2_CHIP) >> 8) & 0xFF;
	u8 b						= ((player-1 ? PLAYER_1_CHIP : PLAYER_2_CHIP) >> 16) & 0xFF;
	u32 color = C2D_Color32(r + (255 - r) * highlight_multiplier, g + (255 - g) * highlight_multiplier, b + (255 - b) * highlight_multiplier, 255);

	C2D_ImageTint tint;
	C2D_SetImageTint(&tint, C2D_TopLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_TopRight, color, 1);
	C2D_SetImageTint(&tint, C2D_BotLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_BotRight, color, 1);

	C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, player-1 ? sprites_selector_p1_idx : sprites_selector_p2_idx), x, y, 0.5f, &tint);
	timer += speed;
}