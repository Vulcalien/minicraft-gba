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

static void pause_tick(void) {
    if(INPUT_CLICKED(KEY_START))
        set_scene(&scene_game, true);
}

static void pause_draw(void) {
    const u8 pause_x = 2;
    const u8 pause_y = 2;
    const u8 pause_w = 18;
    const u8 pause_h = 5;

    screen_draw_frame("PAUSE", pause_x, pause_y, pause_w, pause_h);

    screen_write("TIME:", 4, pause_x + 1, pause_y + 1);
    screen_write_time(gametime, 8, pause_x + 6, pause_y + 1);

    screen_write("SCORE:", 4, pause_x + 1, pause_y + 2);
    SCREEN_WRITE_NUMBER(score, 10, 8, pause_x + 7, pause_y + 2);

    // TODO save game
}

const struct Scene scene_pause = {
    .tick = pause_tick,
    .draw = pause_draw
};
