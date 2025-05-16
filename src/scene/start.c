/* Copyright 2022-2024 Vulcalien
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

#include <random.h>

#include "input.h"
#include "generator.h"
#include "furniture.h"
#include "screen.h"
#include "storage.h"
#include "sound.h"

#define LOAD_GAME   (0)
#define NEW_GAME    (1)
#define OPTIONS     (2)
#define HOW_TO_PLAY (3)
#define ABOUT       (4)

static i8 selected;
static bool can_load;
static bool checksum_verified;

THUMB
static void start_init(u8 flags) {
    can_load = storage_check();

    if(can_load) {
        selected = LOAD_GAME;

        checksum_verified = storage_verify_checksum();
        storage_srand();

        storage_load_options();
    } else {
        selected = NEW_GAME;
    }
}

THUMB
static void start_tick(void) {
    if(input_clicked(KEY_UP))
        selected--;
    if(input_clicked(KEY_DOWN))
        selected++;

    if(selected < (can_load ? LOAD_GAME : NEW_GAME))
        selected = ABOUT;
    else if(selected > ABOUT)
        selected = (can_load ? LOAD_GAME : NEW_GAME);

    if(input_pressed(KEY_A) || input_pressed(KEY_B)) {
        switch(selected) {
            case LOAD_GAME:
                SOUND_PLAY(sound_start);

                // add 'tick_count' to current random seed
                random_seed(tick_count + random_seed(0));

                storage_load();
                set_scene(&scene_game, 7);
                break;

            case NEW_GAME:
                SOUND_PLAY(sound_start);

                // add 'tick_count' to current random seed
                random_seed(tick_count + random_seed(0));

                generate_levels();

                gametime = 0;
                score = 0;

                current_level = 3;

                chest_count = 0;
                for(u32 i = 0; i < CHEST_LIMIT; i++)
                    chest_inventories[i].size = 0;

                set_scene(&scene_game, 7);
                break;

            case OPTIONS:
                set_scene(&scene_options, 1);
                break;

            case HOW_TO_PLAY:
                set_scene(&scene_instructions, 0);
                break;

            case ABOUT:
                set_scene(&scene_about, 0);
                break;
        }
    }
}

#define START_WRITE(text, id, x, y) do {\
    screen_write((text), selected == (id) ? 0 : 1, (x), (y));\
    if(selected == (id)) {\
        screen_write(">", 0, (x) - 2, (y));\
        screen_write("<", 0, (x) + sizeof(text), (y));\
    }\
} while(0)

THUMB
static void start_draw(void) {
    // clear the screen
    for(u32 y = 0; y < 20; y++)
        for(u32 x = 0; x < 30; x++)
            BG3_TILEMAP[x + y * 32] = 32;

    // draw logo
    for(u32 y = 0; y < 2; y++) {
        for(u32 x = 0; x < 14; x++) {
            const u16 tile = (1 + (x + y * 14)) | 2 << 12;
            BG3_TILEMAP[(x + 8) + (y + 4) * 32] = tile;
        }
    }

    if(can_load) {
        START_WRITE("LOAD GAME", LOAD_GAME, 10, 9);
        if(!checksum_verified) {
            screen_write("(!)", 2, 22, 9);

            screen_write("(!) INVALID CHECKSUM", 2, 1, 17);
        }
    }
    START_WRITE("NEW  GAME", NEW_GAME, 10, 10);

    START_WRITE("OPTIONS", OPTIONS, 11, 12);

    START_WRITE("HOW TO PLAY", HOW_TO_PLAY, 9, 14);
    START_WRITE("ABOUT", ABOUT, 12, 15);

    screen_write("V1.3+", 1, 25, 19);
}

const struct Scene scene_start = {
    .init = start_init,

    .tick = start_tick,
    .draw = start_draw
};
