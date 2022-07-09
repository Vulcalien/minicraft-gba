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
#include "crafting.h"

#include "inventory.h"
#include "item.h"
#include "furniture.h"

bool crafting_craft(struct Inventory *inventory,
                    struct crafting_Recipe *recipe) {
    const u8 result = recipe->result;

    // add item to the inventory
    {
        const struct Item *item = &item_list[result];
        if(item->class == ITEMCLASS_TOOL) {
            struct item_Data data = {
                .type = result,
                .tool_level = recipe->tool_level
            };

            if(!inventory_add(inventory, &data, true))
                return false;
        } else if(item->class == ITEMCLASS_FURNITURE) {
            struct item_Data data = { .type = result };

            if(result == CHEST_ITEM) {
                data.chest_id = furniture_new_chest_id();

                // TODO test if this works
                if(data.chest_id >= CHEST_LIMIT)
                    return false;
            }

            if(!inventory_add(inventory, &data, true))
                return false;
        } else {
            if(!inventory_add_resource(inventory, result, true))
                return false;
        }
    }

    // remove the required items
    // (assume that the inventory contains them)
    for(u32 i = 0; i < CRAFTING_MAX_REQUIRED; i++) {
        const u8 item_type = recipe->required.items[i];
        const u8 count     = recipe->required.count[i];

        if(count == 0)
            continue;

        inventory_remove_resource(inventory, item_type, count);
    }

    return true;
}
