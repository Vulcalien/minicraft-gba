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

static void about_tick(void) {
    if(INPUT_CLICKED(KEY_A) || INPUT_CLICKED(KEY_B))
        set_scene(&scene_start, false);
}

static void about_draw(void) {
    // clear the screen
    for(u32 y = 0; y < 20; y++) {
        for(u32 x = 0; x < 30; x += 2) {
            const u16 tile = 29 | (1 << 12);

            *((vu32 *) &BG3_TILEMAP[x + y * 32]) = (tile << 16) | tile;
        }
    }

    screen_write("ABOUT MINICRAFT FOR GBA", 0, 3, 1);

    // TODO say more stuff
    screen_write(
        "THIS IS A GAME BOY ADVANCE\nPORT OF THE GAME MINICRAFT.",
        2, 1, 3
    );

    screen_write("ABOUT MINICRAFT", 0, 7, 9);
    screen_write("(NOTCH'S WORDS)", 1, 7, 10);

    screen_write(
        "MINICRAFT WAS MADE BY MARKUS\n"
        "PERSSON FOR THE 22'ND LUDUM\n"
        "DARE COMPETITION IN DECEMBER\n"
        "2011.\n"
        "\n"
        "IT IS DEDICATED TO MY FATHER.\n"
        "<3",
        2, 1, 12
    );
}

const struct Scene scene_about = {
    .tick = about_tick,
    .draw = about_draw
};
