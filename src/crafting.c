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

// TODO check all the values
const struct crafting_Recipe workbench_recipes[WORKBENCH_RECIPES] = {
    // Furniture

    // Lantern
    {
        .required.items = { WOOD_ITEM, SLIME_ITEM, GLASS_ITEM },
        .required.count = { 5,         10,         4          },

        .result = LANTERN_ITEM
    },

    // Oven
    {
        .required.items = { STONE_ITEM },
        .required.count = { 15 },

        .result = OVEN_ITEM
    },

    // Furnace
    {
        .required.items = { STONE_ITEM },
        .required.count = { 20 },

        .result = FURNACE_ITEM
    },

    // Workbench
    {
        .required.items = { WOOD_ITEM },
        .required.count = { 20 },

        .result = WORKBENCH_ITEM
    },

    // Chest
    {
        .required.items = { WOOD_ITEM },
        .required.count = { 20 },

        .result = CHEST_ITEM
    },

    // Anvil
    {
        .required.items = { IRON_INGOT_ITEM },
        .required.count = { 5 },

        .result = ANVIL_ITEM
    },

    // Wood Tools

    // Wood Sword
    {
        .required.items = { WOOD_ITEM },
        .required.count = { 5 },

        .result = SWORD_ITEM,
        .tool_level = 0
    },

    // Wood Axe
    {
        .required.items = { WOOD_ITEM },
        .required.count = { 5 },

        .result = AXE_ITEM,
        .tool_level = 0
    },

    // Wood Hoe
    {
        .required.items = { WOOD_ITEM },
        .required.count = { 5 },

        .result = HOE_ITEM,
        .tool_level = 0
    },

    // Wood Pick
    {
        .required.items = { WOOD_ITEM },
        .required.count = { 5 },

        .result = PICK_ITEM,
        .tool_level = 0
    },

    // Wood Shovel
    {
        .required.items = { WOOD_ITEM },
        .required.count = { 5 },

        .result = SHOVEL_ITEM,
        .tool_level = 0
    },

    // Stone Tools

    // Stone Sword
    {
        .required.items = { WOOD_ITEM, STONE_ITEM },
        .required.count = { 5,         5          },

        .result = SWORD_ITEM,
        .tool_level = 1
    },

    // Stone Axe
    {
        .required.items = { WOOD_ITEM, STONE_ITEM },
        .required.count = { 5,         5          },

        .result = AXE_ITEM,
        .tool_level = 1
    },

    // Stone Hoe
    {
        .required.items = { WOOD_ITEM, STONE_ITEM },
        .required.count = { 5,         5          },

        .result = HOE_ITEM,
        .tool_level = 1
    },

    // Stone Pick
    {
        .required.items = { WOOD_ITEM, STONE_ITEM },
        .required.count = { 5,         5          },

        .result = PICK_ITEM,
        .tool_level = 1
    },

    // Stone Shovel
    {
        .required.items = { WOOD_ITEM, STONE_ITEM },
        .required.count = { 5,         5          },

        .result = SHOVEL_ITEM,
        .tool_level = 1
    }
};

const struct crafting_Recipe furnace_recipes[FURNACE_RECIPES] = {
    // Iron Ingot
    {
        .required.items = { IRON_ORE_ITEM, COAL_ITEM },
        .required.count = { 4,             1         },

        .result = IRON_INGOT_ITEM
    },

    // Gold Ingot
    {
        .required.items = { GOLD_ORE_ITEM, COAL_ITEM },
        .required.count = { 4,             1         },

        .result = GOLD_INGOT_ITEM
    },

    // Glass
    {
        .required.items = { SAND_ITEM, COAL_ITEM },
        .required.count = { 4,         1         },

        .result = GLASS_ITEM
    }
};

const struct crafting_Recipe oven_recipes[OVEN_RECIPES] = {
    // Bread
    {
        .required.items = { WHEAT_ITEM },
        .required.count = { 4 },

        .result = BREAD_ITEM
    }
};

const struct crafting_Recipe anvil_recipes[ANVIL_RECIPES] = {
    // Iron Tools

    // Iron Sword
    {
        .required.items = { WOOD_ITEM, IRON_INGOT_ITEM },
        .required.count = { 5,         5               },

        .result = SWORD_ITEM,
        .tool_level = 2
    },

    // Iron Axe
    {
        .required.items = { WOOD_ITEM, IRON_INGOT_ITEM },
        .required.count = { 5,         5               },

        .result = AXE_ITEM,
        .tool_level = 2
    },

    // Iron Hoe
    {
        .required.items = { WOOD_ITEM, IRON_INGOT_ITEM },
        .required.count = { 5,         5               },

        .result = HOE_ITEM,
        .tool_level = 2
    },

    // Iron Pick
    {
        .required.items = { WOOD_ITEM, IRON_INGOT_ITEM },
        .required.count = { 5,         5               },

        .result = PICK_ITEM,
        .tool_level = 2
    },

    // Iron Shovel
    {
        .required.items = { WOOD_ITEM, IRON_INGOT_ITEM },
        .required.count = { 5,         5               },

        .result = SHOVEL_ITEM,
        .tool_level = 2
    },

    // Gold Tools

    // Gold Sword
    {
        .required.items = { WOOD_ITEM, GOLD_INGOT_ITEM },
        .required.count = { 5,         5               },

        .result = SWORD_ITEM,
        .tool_level = 3
    },

    // Gold Axe
    {
        .required.items = { WOOD_ITEM, GOLD_INGOT_ITEM },
        .required.count = { 5,         5               },

        .result = AXE_ITEM,
        .tool_level = 3
    },

    // Gold Hoe
    {
        .required.items = { WOOD_ITEM, GOLD_INGOT_ITEM },
        .required.count = { 5,         5               },

        .result = HOE_ITEM,
        .tool_level = 3
    },

    // Gold Pick
    {
        .required.items = { WOOD_ITEM, GOLD_INGOT_ITEM },
        .required.count = { 5,         5               },

        .result = PICK_ITEM,
        .tool_level = 3
    },

    // Gold Shovel
    {
        .required.items = { WOOD_ITEM, GOLD_INGOT_ITEM },
        .required.count = { 5,         5               },

        .result = SHOVEL_ITEM,
        .tool_level = 3
    },

    // Gem Tools

    // Gem Sword
    {
        .required.items = { WOOD_ITEM, GEM_ITEM },
        .required.count = { 5,         50       },

        .result = SWORD_ITEM,
        .tool_level = 4
    },

    // Gem Axe
    {
        .required.items = { WOOD_ITEM, GEM_ITEM },
        .required.count = { 5,         50       },

        .result = AXE_ITEM,
        .tool_level = 4
    },

    // Gem Hoe
    {
        .required.items = { WOOD_ITEM, GEM_ITEM },
        .required.count = { 5,         50       },

        .result = HOE_ITEM,
        .tool_level = 4
    },

    // Gem Pick
    {
        .required.items = { WOOD_ITEM, GEM_ITEM },
        .required.count = { 5,         50       },

        .result = PICK_ITEM,
        .tool_level = 4
    },

    // Gem Shovel
    {
        .required.items = { WOOD_ITEM, GEM_ITEM },
        .required.count = { 5,         50       },

        .result = SHOVEL_ITEM,
        .tool_level = 4
    }
};

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

            if(!inventory_add(inventory, &data, 0))
                return false;
        } else if(item->class == ITEMCLASS_FURNITURE) {
            struct item_Data data = { .type = result };

            if(result == CHEST_ITEM) {
                data.chest_id = furniture_new_chest_id();

                // TODO test if this works
                if(data.chest_id >= CHEST_LIMIT)
                    return false;
            }

            if(!inventory_add(inventory, &data, 0))
                return false;
        } else {
            if(!inventory_add_resource(inventory, result, 1, 0))
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
