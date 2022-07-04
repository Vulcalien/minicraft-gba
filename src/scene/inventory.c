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
#include "item.h"
#include "player.h"

static i8 inventory_selected;

static void inventory_init(void) {
    inventory_selected = 0;
}

static void inventory_tick(void) {
    if(INPUT_CLICKED(KEY_B))
        set_scene(&scene_game, true);

    if(player_inventory.size == 0)
        return;

    if(INPUT_CLICKED(KEY_UP))
        inventory_selected--;
    if(INPUT_CLICKED(KEY_DOWN))
        inventory_selected++;

    if(inventory_selected < 0)
        inventory_selected = player_inventory.size - 1;
    if(inventory_selected >= player_inventory.size)
        inventory_selected = 0;

    if(INPUT_CLICKED(KEY_A)) {
        // TODO set active item

        set_scene(&scene_game, true);
    }
}

#define SET_TILE(x, y, id, flip, palette)\
    do {\
        BG3_TILEMAP[(x) + (y) * 32] = (id) |\
                                      ((flip) << 10) |\
                                      ((palette) << 12);\
    } while(0)

static void inventory_draw(void) {
    // TODO change these values
    #define INV_W  (11)
    #define INV_H  (14)

    #define INV_X0 (2)
    #define INV_Y0 (2)
    #define INV_X1 (INV_X0 + INV_W)
    #define INV_Y1 (INV_Y0 + INV_H)

    // draw corners
    SET_TILE(INV_X0, INV_Y0, 88, 0, 4);
    SET_TILE(INV_X1, INV_Y0, 88, 1, 4);
    SET_TILE(INV_X0, INV_Y1, 88, 2, 4);
    SET_TILE(INV_X1, INV_Y1, 88, 3, 4);

    // draw vertical borders
    for(u32 y = INV_Y0 + 1; y <= INV_Y1 - 1; y++) {
        SET_TILE(INV_X0, y, 90, 0, 4);
        SET_TILE(INV_X1, y, 90, 1, 4);

        // draw background
        for(u32 x = INV_X0 + 1; x <= INV_X1 - 1; x++)
            SET_TILE(x, y, 29, 0, 4);
    }

    // draw horizontal borders
    for(u32 x = INV_X0 + 1; x <= INV_X1 - 1; x++) {
        SET_TILE(x, INV_Y0, 89, 0, 4);
        SET_TILE(x, INV_Y1, 89, 2, 4);
    }

    // DEBUG use yellow font + blue bg
    screen_write("INVENTORY", 0, INV_X0 + 1, INV_Y0);

    i8 item0 = inventory_selected - (INV_H - 2) / 2;
    if(item0 > player_inventory.size - (INV_H - 1))
        item0 = player_inventory.size - (INV_H - 1);
    if(item0 < 0)
        item0 = 0;

    // draw inventory items
    for(u32 i = 0; i < INV_H - 1; i++) {
        if(item0 + i >= player_inventory.size)
            break;

        struct item_Data *data = &player_inventory.items[item0 + i];

        item_draw_icon(data, INV_X0 + 1, INV_Y0 + 1 + i);
        item_write(data, 4, INV_X0 + 2, INV_Y0 + 1 + i);
    }

    // draw cursor arrows
    screen_write(">", 4, INV_X0, INV_Y0 + 1 + (inventory_selected - item0));
    screen_write("<", 4, INV_X1, INV_Y0 + 1 + (inventory_selected - item0));
}
#undef SET_TILE

const struct Scene scene_inventory = {
    .init = inventory_init,

    .tick = inventory_tick,
    .draw = inventory_draw
};
