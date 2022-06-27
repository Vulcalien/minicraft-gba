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
#ifndef MINICRAFT_ITEM
#define MINICRAFT_ITEM

#include "minicraft.h"

#define ITEM_TYPES (100) // DEBUG 100

#define WOOD_ITEM       (0)
#define STONE_ITEM      (1)
#define GLASS_ITEM      (2)
#define WHEAT_ITEM      (3)
#define SLIME_ITEM      (4)
#define CLOTH_ITEM      (5)
#define COAL_ITEM       (6)
#define IRON_ORE_ITEM   (7)
#define GOLD_ORE_ITEM   (8)
#define IRON_INGOT_ITEM (9)
#define GOLD_INGOT_ITEM (10)
#define GEM_ITEM        (11)

#define FLOWER_ITEM (12)
#define SEEDS_ITEM  (13)
#define ACORN_ITEM  (14)
#define CACTUS_ITEM (15)
#define DIRT_ITEM   (16)
#define SAND_ITEM   (17)
#define CLOUD_ITEM  (18)

#define APPLE_ITEM (19)
#define BREAD_ITEM (20)

#define SWORD_ITEM  (21)
#define AXE_ITEM    (22)
#define PICK_ITEM   (23)
#define SHOVEL_ITEM (24)
#define HOE_ITEM    (25)

// TODO Furniture and Power Glove

#define ITEMCLASS_MATERIAL   (0)
#define ITEMCLASS_PLACEABLE  (1)
#define ITEMCLASS_FOOD       (2)
#define ITEMCLASS_TOOL       (3)
#define ITEMCLASS_FURNITURE  (4)
#define ITEMCLASS_POWERGLOVE (5)

struct Item {
    u8 class;
    char *name;

    union {
        // ITEMCLASS_PLACEABLE
        struct {
            u8 placed_tile;
            u8 placeable_on[3];
        };

        // ITEMCLASS_FOOD
        struct {
            u8 hp_gain;
        };

        // ITEMCLASS_FURNITURE
        struct {
            u8 furniture;
        };
    };
};

struct item_Data {
    u8 type;

    union {
        u8 count;
        u8 tool_level;
        u8 chest_id;
    };
};

extern const struct Item item_list[ITEM_TYPES];

#endif // MINICRAFT_ITEM
