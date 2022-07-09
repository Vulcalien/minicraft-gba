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
#include "inventory.h"
#include "item.h"
#include "player.h"
#include "furniture.h"

static i8 chest_selected[2] = { 0, 0 };
static u8 chest_window;

static void chest_init(void) {
    chest_selected[0] = 0;
    chest_selected[0] = 1;

    chest_window = 0;
}

static void chest_tick(void) {
    if(INPUT_CLICKED(KEY_B))
        set_scene(&scene_game, true);

    if(INPUT_CLICKED(KEY_LEFT))
        chest_window = 0;
    if(INPUT_CLICKED(KEY_RIGHT))
        chest_window = 1;

    struct Inventory *inv[2];
    inv[chest_window] = &chest_inventories[chest_opened_id];
    inv[chest_window ^ 1] = &player_inventory;

    if(inv[0]->size == 0)
        return;

    if(INPUT_CLICKED(KEY_UP))
        chest_selected[chest_window]--;
    if(INPUT_CLICKED(KEY_DOWN))
        chest_selected[chest_window]++;

    if(chest_selected[chest_window] < 0)
        chest_selected[chest_window] = inv[0]->size - 1;
    if(chest_selected[chest_window] >= inv[0]->size)
        chest_selected[chest_window] = 0;

    if(INPUT_CLICKED(KEY_A)) {
        struct item_Data removed;
        inventory_remove(inv[0], &removed, chest_selected[chest_window]);

        bool could_add;
        const struct Item *item = ITEM_S(&removed);
        switch(item->class) {
            case ITEMCLASS_MATERIAL:
            case ITEMCLASS_PLACEABLE:
            case ITEMCLASS_FOOD:
                could_add = inventory_add_resource(
                    inv[1],
                    removed.type, removed.count,
                    chest_selected[chest_window ^ 1]
                );
                break;

            default:
                could_add = inventory_add(
                    inv[1], &removed, chest_selected[chest_window ^ 1]
                );
        }

        if(could_add) {
            if(chest_selected[chest_window] == inv[0]->size)
                chest_selected[chest_window]--;
        } else {
            // put the item back into the current inventory
            inventory_add(inv[1], &removed, chest_selected[chest_window]);
        }
    }
}

static void chest_draw(void) {
}

const struct Scene scene_chest = {
    .init = chest_init,

    .tick = chest_tick,
    .draw = chest_draw
};
