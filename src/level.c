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
#include "level.h"

#include "screen.h"
#include "tile.h"

void level_tick(struct Level *level) {
}

// TODO this function has to be very efficient,
// since VBlank does not last long
void level_draw(struct Level *level) {
    // TODO set offset
    u32 x_offset = 0;
    u32 y_offset = 0;

    BG0_XOFFSET = x_offset & 0x0f;
    BG0_YOFFSET = y_offset & 0x0f;

    u32 x0 = x_offset >> 4;
    u32 y0 = y_offset >> 4;

    for(u32 y = 0; y <= 10; y++) {
        for(u32 x = 0; x <= 15; x++) {
            u32 xt = x + x0;
            u32 yt = y + y0;

            const struct Tile *tile = &tile_list[LEVEL_GET_TILE(level, xt, yt)];
            u16 tiles[4];
            tile->draw(level, xt, yt, tiles);

            vu16 *vram_tile_0 = &CHAR_BLOCK_1[x * 2 + y * 2 * 32];
            *vram_tile_0        = tiles[0];
            *(vram_tile_0 + 1)  = tiles[1];
            *(vram_tile_0 + 32) = tiles[2];
            *(vram_tile_0 + 33) = tiles[3];
        }
    }
}
