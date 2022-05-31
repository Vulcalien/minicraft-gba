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

void level_tick(struct Level *level) {
}

void level_draw(struct Level *level) {
    u32 x_offset = 0;
    u32 y_offset = 0;

    u32 x0 = x_offset >> 4;
    u32 y0 = y_offset >> 4;

    u32 x1 = x0 + 14;
    u32 y1 = y0 + 9;

    for(u32 y = y0; y <= y1; y++) {
        for(u32 x = x0; x <= x1; x++) {

        }
    }
}
