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

static i32 selected;
static bool should_render_game = false;

THUMB
static void inventory_init(u8 flags) {
    selected = 0;

    // Bug in the original game: when opening the inventory menu, thus
    // adding the active item to the inventory, the item is always put
    // in slot 0, regardless of it being a resource item or not.
    // This means that if the inventory already contains that resource,
    // there will be two distinct items for the same resource.
    //
    // This is not a duplication bug, however it causes annoyances for
    // crafting. To merge the items, the player can use a chest.
    if(player_active_item.type < ITEM_TYPES)
        if(inventory_add(&player_inventory, &player_active_item, 0))
            player_active_item.type = -1;

    should_render_game = true;
}

THUMB
static void inventory_tick(void) {
    gametime++;

    if(input_clicked(KEY_B))
        set_scene(&scene_game, 1);

    if(player_inventory.size == 0)
        return;

    if(input_clicked(KEY_UP))
        selected--;
    if(input_clicked(KEY_DOWN))
        selected++;

    if(selected < 0)
        selected = player_inventory.size - 1;
    if(selected >= player_inventory.size)
        selected = 0;

    if(input_clicked(KEY_A)) {
        struct item_Data old_active_item = player_active_item;

        inventory_remove(&player_inventory, &player_active_item, selected);

        // If there was an active item, put it back into the inventory.
        // This only happens when the inventory is full.
        if(old_active_item.type < ITEM_TYPES)
            inventory_add(&player_inventory, &old_active_item, 0);

        set_scene(&scene_game, 1);
    }
}

THUMB
static void inventory_draw(void) {
    if(should_render_game) {
        scene_game.draw();
        should_render_game = false;
    }

    const u8 inv_x = 9;
    const u8 inv_y = 2;
    const u8 inv_w = 12;
    const u8 inv_h = 14;

    screen_draw_frame("INVENTORY", inv_x, inv_y, inv_w, inv_h);

    i8 item0 = selected - (inv_h - 2) / 2;
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
        item_write(data, 6, inv_x + 2, inv_y + 1 + i);
    }

    // draw cursor arrows
    screen_write(
        ">", 6, inv_x            , inv_y + 1 + (selected - item0)
    );
    screen_write(
        "<", 6, inv_x + inv_w - 1, inv_y + 1 + (selected - item0)
    );
}

const struct Scene scene_inventory = {
    .init = inventory_init,

    .tick = inventory_tick,
    .draw = inventory_draw
};
