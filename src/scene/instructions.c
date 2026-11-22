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
#include "i18n.h"

THUMB
static void instructions_tick(void) {
    if(input_press(KEY_B) || input_press(KEY_START))
        set_scene(&scene_start, 0);
}

THUMB
static void instructions_draw(void) {
    // clear the screen
    for(u32 y = 0; y < 20; y++)
        for(u32 x = 0; x < 30; x++)
            BG3_TILEMAP[x + y * 32] = 32;

    screen_write(text(TEXT_INSTRUCTIONS), 0, 8, 1);

    screen_write(text(TEXT_INSTRUCTIONS_BODY), 2, 1, 3);
}

const struct Scene scene_instructions = {
    .tick = instructions_tick,
    .draw = instructions_draw
};
