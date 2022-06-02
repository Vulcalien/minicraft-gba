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
#include "tile.h"

#include "level.h"

#define SPR(id, palette) ((id) | ((palette) << 12))

// Grass Tile
static void grass_tick(struct Level *level, u32 xt, u32 yt) {
}

static void grass_draw(struct Level *level, u32 xt, u32 yt, u16 tiles[4]) {
    bool u = LEVEL_GET_TILE_S(level, xt,     yt - 1)->connects_to.grass;
    bool d = LEVEL_GET_TILE_S(level, xt,     yt + 1)->connects_to.grass;
    bool l = LEVEL_GET_TILE_S(level, xt - 1, yt    )->connects_to.grass;
    bool r = LEVEL_GET_TILE_S(level, xt + 1, yt    )->connects_to.grass;

    if(u && l)
        tiles[0] = SPR(0, 0);
    else
        tiles[0] = SPR(4 + u * 4 + l * 7, 0);

    if(u && r)
        tiles[1] = SPR(1, 0);
    else
        tiles[1] = SPR(5 + u * 3 + r * 4, 0);

    if(d && l)
        tiles[2] = SPR(2, 0);
    else
        tiles[2] = SPR(6 + d * 4 + l * 5, 0);

    if(d && r)
        tiles[3] = SPR(3, 0);
    else
        tiles[3] = SPR(7 + d * 3 + r * 2, 0);
}

// Tile List
const struct Tile tile_list[TILES_COUNT] = {
    // Grass
    {
        .tick = grass_tick,
        .draw = grass_draw,

        .connects_to = {
            .grass = true
        }
    }
};
