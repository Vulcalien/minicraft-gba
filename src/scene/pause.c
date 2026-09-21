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

#include "screen.h"
#include "storage.h"
#include "sound.h"
#include "i18n.h"

static bool ask_overwrite;
static u8 selected_answer;
static u8 selected_option;

static bool should_save = false;

#define SAVE_OPTION (0)
#define QUIT_OPTION (1)

THUMB
static void pause_init(u8 flags) {
    ask_overwrite = false;
    selected_option = SAVE_OPTION;
}

THUMB
static void pause_tick(void) {
    if(should_save) {
        storage_save();
        SOUND_PLAY(sound_start);

        should_save = false;
        ask_overwrite = false;
    }

    if(input_press(KEY_START))
        set_scene(&scene_game, 1);

    if(!ask_overwrite && (input_repeat(KEY_UP) || input_repeat(KEY_DOWN)))
        selected_option ^= 1;

    if(input_repeat(KEY_A)) {
        if(ask_overwrite) {
            if(selected_answer == 1)
                ask_overwrite = false;
            else
                should_save = true;
        } else if(selected_option == QUIT_OPTION) {
            set_scene(&scene_start, 1);
        } else if(storage_check()) {
            ask_overwrite = true;
            selected_answer = 0;
        } else {
            should_save = true;
        }
    }

    if(input_repeat(KEY_B))
        ask_overwrite = false;

    if(ask_overwrite && (input_repeat(KEY_LEFT) || input_repeat(KEY_RIGHT)))
        selected_answer ^= 1;
}

#define WRITE_ANSWER(text_id, id, x, y) do {\
    const char *value = text(text_id);\
    screen_write(value, 7 - (selected_answer == id), (x), (y));\
\
    if(selected_answer == id) {\
        screen_write(">", 6, (x) - 2, (y));\
        screen_write("<", 6, (x) + text_length(value) + 1, (y));\
    }\
} while(0)

#define WRITE_MENU_OPTION(text_id, id, x, y) do {\
    const char *value = text(text_id);\
    screen_write(value, 7 - (selected_option == id), (x), (y));\
\
    if(selected_option == id) {\
        screen_write(">", 6, (x) - 2, (y));\
        screen_write("<", 6, (x) + text_length(value) + 1, (y));\
    }\
} while(0)

THUMB
static void pause_draw(void) {
    const u8 pause_x = 6;
    const u8 pause_y = 4;
    const u8 pause_w = 18;
    const u8 pause_h = 11;
    const u8 stats_value_x = pause_x + 7;

    screen_draw_frame("", pause_x, pause_y, pause_w, pause_h);
    screen_write(text(TEXT_PAUSE), 10, pause_x + 1, pause_y + 1);

    screen_write(text(TEXT_TIME), 6, pause_x + 1, pause_y + 3);
    screen_write_time(gametime, 10, stats_value_x, pause_y + 3);

    screen_write(text(TEXT_SCORE), 6, pause_x + 1, pause_y + 4);
    SCREEN_WRITE_NUMBER(score, 10, 10, false, 10, stats_value_x, pause_y + 4);

    if(should_save) {
        screen_write(text(TEXT_SAVING), 6, pause_x + 5, pause_y + 6);
    } else if(ask_overwrite) {
        screen_write(text(TEXT_OVERWRITE_FILE), 6, pause_x + 1, pause_y + 6);

        WRITE_ANSWER(TEXT_YES, 0, pause_x + 4,  pause_y + 8);
        WRITE_ANSWER(TEXT_NO, 1, pause_x + 12, pause_y + 8);
    } else {
        WRITE_MENU_OPTION(TEXT_SAVE_GAME, SAVE_OPTION, pause_x + 2, pause_y + 6);
        WRITE_MENU_OPTION(TEXT_QUIT, QUIT_OPTION, pause_x + 2, pause_y + 8);
    }
}

const struct Scene scene_pause = {
    .init = pause_init,

    .tick = pause_tick,
    .draw = pause_draw
};
