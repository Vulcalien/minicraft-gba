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

// TODO rename SPR to something else: not sprites!
#define SPR(id, palette) ((id) | ((palette) << 12))
#define TILE_M(id, flip_h, flip_v, palette)\
    ((id) | (flip_h << 10) | (flip_v << 11) | (palette << 12))

#define FTICK(name)\
    static void name(struct Level *level, u32 xt, u32 yt)

#define FDRAW(name)\
    IWRAM_SECTION\
    static void name(struct Level *level, u32 xt, u32 yt,\
                     u16 tiles[4], u16 tiles2[4])

// CONNECTS_TO_X
#define CONNECTS_TO_GRASS(level, xt, yt)\
    (LEVEL_GET_TILE_S((level), (xt), (yt))->connects_to.grass)
#define CONNECTS_TO_SAND(level, xt, yt)\
    (LEVEL_GET_TILE_S((level), (xt), (yt))->connects_to.sand)
#define CONNECTS_TO_LIQUID(level, xt, yt)\
    (LEVEL_GET_TILE_S((level), (xt), (yt))->connects_to.liquid)

// NOP
FTICK(nop_tick) {}
FDRAW(nop_draw) {}

// DAMAGE RECOVER
FTICK(damage_recover_tick) {
    u32 damage = LEVEL_GET_DATA(level, xt, yt);
    if(damage != 0)
        LEVEL_SET_DATA(level, xt, yt, damage - 1);
}

// Grass
FTICK(grass_tick) {
}

FDRAW(grass_draw) {
    bool u = CONNECTS_TO_GRASS(level, xt,     yt - 1);
    bool d = CONNECTS_TO_GRASS(level, xt,     yt + 1);
    bool l = CONNECTS_TO_GRASS(level, xt - 1, yt    );
    bool r = CONNECTS_TO_GRASS(level, xt + 1, yt    );

    if(u && l)
        tiles[0] = SPR(0, 0);
    else
        tiles[0] = SPR(4 + u * 7 + l * 4, 0);

    if(u && r)
        tiles[1] = SPR(1, 0);
    else
        tiles[1] = SPR(5 + u * 4 + r * 3, 0);

    if(d && l)
        tiles[2] = SPR(2, 0);
    else
        tiles[2] = SPR(6 + d * 5 + l * 4, 0);

    if(d && r)
        tiles[3] = SPR(3, 0);
    else
        tiles[3] = SPR(7 + d * 2 + r * 3, 0);
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
        tiles[0] = SPR(0 + !ul * 20, 1);
    else
        tiles[0] = SPR(12 + u * 7 + l * 4, 1);

    if(u && r)
        tiles[1] = SPR(1 + !ur * 20, 1);
    else
        tiles[1] = SPR(13 + u * 4 + r * 3, 1);

    if(d && l)
        tiles[2] = SPR(2 + !dl * 20, 1);
    else
        tiles[2] = SPR(14 + d * 5 + l * 4, 1);

    if(d && r)
        tiles[3] = SPR(3 + !dr * 20, 1);
    else
        tiles[3] = SPR(15 + d * 2 + r * 3, 1);
}

// Water
FTICK(water_tick) {
}

FDRAW(water_draw) {
    bool u = CONNECTS_TO_LIQUID(level, xt,     yt - 1);
    bool d = CONNECTS_TO_LIQUID(level, xt,     yt + 1);
    bool l = CONNECTS_TO_LIQUID(level, xt - 1, yt    );
    bool r = CONNECTS_TO_LIQUID(level, xt + 1, yt    );

    bool su = !u && CONNECTS_TO_SAND(level, xt,     yt - 1);
    bool sd = !d && CONNECTS_TO_SAND(level, xt,     yt + 1);
    bool sl = !l && CONNECTS_TO_SAND(level, xt - 1, yt    );
    bool sr = !r && CONNECTS_TO_SAND(level, xt + 1, yt    );

    // TODO animation

    if(u && l)
        tiles[0] = SPR(0, 2);
    else
        tiles[0] = SPR(24 + u * 7 + l * 4, 2 + (su || sl) * 1);

    if(u && r)
        tiles[1] = SPR(1, 2);
    else
        tiles[1] = SPR(25 + u * 4 + r * 3, 2 + (su || sr) * 1);

    if(d && l)
        tiles[2] = SPR(2, 2);
    else
        tiles[2] = SPR(26 + d * 5 + l * 4, 2 + (sd || sl) * 1);

    if(d && r)
        tiles[3] = SPR(3, 2);
    else
        tiles[3] = SPR(27 + d * 2 + r * 3, 2 + (sd || sr) * 1);
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
            tiles[0] = SPR(0, 0);
        else
            tiles[0] = SPR(4 + u * 7 + l * 4, 0);

        tiles[1] = SPR(33, 0);

        tiles[2] = SPR(33, 0);

        if(d && r)
            tiles[3] = SPR(3, 0);
        else
            tiles[3] = SPR(7 + d * 2 + r * 3, 0);
    } else {
        tiles[0] = SPR(33, 0);

        if(u && r)
            tiles[1] = SPR(1, 0);
        else
            tiles[1] = SPR(5 + u * 4 + r * 3, 0);

        if(d && l)
            tiles[2] = SPR(2, 0);
        else
            tiles[2] = SPR(6 + d * 5 + l * 4, 0);

        tiles[3] = SPR(33, 0);
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
        tiles[0] = SPR(38, 0);
    else
        tiles[0] = SPR(34, 0);

    if(u && r && ur)
        tiles[1] = SPR(39, 0);
    else
        tiles[1] = SPR(35, 0);

    if(d && l && dl)
        tiles[2] = SPR(39, 0);
    else
        tiles[2] = SPR(36, 0);

    if(d && r && dr)
        tiles[3] = SPR(38, 0);
    else
        tiles[3] = SPR(37, 0);
}

// Dirt
FDRAW(dirt_draw) {
    tiles[0] = SPR(65, 2);
    tiles[1] = SPR(66, 2);
    tiles[2] = SPR(67, 2);
    tiles[3] = SPR(68, 2);
}

// Sand
FDRAW(sand_draw) {
    bool u = CONNECTS_TO_SAND(level, xt,     yt - 1);
    bool d = CONNECTS_TO_SAND(level, xt,     yt + 1);
    bool l = CONNECTS_TO_SAND(level, xt - 1, yt    );
    bool r = CONNECTS_TO_SAND(level, xt + 1, yt    );

    bool stepped_on = LEVEL_GET_DATA(level, xt, yt) != 0;

    if(u && l)
        tiles[0] = SPR(0 + stepped_on * 40, 4);
    else
        tiles[0] = SPR(4 + u * 7 + l * 4, 4);

    if(u && r)
        tiles[1] = SPR(1, 4);
    else
        tiles[1] = SPR(5 + u * 4 + r * 3, 4);

    if(d && l)
        tiles[2] = SPR(2, 4);
    else
        tiles[2] = SPR(6 + d * 5 + l * 4, 4);

    if(d && r)
        tiles[3] = SPR(3 + stepped_on * 37, 4);
    else
        tiles[3] = SPR(7 + d * 2 + r * 3, 4);
}

// Cactus
FDRAW(cactus_draw) {
    tiles[0] = SPR(41, 4);
    tiles[1] = SPR(42, 4);
    tiles[2] = SPR(43, 4);
    tiles[3] = SPR(44, 4);
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
        tiles[0] = SPR(0, 5);
    else
        tiles[0] = SPR(24 + u * 7 + l * 4, 5 + (su || sl) * 2);

    if(u && r)
        tiles[1] = SPR(1, 5);
    else
        tiles[1] = SPR(25 + u * 4 + r * 3, 5 + (su || sr) * 2);

    if(d && l)
        tiles[2] = SPR(2, 5);
    else
        tiles[2] = SPR(26 + d * 5 + l * 4, 5 + (sd || sl) * 2);

    if(d && r)
        tiles[3] = SPR(3, 5);
    else
        tiles[3] = SPR(27 + d * 2 + r * 3, 5 + (sd || sr) * 2);
}

// Tree/Cactus Sapling
FTICK(sapling_tick) {
}

FDRAW(tree_sapling_draw) {
    grass_draw(level, xt, yt, tiles, tiles2);

    tiles2[0] = SPR(45, 0);
    tiles2[1] = SPR(46, 0);
    tiles2[2] = SPR(47, 0);
    tiles2[3] = SPR(48, 0);
}

FDRAW(cactus_sapling_draw) {
    sand_draw(level, xt, yt, tiles, tiles2);

    tiles2[0] = SPR(45, 0);
    tiles2[1] = SPR(46, 0);
    tiles2[2] = SPR(47, 0);
    tiles2[3] = SPR(48, 0);
}

// Farmland
FTICK(farmland_tick) {
}

FDRAW(farmland_draw) {
    tiles[0] = TILE_M(49, true, false, 3);
    tiles[1] = TILE_M(49, false, false, 3);
    tiles[2] = TILE_M(49, false, false, 3);
    tiles[3] = TILE_M(49, true, false, 3);
}

// Wheat
FTICK(wheat_tick) {
}

FDRAW(wheat_draw) {
    // TODO test if this is accurate
    u32 age = LEVEL_GET_DATA(level, xt, yt) / 10;

    tiles[0] = TILE_M(50 + age, false, false, 3);
    tiles[1] = TILE_M(50 + age, false, false, 3);
    tiles[2] = TILE_M(50 + age, true, false, 3);
    tiles[3] = TILE_M(50 + age, true, false, 3);
}

// Lava
FTICK(lava_tick) {
}

// Stairs Down
FDRAW(stairs_down_draw) {
    tiles[0] = SPR(57, 1);
    tiles[1] = SPR(58, 1);
    tiles[2] = SPR(59, 1);
    tiles[3] = SPR(60, 1);
}

// Stairs Up
FDRAW(stairs_up_draw) {
    tiles[0] = SPR(61, 1);
    tiles[1] = SPR(62, 1);
    tiles[2] = SPR(63, 1);
    tiles[3] = SPR(64, 1);
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
        tiles[0] = SPR(70 - !ul * 50, 8);
    else
        tiles[0] = SPR(12 + u * 7 + l * 4, 8);

    if(u && r)
        tiles[1] = SPR(69 - !ur * 48, 8);
    else
        tiles[1] = SPR(13 + u * 4 + r * 3, 8);

    if(d && l)
        tiles[2] = SPR(69 - !dl * 47, 8);
    else
        tiles[2] = SPR(14 + d * 5 + l * 4, 8);

    if(d && r)
        tiles[3] = SPR(71 - !dr * 48, 8);
    else
        tiles[3] = SPR(15 + d * 2 + r * 3, 8);
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
        tiles[0] = SPR(0 + !ul * 20, 9);
    else
        tiles[0] = SPR(12 + u * 7 + l * 4, 9);

    if(u && r)
        tiles[1] = SPR(1 + !ur * 20, 9);
    else
        tiles[1] = SPR(13 + u * 4 + r * 3, 9);

    if(d && l)
        tiles[2] = SPR(2 + !dl * 20, 9);
    else
        tiles[2] = SPR(14 + d * 5 + l * 4, 9);

    if(d && r)
        tiles[3] = SPR(3 + !dr * 20, 9);
    else
        tiles[3] = SPR(15 + d * 2 + r * 3, 9);
}

// Ores + Cloud Cactus
FDRAW(ore_draw) {
    tiles[0] = SPR(72, 2);
    tiles[1] = SPR(73, 2);
    tiles[2] = SPR(74, 2);
    tiles[3] = SPR(75, 2);
}

const struct Tile tile_list[TILES_COUNT] = {
    // Grass
    {
        .tick = grass_tick,
        .draw = grass_draw,

        .connects_to = {
            .grass = true
        }
    },

    // Rock
    {
        .tick = damage_recover_tick,
        .draw = rock_draw
    },

    // Water
    {
        .tick = water_tick,
        .draw = water_draw,

        .connects_to = {
            .sand   = true,
            .liquid = true
        }
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
        }
    },

    // Dirt
    {
        .tick = nop_tick,
        .draw = dirt_draw
    },

    // Sand
    {
        .tick = damage_recover_tick,
        .draw = sand_draw,

        .connects_to = {
            .sand = true
        }
    },

    // Cactus
    {
        .tick = damage_recover_tick,
        .draw = cactus_draw,

        .connects_to = {
            .sand = true
        }
    },

    // Hole
    {
        .tick = nop_tick,
        .draw = hole_draw,

        .connects_to = {
            .sand   = true,
            .liquid = true,
        }
    },

    // Tree Sapling
    {
        .tick = sapling_tick,
        .draw = tree_sapling_draw,

        .connects_to = {
            .grass = true
        }
    },

    // Cactus Sapling
    {
        .tick = sapling_tick,
        .draw = cactus_sapling_draw,

        .connects_to = {
            .sand = true
        }
    },

    // Farmland
    {
        .tick = farmland_tick,
        .draw = farmland_draw
    },

    // Wheat
    {
        .tick = wheat_tick,
        .draw = wheat_draw
    },

    // Lava
    {
        .tick = lava_tick,
        .draw = water_draw,

        .connects_to = {
            .sand   = true,
            .liquid = true
        }
    },

    // Stairs Down
    {
        .tick = nop_tick,
        .draw = stairs_down_draw
    },

    // Stairs Up
    {
        .tick = nop_tick,
        .draw = stairs_up_draw
    },

    // Infinite Fall
    {
        .tick = nop_tick,
        .draw = nop_draw
    },

    // Cloud
    {
        .tick = nop_tick,
        .draw = cloud_draw
    },

    // Hard Rock
    {
        .tick = damage_recover_tick,
        .draw = hard_rock_draw
    },

    // Iron Ore
    {
        .tick = nop_tick,
        .draw = ore_draw
    },

    // Gold Ore
    {
        .tick = nop_tick,
        .draw = ore_draw
    },

    // Gem Ore
    {
        .tick = nop_tick,
        .draw = ore_draw
    },

    // Cloud Cactus
    {
        .tick = nop_tick,
        .draw = ore_draw
    }
};
