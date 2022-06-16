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
#include "menu.h"

#include "screen.h"

static void start_tick(void) {

}

static void start_draw(void) {
    // clear the screen
    for(u32 y = 0; y < 20; y++) {
        for(u32 x = 0; x < 30; x += 2) {
            u32 tile = 1 | (0 << 12);

            *((vu32 *) &BG3_TILEMAP[x + y * 32]) = (tile << 16) | tile;
        }
    }

    // draw logo
    for(u32 y = 0; y < 2; y++) {
        for(u32 x = 0; x < 14; x++) {
            u32 tile = (2 + (x + y * 14)) | (1 << 12);
            BG3_TILEMAP[(x + 8) + (y + 4) * 32] = tile;
        }
    }
}

const struct Menu menu_start = {
    .tick = start_tick,
    .draw = start_draw
};
