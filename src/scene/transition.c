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

#include "input.h"
#include "screen.h"

static u8 transition_time;

static void transition_init(void) {
    transition_time = 0;
}

static void transition_tick(void) {
    gametime++;

    transition_time++;
    if(transition_time == 15)
        scene_game.init();
    else if(transition_time == 30)
        set_scene(&scene_game, true);
}

static void transition_draw(void) {
    // FIXME very incorrect
    for(u32 y = 0; y < 20; y++) {
        for(u32 x = 0; x < 30; x++) {
            i32 dd = (y + x % 2 * 2 + x / 3) - transition_time;
            if(dd > -30 && dd < 0)
                BG3_TILEMAP[x + y * 32] = 0;
            else
                BG3_TILEMAP[x + y * 32] = 29;
        }
    }

    scene_game.draw();
}

const struct Scene scene_transition = {
    .init = transition_init,

    .tick = transition_tick,
    .draw = transition_draw
};
