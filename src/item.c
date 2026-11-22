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
#include "item.h"

#include "tile.h"
#include "screen.h"

const struct Item item_list[ITEM_TYPES] = {
    // Wood
    {
        .class = ITEMCLASS_MATERIAL,
        .name = TEXT_ITEM_WOOD,
        .palette = 0
    },

    // Stone
    {
        .class = ITEMCLASS_MATERIAL,
        .name = TEXT_ITEM_STONE,
        .palette = 1
    },

    // Glass
    {
        .class = ITEMCLASS_MATERIAL,
        .name = TEXT_ITEM_GLASS,
        .palette = 1
    },

    // Wheat
    {
        .class = ITEMCLASS_MATERIAL,
        .name = TEXT_ITEM_WHEAT,
        .palette = 2
    },

    // Slime
    {
        .class = ITEMCLASS_MATERIAL,
        .name = TEXT_ITEM_SLIME,
        .palette = 1
    },

    // Cloth
    {
        .class = ITEMCLASS_MATERIAL,
        .name = TEXT_ITEM_CLOTH,
        .palette = 2
    },

    // Coal
    {
        .class = ITEMCLASS_MATERIAL,
        .name = TEXT_ITEM_COAL,
        .palette = 1
    },

    // Iron Ore
    {
        .class = ITEMCLASS_MATERIAL,
        .name = TEXT_ITEM_IRON_ORE,
        .palette = 0
    },

    // Gold Ore
    {
        .class = ITEMCLASS_MATERIAL,
        .name = TEXT_ITEM_GOLD_ORE,
        .palette = 2
    },

    // Iron Ingot
    {
        .class = ITEMCLASS_MATERIAL,
        .name = TEXT_ITEM_IRON,
        .palette = 0
    },

    // Gold Ingot
    {
        .class = ITEMCLASS_MATERIAL,
        .name = TEXT_ITEM_GOLD,
        .palette = 2
    },

    // Gem
    {
        .class = ITEMCLASS_MATERIAL,
        .name = TEXT_ITEM_GEM,
        .palette = 2
    },

    // -----

    // Flower
    {
        .class = ITEMCLASS_PLACEABLE,
        .name = TEXT_ITEM_FLOWER,
        .palette = 1,

        .placed_tile = FLOWER_TILE,
        .placeable_on = { GRASS_TILE, -1 }
    },

    // Seeds
    {
        .class = ITEMCLASS_PLACEABLE,
        .name = TEXT_ITEM_SEEDS,
        .palette = 1,

        .placed_tile = WHEAT_TILE,
        .placeable_on = { FARMLAND_TILE, -1 }
    },

    // Acorn
    {
        .class = ITEMCLASS_PLACEABLE,
        .name = TEXT_ITEM_ACORN,
        .palette = 0,

        .placed_tile = TREE_SAPLING_TILE,
        .placeable_on = { GRASS_TILE, -1 }
    },

    // Cactus
    {
        .class = ITEMCLASS_PLACEABLE,
        .name = TEXT_ITEM_CACTUS,
        .palette = 1,

        .placed_tile = CACTUS_SAPLING_TILE,
        .placeable_on = { SAND_TILE, -1 }
    },

    // Dirt
    {
        .class = ITEMCLASS_PLACEABLE,
        .name = TEXT_ITEM_DIRT,
        .palette = 0,

        .placed_tile = DIRT_TILE,
        .placeable_on = { HOLE_TILE, LIQUID_TILE }
    },

    // Sand
    {
        .class = ITEMCLASS_PLACEABLE,
        .name = TEXT_ITEM_SAND,
        .palette = 2,

        .placed_tile = SAND_TILE,
        .placeable_on = { GRASS_TILE, DIRT_TILE }
    },

    // Cloud
    {
        .class = ITEMCLASS_PLACEABLE,
        .name = TEXT_ITEM_CLOUD,
        .palette = 1,

        .placed_tile = CLOUD_TILE,
        .placeable_on = { INFINITE_FALL_TILE, -1 }
    },

    // -----

    // Apple
    {
        .class = ITEMCLASS_FOOD,
        .name = TEXT_ITEM_APPLE,
        .palette = 0,

        .hp_gain = 1
    },

    // Bread
    {
        .class = ITEMCLASS_FOOD,
        .name = TEXT_ITEM_BREAD,
        .palette = 2,

        .hp_gain = 2
    },

    // -----

    // Workbench
    {
        .class = ITEMCLASS_FURNITURE,
        .name = TEXT_ITEM_WORKBENCH,
        .palette = 3
    },

    // Furnace
    {
        .class = ITEMCLASS_FURNITURE,
        .name = TEXT_ITEM_FURNACE,
        .palette = 1
    },

    // Oven
    {
        .class = ITEMCLASS_FURNITURE,
        .name = TEXT_ITEM_OVEN,
        .palette = 1
    },

    // Anvil
    {
        .class = ITEMCLASS_FURNITURE,
        .name = TEXT_ITEM_ANVIL,
        .palette = 1
    },

    // Chest
    {
        .class = ITEMCLASS_FURNITURE,
        .name = TEXT_ITEM_CHEST,
        .palette = 2
    },

    // Lantern
    {
        .class = ITEMCLASS_FURNITURE,
        .name = TEXT_ITEM_LANTERN,
        .palette = 1
    },

    // -----

    // Power Glove
    {
        .class = ITEMCLASS_POWERGLOVE,
        .name = TEXT_ITEM_POWER_GLOVE,
        .palette = 0
    },

    // -----

    // Sword
    {
        .class = ITEMCLASS_TOOL,
        .name = TEXT_ITEM_SWORD,
        .palette = 3
    },

    // Axe
    {
        .class = ITEMCLASS_TOOL,
        .name = TEXT_ITEM_AXE,
        .palette = 3
    },

    // Pick
    {
        .class = ITEMCLASS_TOOL,
        .name = TEXT_ITEM_PICK,
        .palette = 3
    },

    // Shovel
    {
        .class = ITEMCLASS_TOOL,
        .name = TEXT_ITEM_SHOVEL,
        .palette = 3
    },

    // Hoe
    {
        .class = ITEMCLASS_TOOL,
        .name = TEXT_ITEM_HOE,
        .palette = 3
    }
};

THUMB
void item_write(struct item_Data *data, u8 palette, u32 x, u32 y) {
    const struct Item *item = ITEM_S(data);

    if(item_is_resource(data->type)) {
        u16 count = data->count;
        if(count > 999)
            count = 999;

        SCREEN_WRITE_NUMBER(count, 10, 3, false, palette + 3, x, y);
        screen_write(text(item->name), palette, x + 3, y);
    } else {
        item_write_name(data, palette, x, y);
    }
}

static const enum TextID level_names[5] = {
    TEXT_TOOL_WOOD, TEXT_TOOL_ROCK, TEXT_TOOL_IRON, TEXT_TOOL_GOLD,
    TEXT_TOOL_GEM
};

THUMB
void item_write_name(struct item_Data *data, u8 palette, u32 x, u32 y) {
    const struct Item *item = ITEM_S(data);

    if(item->class == ITEMCLASS_TOOL) {
        const u8 level = data->tool_level;

        screen_write(text(level_names[level]), palette, x, y);
        screen_write(text(item->name), palette, x + 4 + (level != 4), y);
    } else {
        screen_write(text(item->name), palette, x, y);
    }
}

THUMB
void item_draw_icon(struct item_Data *data, u32 x, u32 y, bool black_bg) {
    const struct Item *item = ITEM_S(data);

    const u16 tile = 128 + data->type +
                     (item->class == ITEMCLASS_TOOL) * (2 + data->tool_level * 5);
    const u8 palette = (black_bg == false) * (12 + item->palette) +
                       (black_bg == true) * 11;

    BG3_TILEMAP[x + y * 32] = tile | palette << 12;
}
