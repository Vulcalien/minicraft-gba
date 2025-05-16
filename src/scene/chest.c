/* Copyright 2022, 2024 Vulcalien
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

static i32 selected[2] = { 0, 0 };
static u8 chest_window;

static struct Inventory *chest_inventory;

THUMB
static void chest_init(u8 flags) {
    selected[0] = 0;
    selected[1] = 0;

    chest_window = 0;

    chest_inventory = &chest_inventories[chest_opened_id];
}

THUMB
static void chest_tick(void) {
    gametime++;

    if(input_pressed(KEY_B) || input_pressed(KEY_START))
        set_scene(&scene_game, 1);

    if(input_clicked(KEY_LEFT))
        chest_window = 0;
    if(input_clicked(KEY_RIGHT))
        chest_window = 1;

    struct Inventory *inv[2];
    inv[chest_window] = chest_inventory;
    inv[chest_window ^ 1] = &player_inventory;

    if(inv[0]->size == 0)
        return;

    if(input_clicked(KEY_UP))
        selected[chest_window]--;
    if(input_clicked(KEY_DOWN))
        selected[chest_window]++;

    if(selected[chest_window] < 0)
        selected[chest_window] = inv[0]->size - 1;
    if(selected[chest_window] >= inv[0]->size)
        selected[chest_window] = 0;

    if(input_clicked(KEY_A)) {
        struct item_Data removed;
        inventory_remove(inv[0], &removed, selected[chest_window]);

        bool could_add;
        if(item_is_resource(removed.type)) {
            could_add = inventory_add_resource(
                inv[1],
                removed.type, removed.count,
                selected[chest_window ^ 1]
            );
        } else {
            could_add = inventory_add(
                inv[1], &removed, selected[chest_window ^ 1]
            );
        }

        if(could_add) {
            if(selected[chest_window] == inv[0]->size &&
               inv[0]->size > 0) {
                selected[chest_window]--;
            }
        } else {
            // put the item back into the current inventory
            inventory_add(inv[0], &removed, selected[chest_window]);
        }
    }
}

THUMB
static void chest_draw(void) {
    // clear the screen (fully transparent)
    for(u32 y = 0; y < 18; y++)
        for(u32 x = 0; x < 30; x += 2)
            *((vu32 *) &BG3_TILEMAP[x + y * 32]) = 0;

    for(u32 frame = 0; frame < 2; frame++) {
        u8 frame_x;
        const u8 frame_y = 2;
        const u8 frame_w = 12;
        const u8 frame_h = 14;

        struct Inventory *inventory;

        if(frame == 0) {
            frame_x = 2 + (chest_window == 0) * 2;
            inventory = chest_inventory;
        } else {
            frame_x = 16 - (chest_window == 1) * 2;
            inventory = &player_inventory;
        }

        // draw frame
        screen_draw_frame(
            (frame == 0 ? "CHEST" : "INVENTORY"),
            frame_x, frame_y, frame_w, frame_h
        );

        // draw items
        i8 item0 = selected[frame] - (frame_h - 2) / 2;
        if(item0 > inventory->size - (frame_h - 2))
            item0 = inventory->size - (frame_h - 2);
        if(item0 < 0)
            item0 = 0;

        for(u32 i = 0; i < frame_h - 2; i++) {
            if(item0 + i >= inventory->size)
                break;

            struct item_Data *data = &inventory->items[item0 + i];

            item_draw_icon(data, frame_x + 1, frame_y + 1 + i, false);
            item_write(data, 6, frame_x + 2, frame_y + 1 + i);
        }

        // draw cursor arrows
        if(chest_window == frame) {
            u32 cursor_y = frame_y + 1 + (selected[frame] - item0);
            screen_write(">", 6, frame_x, cursor_y);
            screen_write("<", 6, frame_x + frame_w - 1, cursor_y);
        }
    }
}

const struct Scene scene_chest = {
    .init = chest_init,

    .tick = chest_tick,
    .draw = chest_draw
};
