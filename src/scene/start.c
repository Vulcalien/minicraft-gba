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
#include "generator.h"
#include "screen.h"
#include "storage.h"
#include "sound.h"

static i8 start_selected;
static bool start_can_load;

static void start_init(void) {
    start_can_load = storage_check();

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
        if(start_selected == 0) {
            SOUND_PLAY(sound_start);

            srand(tick_count);
            storage_load();

            set_scene(&scene_game, true);
        } else if(start_selected == 1) {
            SOUND_PLAY(sound_start);

            srand(tick_count);
            generate_levels();

            set_scene(&scene_game, true);
        } else if(start_selected == 2) {
            set_scene(&scene_instructions, false);
        } else if(start_selected == 3) {
            set_scene(&scene_about, false);
        }
    }
}

#define START_WRITE(text, highlight, x, y) do {\
    screen_write((text), 1 - ((highlight) == true), (x), (y));\
\
    if(highlight) {\
        screen_write(">", 0, (x) - 2, (y));\
        screen_write("<", 0, (x) + sizeof(text), (y));\
    }\
} while(0)
static void start_draw(void) {
    // clear the screen
    for(u32 y = 0; y < 20; y++)
        for(u32 x = 0; x < 30; x++)
            BG3_TILEMAP[x + y * 32] = 29 | 1 << 12;

    // draw logo
    for(u32 y = 0; y < 2; y++) {
        for(u32 x = 0; x < 14; x++) {
            const u16 tile = (1 + (x + y * 14)) | 1 << 12;
            BG3_TILEMAP[(x + 8) + (y + 4) * 32] = tile;
        }
    }

    if(start_can_load)
        START_WRITE("LOAD GAME", start_selected == 0, 10, 9);
    START_WRITE("NEW  GAME", start_selected == 1, 10, 10);

    START_WRITE("HOW TO PLAY", start_selected == 2, 9, 12);
    START_WRITE("ABOUT", start_selected == 3, 12, 13);
}
#undef START_WRITE

const struct Scene scene_start = {
    .init = start_init,

    .tick = start_tick,
    .draw = start_draw
};
