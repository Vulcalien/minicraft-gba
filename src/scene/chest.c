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

static i32 chest_selected[2] = { 0, 0 };
static u8 chest_window;

static struct Inventory *chest_inventory;

static void chest_init(u8 flags) {
    chest_selected[0] = 0;
    chest_selected[1] = 0;

    chest_window = 0;

    chest_inventory = &chest_inventories[chest_opened_id];
}

static void chest_tick(void) {
    gametime++;

    if(INPUT_CLICKED(KEY_B))
        set_scene(&scene_game, 1);

    if(INPUT_CLICKED(KEY_LEFT))
        chest_window = 0;
    if(INPUT_CLICKED(KEY_RIGHT))
        chest_window = 1;

    struct Inventory *inv[2];
    inv[chest_window] = chest_inventory;
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
        if(item_is_resource(removed.type)) {
            could_add = inventory_add_resource(
                inv[1],
                removed.type, removed.count,
                chest_selected[chest_window ^ 1]
            );
        } else {
            could_add = inventory_add(
                inv[1], &removed, chest_selected[chest_window ^ 1]
            );
        }

        if(could_add) {
            if(chest_selected[chest_window] == inv[0]->size &&
               inv[0]->size > 0) {
                chest_selected[chest_window]--;
            }
        } else {
            // put the item back into the current inventory
            inventory_add(inv[0], &removed, chest_selected[chest_window]);
        }
    }
}

static void chest_draw(void) {
    const u8 chest_x = 2 + (chest_window == 0) * 2;
    const u8 chest_y = 2;
    const u8 chest_w = 12;
    const u8 chest_h = 14;

    const u8 inv_x = 16 - (chest_window == 1) * 2;
    const u8 inv_y = 2;
    const u8 inv_w = chest_w;
    const u8 inv_h = chest_h;

    // clear the screen (fully transparent)
    for(u32 y = 0; y < 18; y++)
        for(u32 x = 0; x < 30; x += 2)
            *((vu32 *) &BG3_TILEMAP[x + y * 32]) = 0;

    screen_draw_frame("CHEST", chest_x, chest_y, chest_w, chest_h);
    screen_draw_frame("INVENTORY", inv_x, inv_y, inv_w, inv_h);

    // draw chest items
    {
        i8 item0 = chest_selected[0] - (chest_h - 2) / 2;
        if(item0 > chest_inventory->size - (chest_h - 2))
            item0 = chest_inventory->size - (chest_h - 2);
        if(item0 < 0)
            item0 = 0;

        for(u32 i = 0; i < chest_h - 2; i++) {
            if(item0 + i >= chest_inventory->size)
                break;

            struct item_Data *data = &chest_inventory->items[item0 + i];

            item_draw_icon(data, chest_x + 1, chest_y + 1 + i, false);
            item_write(data, 4, chest_x + 2, chest_y + 1 + i);
        }

        if(chest_window == 0) {
            screen_write(
                ">", 4,
                chest_x,
                chest_y + 1 + (chest_selected[0] - item0)
            );
            screen_write(
                "<", 4,
                chest_x + chest_w - 1,
                chest_y + 1 + (chest_selected[0] - item0)
            );
        }
    }

    // draw inventory items
    {
        i8 item0 = chest_selected[1] - (inv_h - 2) / 2;
        if(item0 > player_inventory.size - (inv_h - 2))
            item0 = player_inventory.size - (inv_h - 2);
        if(item0 < 0)
            item0 = 0;

        for(u32 i = 0; i < inv_h - 2; i++) {
            if(item0 + i >= player_inventory.size)
                break;

            struct item_Data *data = &player_inventory.items[item0 + i];

            item_draw_icon(data, inv_x + 1, inv_y + 1 + i, false);
            item_write(data, 4, inv_x + 2, inv_y + 1 + i);
        }

        if(chest_window == 1) {
            screen_write(
                ">", 4,
                inv_x,
                inv_y + 1 + (chest_selected[1] - item0)
            );
            screen_write(
                "<", 4,
                inv_x + inv_w - 1,
                inv_y + 1 + (chest_selected[1] - item0)
            );
        }
    }
}

const struct Scene scene_chest = {
    .init = chest_init,

    .tick = chest_tick,
    .draw = chest_draw
};
