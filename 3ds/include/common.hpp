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

#ifndef _3DVIER_COMMON_HPP
#define _3DVIER_COMMON_HPP

#include "gfx.hpp"
#include "gui.hpp"
#include "keyboard.hpp"
#include "lang.hpp"
#include "msg.hpp"
#include "overlay.hpp"
#include "screenCommon.hpp"
#include "settings.hpp"

#include <citro2d.h>

#define TRANSPARENT C2D_Color32(0, 0, 0, 0)
#define BAR_COLOR C2D_Color32(60, 60, 220, 255)
#define BG_COLOR C2D_Color32(160, 160, 220, 255)
#define TEXT_COLOR C2D_Color32(255, 255, 255, 255)
#define BUTTON_COLOR C2D_Color32(60, 0, 170, 255)
#define SELECTED_BUTTON_COLOR C2D_Color32(60, 0, 230, 255)
#define FIELD_COLOR C2D_Color32(0, 0, 243, 255)
#define PLAYER_1_CHIP C2D_Color32(239, 239, 0, 255)
#define PLAYER_2_CHIP C2D_Color32(239, 0, 0, 255)

#endif