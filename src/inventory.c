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

bool inventory_add_resource(struct Inventory *inventory, u8 item_type) {
    // try to increase the count of an already present item
    for(u32 i = 0; i < inventory->size; i++) {
        struct item_Data *data = &inventory->items[i];

        if(data->type == item_type) {
            data->count++;

            return true;
        }
    }

    // append the item at the bottom
    if(inventory->size < INVENTORY_SIZE) {
        struct item_Data *data = &inventory->items[inventory->size];

        data->type = item_type;
        data->count = 1;

        inventory->size++;

        return true;
    }

    // inventory is full
    return false;
}

bool inventory_add_top(struct Inventory *inventory,
                       struct item_Data *data) {
    if(inventory->size < INVENTORY_SIZE) {
        // shift items down
        for(i32 i = inventory->size - 1; i >= 0; i--)
            inventory->items[i + 1] = inventory->items[i];

        inventory->size++;

        inventory->items[0] = *data;

        return true;
    }
    return false;
}

void inventory_remove(struct Inventory *inventory, u8 item_id,
                      struct item_Data *removed_item) {
    *removed_item = inventory->items[item_id];

    // shift items up
    for(u32 i = item_id; i < inventory->size - 1; i++)
        inventory->items[i] = inventory->items[i + 1];

    inventory->size--;
}
