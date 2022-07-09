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

#include "inventory.h"
#include "input.h"
#include "screen.h"
#include "item.h"
#include "player.h"

static i8 inventory_selected;

static void inventory_init(void) {
    inventory_selected = 0;

    if(player_active_item.type < ITEM_TYPES) {
        // FIXME currently, the active item is deleted even if
        // inventory_add fails. find a way to either prevent it
        // or to make it impossible for inventory_add to fail
        inventory_add(&player_inventory, &player_active_item, 0);
        player_active_item.type = -1;
    }
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
        inventory_remove(
            &player_inventory, &player_active_item, inventory_selected
        );

        // copy item palette
        const struct Item *item = ITEM_S(&player_active_item);
        screen_load_active_item_palette(item->palette);

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
    #define INV_X (2)
    #define INV_Y (2)
    #define INV_W (12)
    #define INV_H (14)

    // clear displayed active item
    for(u32 x = 20; x < 30; x++)
        SET_TILE(x, 18, 29, 0, 0);

    screen_draw_frame("INVENTORY", INV_X, INV_Y, INV_W, INV_H);

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

        item_draw_icon(data, INV_X + 1, INV_Y + 1 + i, false);
        item_write(data, 4, INV_X + 2, INV_Y + 1 + i);
    }

    // draw cursor arrows
    screen_write(
        ">", 4, INV_X,             INV_Y + 1 + (inventory_selected - item0)
    );
    screen_write(
        "<", 4, INV_X + INV_W - 1, INV_Y + 1 + (inventory_selected - item0)
    );
}
#undef SET_TILE

#undef INV_X
#undef INV_Y
#undef INV_W
#undef INV_H

const struct Scene scene_inventory = {
    .init = inventory_init,

    .tick = inventory_tick,
    .draw = inventory_draw
};
