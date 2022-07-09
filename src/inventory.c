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
#include "inventory.h"

bool inventory_add(struct Inventory *inventory,
                   struct item_Data *data, bool top) {
    if(inventory->size >= INVENTORY_SIZE)
        return false;

    if(top) {
        // shift items down
        for(i32 i = inventory->size - 1; i >= 0; i--)
            inventory->items[i + 1] = inventory->items[i];

        inventory->items[0] = *data;
    } else {
        inventory->items[inventory->size] = *data;
    }

    inventory->size++;
    return true;
}

bool inventory_add_resource(struct Inventory *inventory,
                            u8 item_type, bool top) {
    // try to increase the count of an already present item
    for(u32 i = 0; i < inventory->size; i++) {
        struct item_Data *data = &inventory->items[i];

        if(data->type == item_type) {
            data->count++;

            return true;
        }
    }

    // the resource was not present: add a new item
    struct item_Data data = { .type  = item_type, .count = 1 };
    return inventory_add(inventory, &data, top);
}

void inventory_remove(struct Inventory *inventory,
                      u8 slot, struct item_Data *removed_item) {
    *removed_item = inventory->items[slot];

    // shift items up
    for(u32 i = slot; i < inventory->size - 1; i++)
        inventory->items[i] = inventory->items[i + 1];

    inventory->size--;
}

void inventory_remove_resource(struct Inventory *inventory,
                               u8 item_type, u8 count) {
    for(u32 i = 0; i < inventory->size; i++) {
        struct item_Data *data = &inventory->items[i];

        if(data->type == item_type && data->count >= count) {
            data->count -= count;
            if(data->count == 0) {
                struct item_Data removed;
                inventory_remove(inventory, i, &removed);
            }

            break;
        }
    }
}
