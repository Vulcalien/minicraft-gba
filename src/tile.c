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

static void grass_tick(struct Level *level, u32 xt, u32 yt) {
}

static void grass_draw(struct Level *level, u32 xt, u32 yt,
                       vu16 *t0, vu16 *t1, vu16 *t2, vu16 *t3) {
    // DEBUG
    *t0 = 0 | (0 << 12);
    *t1 = 1 | (0 << 12);
    *t2 = 2 | (0 << 12);
    *t3 = 3 | (0 << 12);
}

// FIXME this thing does not set the fields. I don't understand why...
const struct Tile tiles[TILES_COUNT] = {
    // Grass
    {
        .tick = grass_tick,
        .draw = grass_draw
    }
};
