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
#include "tile.h"

#include "level.h"
#include "entity.h"
#include "player.h"
#include "item.h"

#define FSTEPPED_ON(name)\
    IWRAM_SECTION\
    static void name(struct Level *level, u32 xt, u32 yt,\
                     struct entity_Data *entity_data)

#define FINTERACT(name)\
    static void name(struct Level *level, u32 xt, u32 yt,\
                     struct item_Data *item)

#define GET_PUNCH_DAMAGE() (1 + rand() % 3)

// Grass
FINTERACT(grass_interact) {
    if(item->type == SHOVEL_ITEM) {
        if(player_pay_stamina(4 - item->tool_level)) {
            if(rand() % 5 == 0)
                entity_add_item(level, xt, yt, SEEDS_ITEM, true);

            LEVEL_SET_TILE(level, xt, yt, DIRT_TILE, 0);

            // TODO play sound
        }
    } else if(item->type == HOE_ITEM) {
        if(player_pay_stamina(4 - item->tool_level)) {
            LEVEL_SET_TILE(level, xt, yt, FARMLAND_TILE, 0);

            if(rand() % 5 == 0)
                entity_add_item(level, xt, yt, SEEDS_ITEM, true);

            // TODO play sound
        }
    }
}

// Rock
FINTERACT(rock_interact) {
    u8 dmg;
    if(item->type == PICK_ITEM && player_pay_stamina(4 - item->tool_level))
        dmg = 10 + item->tool_level * 5 + rand() % 10;
    else
        dmg = GET_PUNCH_DAMAGE();

    u8 damage = LEVEL_GET_DATA(level, xt, yt) + dmg;
    if(damage >= 50) {
        u8 count = 1 + rand() % 4;
        for(u32 i = 0; i < count; i++)
            entity_add_item(level, xt, yt, STONE_ITEM, true);

        if(rand() & 1)
            entity_add_item(level, xt, yt, COAL_ITEM, true);

        LEVEL_SET_TILE(level, xt, yt, DIRT_TILE, 0);
    } else {
        LEVEL_SET_DATA(level, xt, yt, damage);
    }

    entity_add_smash_particle(level, xt, yt);
    entity_add_text_particle(level, (xt << 4) + 8, (yt << 4) + 8, dmg, 0);
}

// Flower
FINTERACT(flower_interact) {
    u8 count;
    if(item->type == SHOVEL_ITEM && player_pay_stamina(4 - item->tool_level))
        count = 2;
    else
        count = 1 + rand() % 2;

    for(u32 i = 0; i < count; i++)
        entity_add_item(level, xt, yt, FLOWER_ITEM, true);

    LEVEL_SET_TILE(level, xt, yt, GRASS_TILE, 0);
}

// Tree
FINTERACT(tree_interact) {
    u8 dmg;
    if(item->type == AXE_ITEM && player_pay_stamina(4 - item->tool_level))
        dmg = 10 + item->tool_level * 5 + rand() % 10;
    else
        dmg = GET_PUNCH_DAMAGE();

    if(rand() % 10 == 0)
        entity_add_item(level, xt, yt, APPLE_ITEM, true);

    u8 damage = LEVEL_GET_DATA(level, xt, yt) + dmg;
    if(damage >= 20) {
        u8 count = 1 + rand() % 2;
        for(u32 i = 0; i < count; i++)
            entity_add_item(level, xt, yt, WOOD_ITEM, true);

        count = rand() % 4;
        for(u32 i = 0; i < count; i++)
            entity_add_item(level, xt, yt, ACORN_ITEM, true);

        LEVEL_SET_TILE(level, xt, yt, GRASS_TILE, 0);
    } else {
        LEVEL_SET_DATA(level, xt, yt, damage);
    }

    entity_add_smash_particle(level, xt, yt);
    entity_add_text_particle(level, (xt << 4) + 8, (yt << 4) + 8, dmg, 0);
}

// Dirt
FINTERACT(dirt_interact) {
    if(item->type == SHOVEL_ITEM) {
        if(player_pay_stamina(4 - item->tool_level)) {
            entity_add_item(level, xt, yt, DIRT_ITEM, true);

            LEVEL_SET_TILE(level, xt, yt, HOLE_TILE, 0);

            // TODO play sound
        }
    } else if(item->type == HOE_ITEM) {
        if(player_pay_stamina(4 - item->tool_level)) {
            LEVEL_SET_TILE(level, xt, yt, FARMLAND_TILE, 0);

            // TODO play sound
        }
    }
}

// Sand
FSTEPPED_ON(sand_stepped_on) {
    u8 etype = entity_data->type;
    if(etype == ZOMBIE_ENTITY ||
       etype == SLIME_ENTITY ||
       etype == PLAYER_ENTITY) {
        LEVEL_SET_DATA(level, xt, yt, 10);
    }
}

FINTERACT(sand_interact) {
    if(item->type == SHOVEL_ITEM && player_pay_stamina(4 - item->tool_level)) {
        entity_add_item(level, xt, yt, SAND_ITEM, true);

        LEVEL_SET_TILE(level, xt, yt, DIRT_TILE, 0);

        // no sound after digging sand
        // might be a bug in the original game
    }
}

// Cactus
FINTERACT(cactus_interact) {
    u8 dmg = GET_PUNCH_DAMAGE();

    u8 damage = LEVEL_GET_DATA(level, xt, yt) + dmg;
    if(damage >= 10) {
        u8 count = 1 + rand() % 2;
        for(u32 i = 0; i < count; i++)
            entity_add_item(level, xt, yt, CACTUS_ITEM, true);

        LEVEL_SET_TILE(level, xt, yt, SAND_TILE, 0);
    } else {
        LEVEL_SET_DATA(level, xt, yt, damage);
    }

    entity_add_smash_particle(level, xt, yt);
    entity_add_text_particle(level, (xt << 4) + 8, (yt << 4) + 8, dmg, 0);
}

// Tree Sapling
FINTERACT(tree_sapling_interact) {
    LEVEL_SET_TILE(level, xt, yt, GRASS_TILE, 0);
}

// Cactus Sapling
FINTERACT(cactus_sapling_interact) {
    LEVEL_SET_TILE(level, xt, yt, SAND_TILE, 0);
}

// Farmland
FSTEPPED_ON(farmland_stepped_on) {
    if(rand() % 60 != 0)
        return;

    if(LEVEL_GET_DATA(level, xt, yt) >= 5)
        LEVEL_SET_TILE(level, xt, yt, DIRT_TILE, 0);
}

FINTERACT(farmland_interact) {
    if(item->type == SHOVEL_ITEM && player_pay_stamina(4 - item->tool_level))
        LEVEL_SET_TILE(level, xt, yt, DIRT_TILE, 0);
}

// Wheat
static inline void wheat_harvest(struct Level *level, u32 xt, u32 yt) {
    if(rand() & 1)
        entity_add_item(level, xt, yt, SEEDS_ITEM, true);

    u8 age = LEVEL_GET_DATA(level, xt, yt);
    if(age >= 40) {
        u8 count;
        if(age == 50)
            count = 2 + rand() % 3;
        else
            count = 1 + rand() % 2;

        for(u32 i = 0; i < count; i++)
            entity_add_item(level, xt, yt, WHEAT_ITEM, true);
    }

    LEVEL_SET_TILE(level, xt, yt, DIRT_TILE, 0);
}

FSTEPPED_ON(wheat_stepped_on) {
    if(rand() % 60 != 0)
        return;

    if(LEVEL_GET_DATA(level, xt, yt) >= 2)
        wheat_harvest(level, xt, yt);
}

FINTERACT(wheat_interact) {
    if(item->type == SHOVEL_ITEM && player_pay_stamina(4 - item->tool_level))
        LEVEL_SET_TILE(level, xt, yt, DIRT_TILE, 0);
    else
        wheat_harvest(level, xt, yt);
}

// Cloud
FINTERACT(cloud_interact) {
    if(item->type == SHOVEL_ITEM && player_pay_stamina(5)) {
        u8 count = 1 + rand() % 2;
        for(u32 i = 0; i < count; i++)
            entity_add_item(level, xt, yt, CLOUD_ITEM, true);
    }
}

// Hard Rock
FINTERACT(hard_rock_interact) {
    u8 dmg = 0;
    if(item->type == PICK_ITEM && item->tool_level == 4) {
        dmg = 30 + rand() % 10;

        u8 damage = LEVEL_GET_DATA(level, xt, yt) + dmg;
        if(damage >= 200) {
            u8 count = 1 + rand() % 4;
            for(u32 i = 0; i < count; i++)
                entity_add_item(level, xt, yt, STONE_ITEM, true);

            if(rand() & 1)
                entity_add_item(level, xt, yt, COAL_ITEM, true);

            LEVEL_SET_TILE(level, xt, yt, DIRT_TILE, 0);
        } else {
            LEVEL_SET_DATA(level, xt, yt, damage);
        }
    }

    entity_add_smash_particle(level, xt, yt);
    entity_add_text_particle(level, (xt << 4) + 8, (yt << 4) + 8, dmg, 0);
}

// Ores
FINTERACT(ore_interact) {
    u8 dmg = 0;
    if(item->type == PICK_ITEM && player_pay_stamina(6 - item->tool_level)) {
        dmg = 1;

        u8 count = rand() % 2;
        u8 tile = LEVEL_GET_TILE(level, xt, yt);
        u8 item_to_drop = ((tile == IRON_ORE_TILE) * IRON_ORE_ITEM) |
                          ((tile == GOLD_ORE_TILE) * GOLD_ORE_ITEM) |
                          ((tile == GEM_ORE_TILE)  * GEM_ITEM);

        u8 damage = LEVEL_GET_DATA(level, xt, yt) + dmg;
        if(damage >= 3 + rand() % 10) {
            LEVEL_SET_TILE(level, xt, yt, DIRT_TILE, 0);

            count += 2;
        } else {
            LEVEL_SET_DATA(level, xt, yt, damage);
        }

        for(u32 i = 0; i < count; i++)
            entity_add_item(level, xt, yt, item_to_drop, true);
    }

    entity_add_smash_particle(level, xt, yt);
    entity_add_text_particle(level, (xt << 4) + 8, (yt << 4) + 8, dmg, 0);
}

// Cloud Cactus
FINTERACT(cloud_cactus_interact) {
    u8 dmg = 0;
    if(item->type == PICK_ITEM && player_pay_stamina(6 - item->tool_level)) {
        dmg = 1;

        u8 damage = LEVEL_GET_DATA(level, xt, yt) + dmg;
        if(damage >= 10)
            LEVEL_SET_TILE(level, xt, yt, CLOUD_TILE, 0);
        else
            LEVEL_SET_DATA(level, xt, yt, damage);
    }

    entity_add_smash_particle(level, xt, yt);
    entity_add_text_particle(level, (xt << 4) + 8, (yt << 4) + 8, dmg, 0);
}

IWRAM_RODATA_SECTION
const struct Tile tile_list[TILE_TYPES] = {
    // Grass
    {
        .connects_to = {
            .grass = true
        },

        .interact = grass_interact
    },

    // Rock
    {
        .is_solid = true,
        .may_pass = -1,

        .interact = rock_interact
    },

    // Liquid
    {
        .connects_to = {
            .sand   = true,
            .liquid = true
        },

        .is_solid = true,
        .may_pass = PLAYER_ENTITY
    },

    // Flower
    {
        .connects_to = {
            .grass = true
        },

        .interact = flower_interact
    },

    // Tree
    {
        .connects_to = {
            .grass = true
        },

        .is_solid = true,
        .may_pass = -1,

        .interact = tree_interact
    },

    // Dirt
    {
        .interact = dirt_interact
    },

    // Sand
    {
        .connects_to = {
            .sand = true
        },

        .stepped_on = sand_stepped_on,

        .interact = sand_interact
    },

    // Cactus
    {
        .connects_to = {
            .sand = true
        },

        .is_solid = true,
        .may_pass = -1,

        .touch_damage = 1,

        .interact = cactus_interact
    },

    // Hole
    {
        .connects_to = {
            .sand   = true,
            .liquid = true,
        },

        .is_solid = true,
        .may_pass = PLAYER_ENTITY
    },

    // Tree Sapling
    {
        .connects_to = {
            .grass = true
        },

        .interact = tree_sapling_interact
    },

    // Cactus Sapling
    {
        .connects_to = {
            .sand = true
        },

        .interact = cactus_sapling_interact
    },

    // Farmland
    {
        .stepped_on = farmland_stepped_on,

        .interact = farmland_interact
    },

    // Wheat
    {
        .stepped_on = wheat_stepped_on,

        .interact = wheat_interact
    },

    // Stairs Down
    { 0 },

    // Stairs Up
    { 0 },

    // Infinite Fall
    {
        .is_solid = true,
        .may_pass = AIR_WIZARD_ENTITY
    },

    // Cloud
    {
        .interact = cloud_interact
    },

    // Hard Rock
    {
        .is_solid = true,
        .may_pass = -1,

        .interact = hard_rock_interact
    },

    // Iron Ore
    {
        .is_solid = true,
        .may_pass = -1,

        .touch_damage = 3,

        .interact = ore_interact
    },

    // Gold Ore
    {
        .is_solid = true,
        .may_pass = -1,

        .touch_damage = 3,

        .interact = ore_interact
    },

    // Gem Ore
    {
        .is_solid = true,
        .may_pass = -1,

        .touch_damage = 3,

        .interact = ore_interact
    },

    // Cloud Cactus
    {
        .is_solid = true,
        .may_pass = AIR_WIZARD_ENTITY,

        .touch_damage = 3,

        .interact = cloud_cactus_interact
    }
};
