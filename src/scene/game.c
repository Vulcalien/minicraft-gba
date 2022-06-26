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
#include "scene.h"

#include "level.h"
#include "screen.h"

struct Level *level;

static void game_init(void) {
    // DEBUG
    current_level = 0;

    level = &levels[current_level];

    // clear the screen (fully transparent)
    for(u32 y = 0; y < 20; y++)
        for(u32 x = 0; x < 30; x += 2)
            *((vu32 *) &BG3_TILEMAP[x + y * 32]) = 0;
}

static void game_tick(void) {
    level_tick(level);
}

static void game_draw(void) {
    level_draw(level);
}

const struct Scene scene_game = {
    .init = game_init,

    .tick = game_tick,
    .draw = game_draw
};
