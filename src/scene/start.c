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
#include "furniture.h"
#include "screen.h"
#include "storage.h"
#include "sound.h"

static i8 selected;
static bool can_load;

THUMB
static void start_init(u8 flags) {
    can_load = storage_check();

    if(can_load) {
        selected = 0;

        storage_srand();
    } else {
        selected = 1;
    }
}

THUMB
static void start_tick(void) {
    if(INPUT_CLICKED(KEY_UP))
        selected--;
    if(INPUT_CLICKED(KEY_DOWN))
        selected++;

    if(selected < (can_load == false))
        selected = 3;
    else if(selected >= 4)
        selected = (can_load == false);

    if(INPUT_CLICKED(KEY_A) || INPUT_CLICKED(KEY_B)) {
        if(selected == 0) {
            SOUND_PLAY(sound_start);

            srand(tick_count, false);
            storage_load();

            set_scene(&scene_game, 7);
        } else if(selected == 1) {
            SOUND_PLAY(sound_start);

            srand(tick_count, false);
            generate_levels();

            gametime = 0;
            score = 0;

            current_level = 3;

            chest_count = 0;
            for(u32 i = 0; i < CHEST_LIMIT; i++)
                chest_inventories[i].size = 0;

            set_scene(&scene_game, 7);
        } else if(selected == 2) {
            set_scene(&scene_instructions, 0);
        } else if(selected == 3) {
            set_scene(&scene_about, 0);
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

THUMB
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

    if(can_load)
        START_WRITE("LOAD GAME", selected == 0, 10, 9);
    START_WRITE("NEW  GAME", selected == 1, 10, 10);

    START_WRITE("HOW TO PLAY", selected == 2, 9, 12);
    START_WRITE("ABOUT", selected == 3, 12, 13);
}

const struct Scene scene_start = {
    .init = start_init,

    .tick = start_tick,
    .draw = start_draw
};
