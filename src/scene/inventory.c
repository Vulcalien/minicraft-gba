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

        // Bug in the original game: when opening the inventory menu, thus
        // adding the active item to the inventory, the item is always put
        // in slot 0, regardless of it being a resource item or not.
        // This means that if the inventory already contains that resource,
        // there will be two distinct items for the same resource.
        //
        // This is not a duplication bug, however it causes annoyances for
        // crafting. To merge the items, the player can use a chest.
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

static void inventory_draw(void) {
    const u8 inv_x = 9;
    const u8 inv_y = 2;
    const u8 inv_w = 12;
    const u8 inv_h = 14;

    // clear displayed active item
    for(u32 x = 20; x < 30; x++)
        BG3_TILEMAP[x + 18 * 32] = 29;

    screen_draw_frame("INVENTORY", inv_x, inv_y, inv_w, inv_h);

    i8 item0 = inventory_selected - (inv_h - 2) / 2;
    if(item0 > player_inventory.size - (inv_h - 2))
        item0 = player_inventory.size - (inv_h - 2);
    if(item0 < 0)
        item0 = 0;

    // draw inventory items
    for(u32 i = 0; i < inv_h - 2; i++) {
        if(item0 + i >= player_inventory.size)
            break;

        struct item_Data *data = &player_inventory.items[item0 + i];

        item_draw_icon(data, inv_x + 1, inv_y + 1 + i, false);
        item_write(data, 4, inv_x + 2, inv_y + 1 + i);
    }

    // draw cursor arrows
    screen_write(
        ">", 4, inv_x            , inv_y + 1 + (inventory_selected - item0)
    );
    screen_write(
        "<", 4, inv_x + inv_w - 1, inv_y + 1 + (inventory_selected - item0)
    );
}

const struct Scene scene_inventory = {
    .init = inventory_init,

    .tick = inventory_tick,
    .draw = inventory_draw
};
