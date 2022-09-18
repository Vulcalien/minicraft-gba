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

static u16 prestart_counter = 0;

static void prestart_tick(void) {
    prestart_counter++;

    if(prestart_counter == 0x20 + 2 * 60)
        set_scene(&scene_start, true);
}

static void prestart_draw(void) {
    if(prestart_counter < 0x20) {
        u8 val = 0x20 - prestart_counter;
        u16 color = val << 10 | val << 5 | val;

        screen_set_bg_palette_color(9, 0xd, color);
    }

    screen_write(
        "MINICRAFT WAS MADE BY MARKUS\n"
        "PERSSON FOR THE 22'ND LUDUM\n"
        "DARE COMPETITION IN DECEMBER\n"
        "2011.\n"
        "\n"
        "THIS GBA DEMAKE OF THE GAME\n"
        "WAS MADE BY VULCALIEN.\n"
        "\n"
        "THE ARTWORK AND SOUNDS WERE\n"
        "MADE BY MARKUS PERSSON.\n"
        "\n"
        "\n"
        "COPYRIGHT 2022 VULCALIEN\n"
        "\n"
        "THIS IS FREE SOFTWARE\n"
        "RELEASED UNDER THE\n"
        "GNU GENERAL PUBLIC LICENSE\n"
        "EITHER VERSION 3 OR LATER.",
        9, 1, 1
    );
}

const struct Scene scene_prestart = {
    .tick = prestart_tick,
    .draw = prestart_draw
};
