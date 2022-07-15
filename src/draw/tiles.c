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

#define FDRAW(name)\
    static inline void name(struct Level *level, u32 xt, u32 yt,\
                            u16 tiles[4], u16 tiles2[4])

#define TILE(id, palette) ((id) | ((palette) << 12))
#define TILE_M(id, flip, palette)\
    ((id) | (flip << 10) | (palette << 12))

// CONNECTS_TO_X
#define CONNECTS_TO_GRASS(level, xt, yt)\
    (LEVEL_GET_TILE_S((level), (xt), (yt))->connects_to.grass)
#define CONNECTS_TO_SAND(level, xt, yt)\
    (LEVEL_GET_TILE_S((level), (xt), (yt))->connects_to.sand)
#define CONNECTS_TO_LIQUID(level, xt, yt)\
    (LEVEL_GET_TILE_S((level), (xt), (yt))->connects_to.liquid)

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

FDRAW(flower_draw) {
    bool u = CONNECTS_TO_GRASS(level, xt,     yt - 1);
    bool d = CONNECTS_TO_GRASS(level, xt,     yt + 1);
    bool l = CONNECTS_TO_GRASS(level, xt - 1, yt    );
    bool r = CONNECTS_TO_GRASS(level, xt + 1, yt    );

    bool shape = LEVEL_GET_DATA(level, xt, yt) & 1;

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

FDRAW(dirt_draw) {
    tiles[0] = TILE(64, 2);
    tiles[1] = TILE(65, 2);
    tiles[2] = TILE(66, 2);
    tiles[3] = TILE(67, 2);
}

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

FDRAW(cactus_draw) {
    tiles[0] = TILE(41, 4);
    tiles[1] = TILE(42, 4);
    tiles[2] = TILE(43, 4);
    tiles[3] = TILE(44, 4);
}

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

FDRAW(sapling_draw) {
    tiles2[0] = TILE(45, 0);
    tiles2[1] = TILE(46, 0);
    tiles2[2] = TILE(47, 0);
    tiles2[3] = TILE(48, 0);
}

FDRAW(farmland_draw) {
    tiles[0] = TILE_M(49, 0x1, 3);
    tiles[1] = TILE_M(49, 0x0, 3);
    tiles[2] = TILE_M(49, 0x0, 3);
    tiles[3] = TILE_M(49, 0x1, 3);
}

FDRAW(wheat_draw) {
    // TODO test if this is accurate
    u8 age = LEVEL_GET_DATA(level, xt, yt) / 10;

    tiles[0] = TILE_M(50 + age, 0x0, 3);
    tiles[1] = TILE_M(50 + age, 0x0, 3);
    tiles[2] = TILE_M(50 + age, 0x1, 3);
    tiles[3] = TILE_M(50 + age, 0x1, 3);
}

FDRAW(stairs_down_draw) {
    tiles[0] = TILE(56, 1);
    tiles[1] = TILE(57, 1);
    tiles[2] = TILE(58, 1);
    tiles[3] = TILE(59, 1);
}

FDRAW(stairs_up_draw) {
    tiles[0] = TILE(60, 1);
    tiles[1] = TILE(61, 1);
    tiles[2] = TILE(62, 1);
    tiles[3] = TILE(63, 1);
}

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

FDRAW(ore_draw) {
    tiles[0] = TILE(71, 2);
    tiles[1] = TILE(72, 2);
    tiles[2] = TILE(73, 2);
    tiles[3] = TILE(74, 2);
}

#define CALL(function)\
    function(level, xt, yt, tiles, tiles2)

static inline void draw_tile(struct Level *level, u32 xt, u32 yt,
                             u16 tiles[4], u16 tiles2[4]) {
    switch(LEVEL_GET_TILE(level, xt, yt)) {
        case TREE_SAPLING_TILE:
            CALL(sapling_draw);
        case GRASS_TILE:
            CALL(grass_draw);
            break;

        case ROCK_TILE:
            CALL(rock_draw);
            break;

        case WATER_TILE:
        case LAVA_TILE:
            CALL(water_draw);
            break;

        case FLOWER_TILE:
            CALL(flower_draw);
            break;

        case TREE_TILE:
            CALL(tree_draw);
            break;

        case DIRT_TILE:
            CALL(dirt_draw);
            break;

        case CACTUS_SAPLING_TILE:
            CALL(sapling_draw);
        case SAND_TILE:
            CALL(sand_draw);
            break;

        case CACTUS_TILE:
            CALL(cactus_draw);
            break;

        case HOLE_TILE:
            CALL(hole_draw);
            break;

        case FARMLAND_TILE:
            CALL(farmland_draw);
            break;

        case WHEAT_TILE:
            CALL(wheat_draw);
            break;

        case STAIRS_DOWN_TILE:
            CALL(stairs_down_draw);
            break;

        case STAIRS_UP_TILE:
            CALL(stairs_up_draw);
            break;

        case CLOUD_TILE:
            CALL(cloud_draw);
            break;

        case HARD_ROCK_TILE:
            CALL(hard_rock_draw);
            break;

        case IRON_ORE_TILE:
        case GOLD_ORE_TILE:
        case GEM_ORE_TILE:
        case CLOUD_CACTUS_TILE:
            CALL(ore_draw);
            break;
    }
}
