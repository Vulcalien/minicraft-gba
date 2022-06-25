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

#include "input.h"
#include "screen.h"

static i8 start_selected;
static bool start_can_load;

static void start_init(void) {
    start_can_load = false;

    if(start_can_load)
        start_selected = 0;
    else
        start_selected = 1;
}

static void start_tick(void) {
    if(INPUT_CLICKED(KEY_UP))
        start_selected--;
    if(INPUT_CLICKED(KEY_DOWN))
        start_selected++;

    if(start_selected < (start_can_load == false))
        start_selected = 3;
    else if(start_selected >= 4)
        start_selected = (start_can_load == false);

    if(INPUT_CLICKED(KEY_A) || INPUT_CLICKED(KEY_B)) {
    }
}

static void start_draw(void) {
    // clear the screen
    for(u32 y = 0; y < 20; y++) {
        for(u32 x = 0; x < 30; x += 2) {
            const u16 tile = 29 | (1 << 12);

            *((vu32 *) &BG3_TILEMAP[x + y * 32]) = (tile << 16) | tile;
        }
    }

    // draw logo
    for(u32 y = 0; y < 2; y++) {
        for(u32 x = 0; x < 14; x++) {
            const u16 tile = (1 + (x + y * 14)) | (1 << 12);
            BG3_TILEMAP[(x + 8) + (y + 4) * 32] = tile;
        }
    }

    if(start_can_load)
        screen_write("LOAD GAME", 0 + (start_selected != 0) * 1, 10, 9);
    screen_write("NEW  GAME", 0 + (start_selected != 1) * 1, 10, 10);

    screen_write("HOW TO PLAY", 0 + (start_selected != 2) * 1, 9, 12);
    screen_write("ABOUT",       0 + (start_selected != 3) * 1, 12, 13);

    // TODO draw arrows
}

const struct Menu menu_start = {
    .init = start_init,

    .tick = start_tick,
    .draw = start_draw
};
