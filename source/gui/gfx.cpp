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

#include "common.hpp"
#include "config.hpp"

extern std::unique_ptr<Config> config;

void GFX::DrawTop(bool useBars) {
	Gui::ScreenDraw(Top);
	if (useBars) {
		Gui::Draw_Rect(0, 0, 400, 30, config->barColor());
		Gui::Draw_Rect(0, 30, 400, 180, config->bgColor());
		Gui::Draw_Rect(0, 210, 400, 30, config->barColor());
	} else {
		Gui::Draw_Rect(0, 0, 400, 240, config->bgColor());
	}
}

// False actually only draws 1 bar on the top screen. Special case for the Game screen. xD
void GFX::DrawBottom(bool useBars) {
	Gui::ScreenDraw(Bottom);
	if (useBars) {
		Gui::Draw_Rect(0, 0, 320, 30, config->barColor());
		Gui::Draw_Rect(0, 30, 320, 180, config->bgColor());
		Gui::Draw_Rect(0, 210, 320, 30, config->barColor());
	} else {
		Gui::Draw_Rect(0, 0, 320, 240, config->bgColor());
	}
}

extern C2D_SpriteSheet characters;
extern C2D_SpriteSheet sprites;

void GFX::DrawSprite(int index, int x, int y, float ScaleX, float ScaleY) {
	Gui::DrawSprite(sprites, index, x, y, ScaleX, ScaleY);
}

// Actual game stuff.
void GFX::DrawRaster(int x, int y) {
	u32 color = config->raster();
	C2D_ImageTint tint;
	C2D_SetImageTint(&tint, C2D_TopLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_TopRight, color, 1);
	C2D_SetImageTint(&tint, C2D_BotLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_BotRight, color, 1);

	C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, sprites_raster_idx), x, y, 0.5, &tint, 1, 1);
}

void GFX::DrawChip(int x, int y, float ScaleX, float ScaleY, int player) {
	u32 color;
	// Recolor chip.
	if (player == 1) {
		color = config->p1Chip();
	} else {
		color = config->p2Chip();
	}

	C2D_ImageTint tint;
	C2D_SetImageTint(&tint, C2D_TopLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_TopRight, color, 1);
	C2D_SetImageTint(&tint, C2D_BotLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_BotRight, color, 1);
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, sprites_circle_idx), x, y, 0.5, &tint, ScaleX, ScaleY);
}

// Selectors.
void GFX::DrawSelectedChip(int x, int y, float ScaleX, float ScaleY) {
	static float timer			= 0.0f;
	float highlight_multiplier	= fmax(0.0, fabs(fmod(timer, 1.0) - 0.5) / 0.5);
	u8 r						= config->selectorColor() & 0xFF;
	u8 g						= (config->selectorColor() >> 8) & 0xFF;
	u8 b						= (config->selectorColor() >> 16) & 0xFF;
	u32 color = C2D_Color32(r + (255 - r) * highlight_multiplier, g + (255 - g) * highlight_multiplier, b + (255 - b) * highlight_multiplier, 255);
	C2D_ImageTint tint;
	C2D_SetImageTint(&tint, C2D_TopLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_TopRight, color, 1);
	C2D_SetImageTint(&tint, C2D_BotLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_BotRight, color, 1);
	C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, sprites_circleSelector_idx), x, y, 0.5, &tint, ScaleX, ScaleY);
	timer += .030;
}

void GFX::DrawButtonSelector(int x, int y, float ScaleX, float ScaleY, bool useSmall)
{
	static float timer			= 0.0f;
	float highlight_multiplier	= fmax(0.0, fabs(fmod(timer, 1.0) - 0.5) / 0.5);
	u8 r						= config->selectorColor() & 0xFF;
	u8 g						= (config->selectorColor() >> 8) & 0xFF;
	u8 b						= (config->selectorColor() >> 16) & 0xFF;
	u32 color = C2D_Color32(r + (255 - r) * highlight_multiplier, g + (255 - g) * highlight_multiplier, b + (255 - b) * highlight_multiplier, 255);
	C2D_ImageTint tint;
	C2D_SetImageTint(&tint, C2D_TopLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_TopRight, color, 1);
	C2D_SetImageTint(&tint, C2D_BotLeft, color, 1);
	C2D_SetImageTint(&tint, C2D_BotRight, color, 1);
	if (useSmall) {
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, sprites_btnSelector2_idx), x, y, 0.5f, &tint, ScaleX, ScaleY);
	} else {
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, sprites_btnSelector_idx), x, y, 0.5f, &tint, ScaleX, ScaleY);
	}
	timer += .030;
}

// Player Character.
void GFX::DrawChar(int image, int x, int y, float ScaleX, float ScaleY) {
	Gui::DrawSprite(characters, image, x, y, ScaleX, ScaleY);
}

void GFX::DrawPlayer(int x, int y, float ScaleX, float ScaleY, int player) {
	switch (player) {
		case 0:
			DrawChar(chars_char1_idx, x, y, ScaleX, ScaleY);
			break;
		case 1:
			DrawChar(chars_char2_idx, x, y, ScaleX, ScaleY);
			break;
		case 2:
			DrawChar(chars_char3_idx, x, y, ScaleX, ScaleY);
			break;
		case 3:
			DrawChar(chars_char4_idx, x, y, ScaleX, ScaleY);
			break;
		case 4:
			DrawChar(chars_char5_idx, x, y, ScaleX, ScaleY);
			break;
		case 5:
			DrawChar(chars_char6_idx, x, y, ScaleX, ScaleY);
			break;
		case 6:
			DrawChar(chars_char7_idx, x, y, ScaleX, ScaleY);
			break;
		case 7:
			DrawChar(chars_char8_idx, x, y, ScaleX, ScaleY);
			break;
	}
}