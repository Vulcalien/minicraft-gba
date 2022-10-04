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
#include "storage.h"
#include "sound.h"

static bool pause_ask_overwrite;
static u8 pause_answer_selected;

static bool pause_should_save = false;

static void pause_init(u8 flags) {
    pause_ask_overwrite = false;
}

static void pause_tick(void) {
    if(pause_should_save) {
        storage_save();
        SOUND_PLAY(sound_start);

        pause_should_save = false;
        pause_ask_overwrite = false;
    }

    if(INPUT_CLICKED(KEY_START))
        set_scene(&scene_game, 1);

    if(INPUT_CLICKED(KEY_A)) {
        if(pause_ask_overwrite) {
            if(pause_answer_selected == 1)
                pause_ask_overwrite = false;
            else
                pause_should_save = true;
        } else if(storage_check()) {
            pause_ask_overwrite = true;
            pause_answer_selected = 0;
        } else {
            pause_should_save = true;
        }
    }

    if(INPUT_CLICKED(KEY_B))
        pause_ask_overwrite = false;

    if(pause_ask_overwrite &&
       (INPUT_CLICKED(KEY_LEFT) || INPUT_CLICKED(KEY_RIGHT))) {
        pause_answer_selected ^= 1;
    }
}

#define WRITE_OPTION(text, id, x, y) do {\
    screen_write((text), 5 - (pause_answer_selected == id), (x), (y));\
\
    if(pause_answer_selected == id) {\
        screen_write(">", 4, (x) - 2, (y));\
        screen_write("<", 4, (x) + sizeof(text), (y));\
    }\
} while(0)
static void pause_draw(void) {
    const u8 pause_x = 6;
    const u8 pause_y = 5;
    const u8 pause_w = 18;
    const u8 pause_h = 8;

    screen_draw_frame("PAUSE", pause_x, pause_y, pause_w, pause_h);

    screen_write("TIME:", 4, pause_x + 1, pause_y + 1);
    screen_write_time(gametime, 8, pause_x + 6, pause_y + 1);

    screen_write("SCORE:", 4, pause_x + 1, pause_y + 2);
    SCREEN_WRITE_NUMBER(score, 10, 10, false, 8, pause_x + 7, pause_y + 2);

    if(pause_should_save) {
        screen_write("SAVING...", 4, pause_x + 5, pause_y + 5);
    } else if(pause_ask_overwrite) {
        screen_write("OVERWRITE FILE?", 4, pause_x + 1, pause_y + 4);

        WRITE_OPTION("YES", 0, pause_x + 4,  pause_y + 6);
        WRITE_OPTION("NO",  1, pause_x + 12, pause_y + 6);
    } else {
        screen_write("> SAVE GAME <", 4, pause_x + 2, pause_y + 5);
    }
}
#undef WRITE_OPTION

const struct Scene scene_pause = {
    .init = pause_init,

    .tick = pause_tick,
    .draw = pause_draw
};
