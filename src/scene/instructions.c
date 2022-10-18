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

THUMB
static void instructions_tick(void) {
    if(INPUT_CLICKED(KEY_A) || INPUT_CLICKED(KEY_B))
        set_scene(&scene_start, 0);
}

THUMB
static void instructions_draw(void) {
    // clear the screen
    for(u32 y = 0; y < 20; y++)
        for(u32 x = 0; x < 30; x++)
            BG3_TILEMAP[x + y * 32] = 29 | 1 << 12;

    screen_write("HOW TO PLAY", 0, 9, 1);

    screen_write(
        "MOVE YOUR CHARACTER USING\n"
        "THE DPAD.\n"
        "\n"
        "PRESS A TO ATTACK AND TO USE\n"
        "ITEMS AND B TO OPEN THE\n"
        "INVENTORY.\n"
        "\n"
        "SELECT AN ITEM IN THE\n"
        "INVENTORY TO EQUIP IT.\n"
        "\n"
        "KILL THE AIR WIZARD TO WIN\n"
        "THE GAME!\n"
        "\n"
        "\n"
        "PRESS START TO PAUSE AND TO\n"
        "SAVE THE GAME.",
        2, 1, 3
    );
}

const struct Scene scene_instructions = {
    .tick = instructions_tick,
    .draw = instructions_draw
};
