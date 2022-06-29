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

const struct Item item_list[ITEM_TYPES] = {
    // Wood
    {
        .class = ITEMCLASS_MATERIAL,
        .name = "WOOD"
    },

    // Stone
    {
        .class = ITEMCLASS_MATERIAL,
        .name = "STONE"
    },

    // Glass
    {
        .class = ITEMCLASS_MATERIAL,
        .name = "GLASS"
    },

    // Wheat
    {
        .class = ITEMCLASS_MATERIAL,
        .name = "WHEAT"
    },

    // Slime
    {
        .class = ITEMCLASS_MATERIAL,
        .name = "SLIME"
    },

    // Cloth
    {
        .class = ITEMCLASS_MATERIAL,
        .name = "CLOTH"
    },

    // Coal
    {
        .class = ITEMCLASS_MATERIAL,
        .name = "COAL"
    },

    // Iron Ore
    {
        .class = ITEMCLASS_MATERIAL,
        .name = "I.ORE"
    },

    // Gold Ore
    {
        .class = ITEMCLASS_MATERIAL,
        .name = "G.ORE"
    },

    // Iron Ingot
    {
        .class = ITEMCLASS_MATERIAL,
        .name = "IRON"
    },

    // Gold Ingot
    {
        .class = ITEMCLASS_MATERIAL,
        .name = "GOLD"
    },

    // Gem
    {
        .class = ITEMCLASS_MATERIAL,
        .name = "GEM"
    },

    // -----

    // Flower
    {
        .class = ITEMCLASS_PLACEABLE,
        .name = "FLOWER",

        .placed_tile = FLOWER_TILE,
        .placeable_on = { GRASS_TILE, -1, -1 }
    },

    // Seeds
    {
        .class = ITEMCLASS_PLACEABLE,
        .name = "SEEDS",

        .placed_tile = WHEAT_TILE,
        .placeable_on = { FARMLAND_TILE, -1, -1 }
    },

    // Acorn
    {
        .class = ITEMCLASS_PLACEABLE,
        .name = "ACORN",

        .placed_tile = TREE_SAPLING_TILE,
        .placeable_on = { GRASS_TILE, -1, -1 }
    },

    // Cactus
    {
        .class = ITEMCLASS_PLACEABLE,
        .name = "CACTUS",

        .placed_tile = CACTUS_SAPLING_TILE,
        .placeable_on = { SAND_TILE, -1, -1 }
    },

    // Dirt
    {
        .class = ITEMCLASS_PLACEABLE,
        .name = "DIRT",

        .placed_tile = DIRT_TILE,
        .placeable_on = { HOLE_TILE, WATER_TILE, LAVA_TILE }
    },

    // Sand
    {
        .class = ITEMCLASS_PLACEABLE,
        .name = "SAND",

        .placed_tile = SAND_TILE,
        .placeable_on = { GRASS_TILE, DIRT_TILE, -1 }
    },

    // Cloud
    {
        .class = ITEMCLASS_PLACEABLE,
        .name = "CLOUD",

        .placed_tile = CLOUD_TILE,
        .placeable_on = { INFINITE_FALL_TILE, -1, -1 }
    },

    // -----

    // Apple
    {
        .class = ITEMCLASS_FOOD,
        .name = "APPLE",

        .hp_gain = 1
    },

    // Bread
    {
        .class = ITEMCLASS_FOOD,
        .name = "Bread",

        .hp_gain = 2
    },

    // -----

    // Sword
    {
        .class = ITEMCLASS_TOOL,
        .name = "SWRD"
    },

    // Axe
    {
        .class = ITEMCLASS_TOOL,
        .name = "AXE"
    },

    // Pick
    {
        .class = ITEMCLASS_TOOL,
        .name = "PICK"
    },

    // Shovel
    {
        .class = ITEMCLASS_TOOL,
        .name = "SHVL"
    },

    // Hoe
    {
        .class = ITEMCLASS_TOOL,
        .name = "HOE"
    },

    // -----

    // Workbench
    {
        .class = ITEMCLASS_FURNITURE,
        .name = "WORKBENCH"
    },

    // Furnace
    {
        .class = ITEMCLASS_FURNITURE,
        .name = "FURNACE"
    },

    // Oven
    {
        .class = ITEMCLASS_FURNITURE,
        .name = "OVEN"
    },

    // Anvil
    {
        .class = ITEMCLASS_FURNITURE,
        .name = "ANVIL"
    },

    // Chest
    {
        .class = ITEMCLASS_FURNITURE,
        .name = "CHEST"
    },

    // Lantern
    {
        .class = ITEMCLASS_FURNITURE,
        .name = "LANTERN"
    },

    // -----

    // Power Glove
    {
        .class = ITEMCLASS_FURNITURE,
        .name = "POW GLOVE"
    }
};
