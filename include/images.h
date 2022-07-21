/* Copyright 2022 Vulcalien
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef MINICRAFT_IMAGES
#define MINICRAFT_IMAGES

#include "minicraft.h"

// TODO adjust the size once the palettes are complete
extern const u16 bg_palette[11 * 16];
extern const u16 sprite_palette[11 * 16];
extern const u16 item_palette[4 * 16];

// TODO adjust this to be the right size
// once the tileset is complete
extern const u8 level_tileset[32 * 128];
extern const u8 gui_tileset[32 * 128];
extern const u8 light_tileset[32 * 5];

extern const u8 sprite_tileset[32 * 96];
extern const u8 font_sprite_tileset[32 * 10];

extern const u8 item_tileset[32 * 53];

#endif // MINICRAFT_IMAGES
