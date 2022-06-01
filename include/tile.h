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
#ifndef MINICRAFT_TILE
#define MINICRAFT_TILE

#include "minicraft.h"

#include "level.h"

struct Tile {
    void (*tick)(struct Level *level, u32 xt, u32 yt);
    void (*draw)(struct Level *level, u32 xt, u32 yt, u16 tiles[4]);
};

#define TILES_COUNT (23)
extern const struct Tile tile_list[TILES_COUNT];

#endif // MINICRAFT_TILE
