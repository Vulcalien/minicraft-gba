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

static u8 death_time;

static void death_init(u8 flags) {
    death_time = 0;
}

static void death_tick(void) {
    death_time++;
    if(death_time > 60) {
        if(INPUT_CLICKED(KEY_A) || INPUT_CLICKED(KEY_B))
            set_scene(&scene_start, 1);
    }
}

static void death_draw(void) {
    const u8 death_x = 6;
    const u8 death_y = 5;
    const u8 death_w = 18;
    const u8 death_h = 7;

    screen_draw_frame("", death_x, death_y, death_w, death_h);
    screen_write("YOU DIED! AWW!", 4, death_x + 1, death_y + 1);

    screen_write("TIME:", 4, death_x + 1, death_y + 2);
    screen_write_time(gametime, 8, death_x + 6, death_y + 2);

    screen_write("SCORE:", 4, death_x + 1, death_y + 3);
    SCREEN_WRITE_NUMBER(score, 10, 8, death_x + 7, death_y + 3);

    screen_write("PRESS A TO LOSE", 6, death_x + 1, death_y + 5);
}

const struct Scene scene_death = {
    .init = death_init,

    .tick = death_tick,
    .draw = death_draw
};
