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

static u8 win_time;

THUMB
static void win_init(u8 flags) {
    win_time = 0;
}

THUMB
static void win_tick(void) {
    win_time++;
    if(win_time > 60) {
        if(INPUT_CLICKED(KEY_A) || INPUT_CLICKED(KEY_B))
            set_scene(&scene_game, 1);
    }
}

THUMB
static void win_draw(void) {
    const u8 win_x = 6;
    const u8 win_y = 5;
    const u8 win_w = 18;
    const u8 win_h = 7;

    screen_draw_frame("", win_x, win_y, win_w, win_h);
    screen_write("YOU WON! YAY!", 6, win_x + 1, win_y + 1);

    screen_write("TIME:", 6, win_x + 1, win_y + 2);
    screen_write_time(gametime, 10, win_x + 6, win_y + 2);

    screen_write("SCORE:", 6, win_x + 1, win_y + 3);
    SCREEN_WRITE_NUMBER(score, 10, 10, false, 10, win_x + 7, win_y + 3);

    screen_write("PRESS A TO WIN", 8, win_x + 1, win_y + 5);
}

const struct Scene scene_win = {
    .init = win_init,

    .tick = win_tick,
    .draw = win_draw
};
