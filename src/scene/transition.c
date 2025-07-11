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

#include "screen.h"

static u8 transition_time;
static u8 old_level;

THUMB
static void transition_init(u8 flags) {
    transition_time = 0;
    old_level = current_level;
}

THUMB
static void transition_tick(void) {
    gametime++;

    transition_time++;
    if(transition_time == 17)
        scene_game.init(3);
    else if(transition_time == 31)
        set_scene(&scene_game, 1);
}

IWRAM_SECTION
static void transition_draw(void) {
    scene_game.draw();

    for(u32 y = 0; y < 20; y++) {
        for(u32 x = 0; x < 30; x++) {
            i32 dd = (y + x % 2 * 2 + x / 3) - transition_time * 2;

            u16 ydraw = old_level < current_level ? y : 19 - y;
            if(dd > -33 && dd < 0)
                BG3_TILEMAP[x + ydraw * 32] = 32;
            else if(ydraw < 18)
                BG3_TILEMAP[x + ydraw * 32] = 0;
        }
    }
}

const struct Scene scene_transition = {
    .init = transition_init,

    .tick = transition_tick,
    .draw = transition_draw
};
