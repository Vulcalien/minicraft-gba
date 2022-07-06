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

#define TILE(id, palette) ((id) | ((palette) << 12))
#define TILE_M(id, flip, palette)\
    ((id) | (flip << 10) | (palette << 12))

#define FTICK(name)\
    IWRAM_SECTION\
    static void name(struct Level *level, u32 xt, u32 yt)

#define FDRAW(name)\
    IWRAM_SECTION\
    static void name(struct Level *level, u32 xt, u32 yt,\
                     u16 tiles[4], u16 tiles2[4])

#define FSTEPPED_ON(name)\
    IWRAM_SECTION\
    static void name(struct Level *level, u32 xt, u32 yt,\
                     struct entity_Data *entity_data)

#define FINTERACT(name)\
    IWRAM_SECTION\
    static void name(struct Level *level, u32 xt, u32 yt,\
                     struct item_Data *item)

// CONNECTS_TO_X
#define CONNECTS_TO_GRASS(level, xt, yt)\
    (LEVEL_GET_TILE_S((level), (xt), (yt))->connects_to.grass)
#define CONNECTS_TO_SAND(level, xt, yt)\
    (LEVEL_GET_TILE_S((level), (xt), (yt))->connects_to.sand)
#define CONNECTS_TO_LIQUID(level, xt, yt)\
    (LEVEL_GET_TILE_S((level), (xt), (yt))->connects_to.liquid)

#define GET_PUNCH_DAMAGE() (1 + rand() % 3)

// DAMAGE RECOVER
FTICK(damage_recover_tick) {
    u8 damage = LEVEL_GET_DATA(level, xt, yt);
    if(damage != 0)
        LEVEL_SET_DATA(level, xt, yt, damage - 1);
}

// Grass
FTICK(grass_tick) {
    if(rand() % 40 != 0)
        return;

    i32 xn = xt;
    i32 yn = yt;

    if(rand() & 1)
        xn += (rand() & 1) * 2 - 1;
    else
        yn += (rand() & 1) * 2 - 1;

    if(LEVEL_GET_TILE(level, xn, yn) == DIRT_TILE)
        LEVEL_SET_TILE(level, xn, yn, GRASS_TILE, 0);
}

FDRAW(grass_draw) {
    bool u = CONNECTS_TO_GRASS(level, xt,     yt - 1);
    bool d = CONNECTS_TO_GRASS(level, xt,     yt + 1);
    bool l = CONNECTS_TO_GRASS(level, xt - 1, yt    );
    bool r = CONNECTS_TO_GRASS(level, xt + 1, yt    );

    if(u && l)
        tiles[0] = TILE(0, 0);
    else
        tiles[0] = TILE(4 + u * 7 + l * 4, 0);

    if(u && r)
        tiles[1] = TILE(1, 0);
    else
        tiles[1] = TILE(5 + u * 4 + r * 3, 0);

    if(d && l)
        tiles[2] = TILE(2, 0);
    else
        tiles[2] = TILE(6 + d * 5 + l * 4, 0);

    if(d && r)
        tiles[3] = TILE(3, 0);
    else
        tiles[3] = TILE(7 + d * 2 + r * 3, 0);
}

FINTERACT(grass_interact) {
    if(item->type == SHOVEL_ITEM) {
        if(player_pay_stamina(4 - item->tool_level)) {
            LEVEL_SET_TILE(level, xt, yt, DIRT_TILE, 0);

            if(rand() % 5 == 0) {
                // TODO add seeds
            }

            // TODO play sound
        }
    } else if(item->type == HOE_ITEM) {
        if(player_pay_stamina(4 - item->tool_level)) {
            LEVEL_SET_TILE(level, xt, yt, FARMLAND_TILE, 0);

            // TODO play sound
        }
    }
}

// Rock
FDRAW(rock_draw) {
    bool u = LEVEL_GET_TILE(level, xt,     yt - 1) == ROCK_TILE;
    bool d = LEVEL_GET_TILE(level, xt,     yt + 1) == ROCK_TILE;
    bool l = LEVEL_GET_TILE(level, xt - 1, yt    ) == ROCK_TILE;
    bool r = LEVEL_GET_TILE(level, xt + 1, yt    ) == ROCK_TILE;

    bool ul = LEVEL_GET_TILE(level, xt - 1, yt - 1) == ROCK_TILE;
    bool dl = LEVEL_GET_TILE(level, xt - 1, yt + 1) == ROCK_TILE;
    bool ur = LEVEL_GET_TILE(level, xt + 1, yt - 1) == ROCK_TILE;
    bool dr = LEVEL_GET_TILE(level, xt + 1, yt + 1) == ROCK_TILE;

    if(u && l)
        tiles[0] = TILE(0 + !ul * 20, 1);
    else
        tiles[0] = TILE(12 + u * 7 + l * 4, 1);

    if(u && r)
        tiles[1] = TILE(1 + !ur * 20, 1);
    else
        tiles[1] = TILE(13 + u * 4 + r * 3, 1);

    if(d && l)
        tiles[2] = TILE(2 + !dl * 20, 1);
    else
        tiles[2] = TILE(14 + d * 5 + l * 4, 1);

    if(d && r)
        tiles[3] = TILE(3 + !dr * 20, 1);
    else
        tiles[3] = TILE(15 + d * 2 + r * 3, 1);
}

FINTERACT(rock_interact) {
    u8 dmg;
    if(item->type == PICK_ITEM && player_pay_stamina(4 - item->tool_level))
        dmg = 10 + item->tool_level * 5 + rand() % 10;
    else
        dmg = GET_PUNCH_DAMAGE();

    u8 damage = LEVEL_GET_DATA(level, xt, yt) + dmg;

    if(damage >= 50) {
        u8 count = 1 + rand() % 4;
        for(u32 i = 0; i < count; i++) {
            // TODO drop stone
        }

        count = rand() % 2;
        for(u32 i = 0; i < count; i++) {
            // TODO drop coal
        }

        LEVEL_SET_TILE(level, xt, yt, DIRT_TILE, 0);
    } else {
        LEVEL_SET_DATA(level, xt, yt, damage);
    }

    // TODO add smash particle
    // TODO add text particle
}

// Water
FTICK(water_tick) {
    i32 xn = xt;
    i32 yn = yt;

    if(rand() & 1)
        xn += (rand() & 1) * 2 - 1;
    else
        yn += (rand() & 1) * 2 - 1;

    if(LEVEL_GET_TILE(level, xn, yn) == HOLE_TILE)
        LEVEL_SET_TILE(level, xn, yn, WATER_TILE, 0);
}

FDRAW(water_draw) {
    u32 water_seed = (tick_count + 0x109f77 * xt - 0xab24af3 * yt) / 10 * 0x248f7b13 + 0xc21840c5;
    u16 water_rand = (water_seed * 0x248f7b13 + 0xc21840c5) >> 16;

    bool u = CONNECTS_TO_LIQUID(level, xt,     yt - 1);
    bool d = CONNECTS_TO_LIQUID(level, xt,     yt + 1);
    bool l = CONNECTS_TO_LIQUID(level, xt - 1, yt    );
    bool r = CONNECTS_TO_LIQUID(level, xt + 1, yt    );

    bool su = !u && CONNECTS_TO_SAND(level, xt,     yt - 1);
    bool sd = !d && CONNECTS_TO_SAND(level, xt,     yt + 1);
    bool sl = !l && CONNECTS_TO_SAND(level, xt - 1, yt    );
    bool sr = !r && CONNECTS_TO_SAND(level, xt + 1, yt    );

    if(u && l)
        tiles[0] = TILE_M((water_rand >> 0) & 3, (water_rand >> 2) & 3, 2);
    else
        tiles[0] = TILE(24 + u * 7 + l * 4, 2 + (su || sl) * 1);

    if(u && r)
        tiles[1] = TILE_M((water_rand >> 4) & 3, (water_rand >> 6) & 3, 2);
    else
        tiles[1] = TILE(25 + u * 4 + r * 3, 2 + (su || sr) * 1);

    if(d && l)
        tiles[2] = TILE_M((water_rand >> 8) & 3, (water_rand >> 10) & 3, 2);
    else
        tiles[2] = TILE(26 + d * 5 + l * 4, 2 + (sd || sl) * 1);

    if(d && r)
        tiles[3] = TILE_M((water_rand >> 12) & 3, (water_rand >> 14) & 3, 2);
    else
        tiles[3] = TILE(27 + d * 2 + r * 3, 2 + (sd || sr) * 1);
}

// Flower
FDRAW(flower_draw) {
    bool u = CONNECTS_TO_GRASS(level, xt,     yt - 1);
    bool d = CONNECTS_TO_GRASS(level, xt,     yt + 1);
    bool l = CONNECTS_TO_GRASS(level, xt - 1, yt    );
    bool r = CONNECTS_TO_GRASS(level, xt + 1, yt    );

    bool shape = (LEVEL_GET_DATA(level, xt, yt) >> 5) & 0x01;

    if(shape) {
        if(u && l)
            tiles[0] = TILE(0, 0);
        else
            tiles[0] = TILE(4 + u * 7 + l * 4, 0);

        tiles[1] = TILE(33, 0);

        tiles[2] = TILE(33, 0);

        if(d && r)
            tiles[3] = TILE(3, 0);
        else
            tiles[3] = TILE(7 + d * 2 + r * 3, 0);
    } else {
        tiles[0] = TILE(33, 0);

        if(u && r)
            tiles[1] = TILE(1, 0);
        else
            tiles[1] = TILE(5 + u * 4 + r * 3, 0);

        if(d && l)
            tiles[2] = TILE(2, 0);
        else
            tiles[2] = TILE(6 + d * 5 + l * 4, 0);

        tiles[3] = TILE(33, 0);
    }
}

// Tree
FDRAW(tree_draw) {
    bool u = LEVEL_GET_TILE(level, xt,     yt - 1) == TREE_TILE;
    bool d = LEVEL_GET_TILE(level, xt,     yt + 1) == TREE_TILE;
    bool l = LEVEL_GET_TILE(level, xt - 1, yt    ) == TREE_TILE;
    bool r = LEVEL_GET_TILE(level, xt + 1, yt    ) == TREE_TILE;

    bool ul = LEVEL_GET_TILE(level, xt - 1, yt - 1) == TREE_TILE;
    bool dl = LEVEL_GET_TILE(level, xt - 1, yt + 1) == TREE_TILE;
    bool ur = LEVEL_GET_TILE(level, xt + 1, yt - 1) == TREE_TILE;
    bool dr = LEVEL_GET_TILE(level, xt + 1, yt + 1) == TREE_TILE;

    if(u && l && ul)
        tiles[0] = TILE(38, 0);
    else
        tiles[0] = TILE(34, 0);

    if(u && r && ur)
        tiles[1] = TILE(39, 0);
    else
        tiles[1] = TILE(35, 0);

    if(d && l && dl)
        tiles[2] = TILE(39, 0);
    else
        tiles[2] = TILE(36, 0);

    if(d && r && dr)
        tiles[3] = TILE(38, 0);
    else
        tiles[3] = TILE(37, 0);
}

// Dirt
FDRAW(dirt_draw) {
    tiles[0] = TILE(64, 2);
    tiles[1] = TILE(65, 2);
    tiles[2] = TILE(66, 2);
    tiles[3] = TILE(67, 2);
}

// Sand
FDRAW(sand_draw) {
    bool u = CONNECTS_TO_SAND(level, xt,     yt - 1);
    bool d = CONNECTS_TO_SAND(level, xt,     yt + 1);
    bool l = CONNECTS_TO_SAND(level, xt - 1, yt    );
    bool r = CONNECTS_TO_SAND(level, xt + 1, yt    );

    bool stepped_on = LEVEL_GET_DATA(level, xt, yt) != 0;

    if(u && l)
        tiles[0] = TILE(0 + stepped_on * 40, 4);
    else
        tiles[0] = TILE(4 + u * 7 + l * 4, 4);

    if(u && r)
        tiles[1] = TILE(1, 4);
    else
        tiles[1] = TILE(5 + u * 4 + r * 3, 4);

    if(d && l)
        tiles[2] = TILE(2, 4);
    else
        tiles[2] = TILE(6 + d * 5 + l * 4, 4);

    if(d && r)
        tiles[3] = TILE(3 + stepped_on * 37, 4);
    else
        tiles[3] = TILE(7 + d * 2 + r * 3, 4);
}

FSTEPPED_ON(sand_stepped_on) {
    u8 etype = entity_data->type;
    if(etype == ZOMBIE_ENTITY ||
       etype == SLIME_ENTITY ||
       etype == PLAYER_ENTITY) {
        LEVEL_SET_DATA(level, xt, yt, 10);
    }
}

// Cactus
FDRAW(cactus_draw) {
    tiles[0] = TILE(41, 4);
    tiles[1] = TILE(42, 4);
    tiles[2] = TILE(43, 4);
    tiles[3] = TILE(44, 4);
}

// Hole
FDRAW(hole_draw) {
    bool u = CONNECTS_TO_LIQUID(level, xt,     yt - 1);
    bool d = CONNECTS_TO_LIQUID(level, xt,     yt + 1);
    bool l = CONNECTS_TO_LIQUID(level, xt - 1, yt    );
    bool r = CONNECTS_TO_LIQUID(level, xt + 1, yt    );

    bool su = !u && CONNECTS_TO_SAND(level, xt,     yt - 1);
    bool sd = !d && CONNECTS_TO_SAND(level, xt,     yt + 1);
    bool sl = !l && CONNECTS_TO_SAND(level, xt - 1, yt    );
    bool sr = !r && CONNECTS_TO_SAND(level, xt + 1, yt    );

    if(u && l)
        tiles[0] = TILE(0, 5);
    else
        tiles[0] = TILE(24 + u * 7 + l * 4, 5 + (su || sl) * 1);

    if(u && r)
        tiles[1] = TILE(1, 5);
    else
        tiles[1] = TILE(25 + u * 4 + r * 3, 5 + (su || sr) * 1);

    if(d && l)
        tiles[2] = TILE(2, 5);
    else
        tiles[2] = TILE(26 + d * 5 + l * 4, 5 + (sd || sl) * 1);

    if(d && r)
        tiles[3] = TILE(3, 5);
    else
        tiles[3] = TILE(27 + d * 2 + r * 3, 5 + (sd || sr) * 1);
}

// Tree Sapling
FTICK(tree_sapling_tick) {
    u8 age = LEVEL_GET_DATA(level, xt, yt) + 1;

    if(age > 100)
        LEVEL_SET_TILE(level, xt, yt, TREE_TILE, 0);
    else
        LEVEL_SET_DATA(level, xt, yt, age);
}

FDRAW(tree_sapling_draw) {
    grass_draw(level, xt, yt, tiles, tiles2);

    tiles2[0] = TILE(45, 0);
    tiles2[1] = TILE(46, 0);
    tiles2[2] = TILE(47, 0);
    tiles2[3] = TILE(48, 0);
}

// Cactus Sapling
FTICK(cactus_sapling_tick) {
    u8 age = LEVEL_GET_DATA(level, xt, yt) + 1;

    if(age > 100)
        LEVEL_SET_TILE(level, xt, yt, CACTUS_TILE, 0);
    else
        LEVEL_SET_DATA(level, xt, yt, age);
}

FDRAW(cactus_sapling_draw) {
    sand_draw(level, xt, yt, tiles, tiles2);

    tiles2[0] = TILE(45, 0);
    tiles2[1] = TILE(46, 0);
    tiles2[2] = TILE(47, 0);
    tiles2[3] = TILE(48, 0);
}

// Farmland
FTICK(farmland_tick) {
    u8 age = LEVEL_GET_DATA(level, xt, yt);
    if(age < 5)
        LEVEL_SET_DATA(level, xt, yt, age + 1);
}

FDRAW(farmland_draw) {
    tiles[0] = TILE_M(49, 0x1, 3);
    tiles[1] = TILE_M(49, 0x0, 3);
    tiles[2] = TILE_M(49, 0x0, 3);
    tiles[3] = TILE_M(49, 0x1, 3);
}

FSTEPPED_ON(farmland_stepped_on) {
    if(rand() % 60 != 0)
        return;

    if(LEVEL_GET_DATA(level, xt, yt) >= 5)
        LEVEL_SET_TILE(level, xt, yt, DIRT_TILE, 0);
}

// Wheat
FTICK(wheat_tick) {
    if(rand() & 1)
        return;

    u8 age = LEVEL_GET_DATA(level, xt, yt);
    if(age < 50)
        LEVEL_SET_DATA(level, xt, yt, age + 1);
}

FSTEPPED_ON(wheat_stepped_on) {
    if(rand() % 60 != 0)
        return;

    if(LEVEL_GET_DATA(level, xt, yt) >= 2)
        ; // TODO harvest
}

FDRAW(wheat_draw) {
    // TODO test if this is accurate
    u8 age = LEVEL_GET_DATA(level, xt, yt) / 10;

    tiles[0] = TILE_M(50 + age, 0x0, 3);
    tiles[1] = TILE_M(50 + age, 0x0, 3);
    tiles[2] = TILE_M(50 + age, 0x1, 3);
    tiles[3] = TILE_M(50 + age, 0x1, 3);
}

// Lava
FTICK(lava_tick) {
    i32 xn = xt;
    i32 yn = yt;

    if(rand() & 1)
        xn += (rand() & 1) * 2 - 1;
    else
        yn += (rand() & 1) * 2 - 1;

    if(LEVEL_GET_TILE(level, xn, yn) == HOLE_TILE)
        LEVEL_SET_TILE(level, xn, yn, LAVA_TILE, 0);
}

// Stairs Down
FDRAW(stairs_down_draw) {
    tiles[0] = TILE(56, 1);
    tiles[1] = TILE(57, 1);
    tiles[2] = TILE(58, 1);
    tiles[3] = TILE(59, 1);
}

// Stairs Up
FDRAW(stairs_up_draw) {
    tiles[0] = TILE(60, 1);
    tiles[1] = TILE(61, 1);
    tiles[2] = TILE(62, 1);
    tiles[3] = TILE(63, 1);
}

// Cloud
FDRAW(cloud_draw) {
    bool u = LEVEL_GET_TILE(level, xt,     yt - 1) != INFINITE_FALL_TILE;
    bool d = LEVEL_GET_TILE(level, xt,     yt + 1) != INFINITE_FALL_TILE;
    bool l = LEVEL_GET_TILE(level, xt - 1, yt    ) != INFINITE_FALL_TILE;
    bool r = LEVEL_GET_TILE(level, xt + 1, yt    ) != INFINITE_FALL_TILE;

    bool ul = LEVEL_GET_TILE(level, xt - 1, yt - 1) != INFINITE_FALL_TILE;
    bool dl = LEVEL_GET_TILE(level, xt - 1, yt + 1) != INFINITE_FALL_TILE;
    bool ur = LEVEL_GET_TILE(level, xt + 1, yt - 1) != INFINITE_FALL_TILE;
    bool dr = LEVEL_GET_TILE(level, xt + 1, yt + 1) != INFINITE_FALL_TILE;

    if(u && l)
        tiles[0] = TILE(69 - !ul * 50, 7);
    else
        tiles[0] = TILE(12 + u * 7 + l * 4, 7);

    if(u && r)
        tiles[1] = TILE(68 - !ur * 48, 7);
    else
        tiles[1] = TILE(13 + u * 4 + r * 3, 7);

    if(d && l)
        tiles[2] = TILE(68 - !dl * 47, 7);
    else
        tiles[2] = TILE(14 + d * 5 + l * 4, 7);

    if(d && r)
        tiles[3] = TILE(70 - !dr * 48, 7);
    else
        tiles[3] = TILE(15 + d * 2 + r * 3, 7);
}

// Hard Rock
FDRAW(hard_rock_draw) {
    bool u = LEVEL_GET_TILE(level, xt,     yt - 1) == HARD_ROCK_TILE;
    bool d = LEVEL_GET_TILE(level, xt,     yt + 1) == HARD_ROCK_TILE;
    bool l = LEVEL_GET_TILE(level, xt - 1, yt    ) == HARD_ROCK_TILE;
    bool r = LEVEL_GET_TILE(level, xt + 1, yt    ) == HARD_ROCK_TILE;

    bool ul = LEVEL_GET_TILE(level, xt - 1, yt - 1) == HARD_ROCK_TILE;
    bool dl = LEVEL_GET_TILE(level, xt - 1, yt + 1) == HARD_ROCK_TILE;
    bool ur = LEVEL_GET_TILE(level, xt + 1, yt - 1) == HARD_ROCK_TILE;
    bool dr = LEVEL_GET_TILE(level, xt + 1, yt + 1) == HARD_ROCK_TILE;

    if(u && l)
        tiles[0] = TILE(0 + !ul * 20, 8);
    else
        tiles[0] = TILE(12 + u * 7 + l * 4, 8);

    if(u && r)
        tiles[1] = TILE(1 + !ur * 20, 8);
    else
        tiles[1] = TILE(13 + u * 4 + r * 3, 8);

    if(d && l)
        tiles[2] = TILE(2 + !dl * 20, 8);
    else
        tiles[2] = TILE(14 + d * 5 + l * 4, 8);

    if(d && r)
        tiles[3] = TILE(3 + !dr * 20, 8);
    else
        tiles[3] = TILE(15 + d * 2 + r * 3, 8);
}

// Ores + Cloud Cactus
FDRAW(ore_draw) {
    tiles[0] = TILE(71, 2);
    tiles[1] = TILE(72, 2);
    tiles[2] = TILE(73, 2);
    tiles[3] = TILE(74, 2);
}

IWRAM_RODATA_SECTION
const struct Tile tile_list[TILE_TYPES] = {
    // Grass
    {
        .tick = grass_tick,
        .draw = grass_draw,

        .connects_to = {
            .grass = true
        },

        .interact = grass_interact
    },

    // Rock
    {
        .tick = damage_recover_tick,
        .draw = rock_draw,

        .is_solid = true,
        .may_pass = -1,

        .interact = rock_interact
    },

    // Water
    {
        .tick = water_tick,
        .draw = water_draw,

        .connects_to = {
            .sand   = true,
            .liquid = true
        },

        .is_solid = true,
        .may_pass = PLAYER_ENTITY
    },

    // Flower
    {
        .tick = grass_tick,
        .draw = flower_draw,

        .connects_to = {
            .grass = true
        }
    },

    // Tree
    {
        .tick = damage_recover_tick,
        .draw = tree_draw,

        .connects_to = {
            .grass = true
        },

        .is_solid = true,
        .may_pass = -1
    },

    // Dirt
    {
        .tick = NULL,
        .draw = dirt_draw
    },

    // Sand
    {
        .tick = damage_recover_tick,
        .draw = sand_draw,

        .connects_to = {
            .sand = true
        },

        .stepped_on = sand_stepped_on
    },

    // Cactus
    {
        .tick = damage_recover_tick,
        .draw = cactus_draw,

        .connects_to = {
            .sand = true
        },

        .is_solid = true,
        .may_pass = -1,

        .hurts_on_touch = true,
        .touch_damage = 1
    },

    // Hole
    {
        .tick = NULL,
        .draw = hole_draw,

        .connects_to = {
            .sand   = true,
            .liquid = true,
        },

        .is_solid = true,
        .may_pass = PLAYER_ENTITY
    },

    // Tree Sapling
    {
        .tick = tree_sapling_tick,
        .draw = tree_sapling_draw,

        .connects_to = {
            .grass = true
        }
    },

    // Cactus Sapling
    {
        .tick = cactus_sapling_tick,
        .draw = cactus_sapling_draw,

        .connects_to = {
            .sand = true
        }
    },

    // Farmland
    {
        .tick = farmland_tick,
        .draw = farmland_draw,

        .stepped_on = farmland_stepped_on
    },

    // Wheat
    {
        .tick = wheat_tick,
        .draw = wheat_draw,

        .stepped_on = wheat_stepped_on
    },

    // Lava
    {
        .tick = lava_tick,
        .draw = water_draw,

        .connects_to = {
            .sand   = true,
            .liquid = true
        },

        .is_solid = true,
        .may_pass = PLAYER_ENTITY
    },

    // Stairs Down
    {
        .tick = NULL,
        .draw = stairs_down_draw
    },

    // Stairs Up
    {
        .tick = NULL,
        .draw = stairs_up_draw
    },

    // Infinite Fall
    {
        .tick = NULL,
        .draw = NULL,

        .is_solid = true,
        .may_pass = AIR_WIZARD_ENTITY
    },

    // Cloud
    {
        .tick = NULL,
        .draw = cloud_draw
    },

    // Hard Rock
    {
        .tick = damage_recover_tick,
        .draw = hard_rock_draw,

        .is_solid = true,
        .may_pass = -1
    },

    // Iron Ore
    {
        .tick = NULL,
        .draw = ore_draw,

        .is_solid = true,
        .may_pass = -1,

        .hurts_on_touch = true,
        .touch_damage = 3
    },

    // Gold Ore
    {
        .tick = NULL,
        .draw = ore_draw,

        .is_solid = true,
        .may_pass = -1,

        .hurts_on_touch = true,
        .touch_damage = 3
    },

    // Gem Ore
    {
        .tick = NULL,
        .draw = ore_draw,

        .is_solid = true,
        .may_pass = -1,

        .hurts_on_touch = true,
        .touch_damage = 3
    },

    // Cloud Cactus
    {
        .tick = NULL,
        .draw = ore_draw,

        .is_solid = true,
        .may_pass = AIR_WIZARD_ENTITY,

        .hurts_on_touch = true,
        .touch_damage = 3
    }
};
