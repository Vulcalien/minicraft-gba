/* Copyright 2024 Vulcalien
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

#include "options.h"
#include "screen.h"

#define KEEP_INVENTORY (0)
#define EXIT           (1)

static i8 selected;

THUMB
static void options_init(u8 flags) {
    selected = 0;
}

THUMB
static void options_tick(void) {
    if(input_repeat(KEY_UP))
        selected--;
    if(input_repeat(KEY_DOWN))
        selected++;

    if(selected < 0)
        selected = EXIT;
    else if(selected > EXIT)
        selected = 0;

    if(input_press(KEY_B) || input_press(KEY_START))
        set_scene(&scene_start, 0);

    if(input_press(KEY_A)) {
        switch(selected) {
            case KEEP_INVENTORY:
                options.keep_inventory = !options.keep_inventory;
                break;

            case EXIT:
                set_scene(&scene_start, 0);
                break;
        }
    }
}

#define WRITE_OPTION(text, id, x, y) do {\
    screen_write((text), selected == (id) ? 0 : 2, (x), (y));\
    if(selected == (id)) {\
        if((id) != EXIT) {\
            screen_write(">", 0, 1,  (y));\
            screen_write("<", 0, 28, (y));\
        } else {\
            screen_write(">", 0, 11, (y));\
            screen_write("<", 0, 18, (y));\
        }\
    }\
} while(0)

THUMB
static void options_draw(void) {
    // clear the screen
    for(u32 y = 0; y < 20; y++)
        for(u32 x = 0; x < 30; x++)
            BG3_TILEMAP[x + y * 32] = 32;

    screen_write("GAME OPTIONS", 0, 9, 1);

    WRITE_OPTION("KEEP INVENTORY", KEEP_INVENTORY, 3, 4);
    if(options.keep_inventory)
        screen_write("YES", 4, 24, 4);
    else
        screen_write("NO", 5, 25, 4);

    // write 'EXIT'
    WRITE_OPTION("EXIT", EXIT, 13, 18);
}

const struct Scene scene_options = {
    .init = options_init,

    .tick = options_tick,
    .draw = options_draw
};
