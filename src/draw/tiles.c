/* Copyright 2022, 2024 Vulcalien
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
                            u16 tiles[4])

#define TILE(id, palette) ((id) | (palette) << 12)
#define TILE_M(id, flip, palette)\
    ((id) | (flip) << 10 | (palette) << 12)

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
        tiles[0] = TILE(0 + !ul * 20, 2);
    else
        tiles[0] = TILE(12 + u * 7 + l * 4, 2);

    if(u && r)
        tiles[1] = TILE(1 + !ur * 20, 2);
    else
        tiles[1] = TILE(13 + u * 4 + r * 3, 2);

    if(d && l)
        tiles[2] = TILE(2 + !dl * 20, 2);
    else
        tiles[2] = TILE(14 + d * 5 + l * 4, 2);

    if(d && r)
        tiles[3] = TILE(3 + !dr * 20, 2);
    else
        tiles[3] = TILE(15 + d * 2 + r * 3, 2);
}

FDRAW(liquid_draw) {
    u32 liquid_rand = (tick_count + 0x109f77 * xt - 0xab24af3 * yt) / 10;
    liquid_rand = (liquid_rand * 0x248f7b13 + 0xc21840c5) >> 16;

    bool u = CONNECTS_TO_LIQUID(level, xt,     yt - 1);
    bool d = CONNECTS_TO_LIQUID(level, xt,     yt + 1);
    bool l = CONNECTS_TO_LIQUID(level, xt - 1, yt    );
    bool r = CONNECTS_TO_LIQUID(level, xt + 1, yt    );

    bool su = !u & CONNECTS_TO_SAND(level, xt,     yt - 1);
    bool sd = !d & CONNECTS_TO_SAND(level, xt,     yt + 1);
    bool sl = !l & CONNECTS_TO_SAND(level, xt - 1, yt    );
    bool sr = !r & CONNECTS_TO_SAND(level, xt + 1, yt    );

    if(u && l)
        tiles[0] = TILE_M((liquid_rand >> 0) & 3, (liquid_rand >> 2) & 3, 3);
    else
        tiles[0] = TILE(24 + u * 7 + l * 4, 3 + (su || sl));

    if(u && r)
        tiles[1] = TILE_M((liquid_rand >> 4) & 3, (liquid_rand >> 6) & 3, 3);
    else
        tiles[1] = TILE(25 + u * 4 + r * 3, 3 + (su || sr));

    if(d && l)
        tiles[2] = TILE_M((liquid_rand >> 8) & 3, (liquid_rand >> 10) & 3, 3);
    else
        tiles[2] = TILE(26 + d * 5 + l * 4, 3 + (sd || sl));

    if(d && r)
        tiles[3] = TILE_M((liquid_rand >> 12) & 3, (liquid_rand >> 14) & 3, 3);
    else
        tiles[3] = TILE(27 + d * 2 + r * 3, 3 + (sd || sr));
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

    tiles[0] = TILE(36 - (u && l && ul) * 2, 0);
    tiles[1] = TILE(37 - (u && r && ur) * 2, 0);
    tiles[2] = TILE(38 - (d && l && dl) * 3, 0);
    tiles[3] = TILE(39 - (d && r && dr) * 5, 0);
}

FDRAW(dirt_draw) {
    tiles[0] = TILE(0, 8);
    tiles[1] = TILE(1, 8);
    tiles[2] = TILE(2, 8);
    tiles[3] = TILE(3, 8);
}

FDRAW(sand_draw) {
    bool u = CONNECTS_TO_SAND(level, xt,     yt - 1);
    bool d = CONNECTS_TO_SAND(level, xt,     yt + 1);
    bool l = CONNECTS_TO_SAND(level, xt - 1, yt    );
    bool r = CONNECTS_TO_SAND(level, xt + 1, yt    );

    bool stepped_on = LEVEL_GET_DATA(level, xt, yt) != 0;

    if(u && l)
        tiles[0] = TILE(0 + stepped_on * 84, 1);
    else
        tiles[0] = TILE(4 + u * 7 + l * 4, 1);

    if(u && r)
        tiles[1] = TILE(1, 1);
    else
        tiles[1] = TILE(5 + u * 4 + r * 3, 1);

    if(d && l)
        tiles[2] = TILE(2, 1);
    else
        tiles[2] = TILE(6 + d * 5 + l * 4, 1);

    if(d && r)
        tiles[3] = TILE(3 + stepped_on * 81, 1);
    else
        tiles[3] = TILE(7 + d * 2 + r * 3, 1);
}

FDRAW(cactus_draw) {
    tiles[0] = TILE(40, 1);
    tiles[1] = TILE(41, 1);
    tiles[2] = TILE(42, 1);
    tiles[3] = TILE(43, 1);
}

FDRAW(hole_draw) {
    bool u = CONNECTS_TO_LIQUID(level, xt,     yt - 1);
    bool d = CONNECTS_TO_LIQUID(level, xt,     yt + 1);
    bool l = CONNECTS_TO_LIQUID(level, xt - 1, yt    );
    bool r = CONNECTS_TO_LIQUID(level, xt + 1, yt    );

    bool su = !u & CONNECTS_TO_SAND(level, xt,     yt - 1);
    bool sd = !d & CONNECTS_TO_SAND(level, xt,     yt + 1);
    bool sl = !l & CONNECTS_TO_SAND(level, xt - 1, yt    );
    bool sr = !r & CONNECTS_TO_SAND(level, xt + 1, yt    );

    if(u && l)
        tiles[0] = TILE(0, 5);
    else
        tiles[0] = TILE(24 + u * 7 + l * 4, 2 + (su || sl) * 3);

    if(u && r)
        tiles[1] = TILE(1, 5);
    else
        tiles[1] = TILE(25 + u * 4 + r * 3, 2 + (su || sr) * 3);

    if(d && l)
        tiles[2] = TILE(2, 5);
    else
        tiles[2] = TILE(26 + d * 5 + l * 4, 2 + (sd || sl) * 3);

    if(d && r)
        tiles[3] = TILE(3, 5);
    else
        tiles[3] = TILE(27 + d * 2 + r * 3, 2 + (sd || sr) * 3);
}

FDRAW(tree_sapling_draw) {
    bool u = CONNECTS_TO_GRASS(level, xt,     yt - 1);
    bool d = CONNECTS_TO_GRASS(level, xt,     yt + 1);
    bool l = CONNECTS_TO_GRASS(level, xt - 1, yt    );
    bool r = CONNECTS_TO_GRASS(level, xt + 1, yt    );

    tiles[0] = TILE(56 + !u * 4 + !l * 8, 0);
    tiles[1] = TILE(57 + !u * 4 + !r * 8, 0);
    tiles[2] = TILE(58 + !d * 4 + !l * 8, 0);
    tiles[3] = TILE(59 + !d * 4 + !r * 8, 0);
}

FDRAW(cactus_sapling_draw) {
    bool u = CONNECTS_TO_SAND(level, xt,     yt - 1);
    bool d = CONNECTS_TO_SAND(level, xt,     yt + 1);
    bool l = CONNECTS_TO_SAND(level, xt - 1, yt    );
    bool r = CONNECTS_TO_SAND(level, xt + 1, yt    );

    bool stepped_on = LEVEL_GET_DATA(level, xt, yt) != 0;

    tiles[0] = TILE(56 + !u * 4 + !l * 8 + (u && l && stepped_on) * 29, 1);
    tiles[1] = TILE(57 + !u * 4 + !r * 8, 1);
    tiles[2] = TILE(58 + !d * 4 + !l * 8, 1);
    tiles[3] = TILE(59 + !d * 4 + !r * 8 + (d && r && stepped_on) * 27, 1);
}

FDRAW(farmland_draw) {
    tiles[0] = TILE_M(72, 1, 4);
    tiles[1] = TILE_M(72, 0, 4);
    tiles[2] = TILE_M(72, 0, 4);
    tiles[3] = TILE_M(72, 1, 4);
}

FDRAW(wheat_draw) {
    u8 age = LEVEL_GET_DATA(level, xt, yt) / 10;

    tiles[0] = TILE_M(73 + age, 0, 4);
    tiles[1] = TILE_M(73 + age, 0, 4);
    tiles[2] = TILE_M(73 + age, 1, 4);
    tiles[3] = TILE_M(73 + age, 1, 4);
}

FDRAW(stairs_down_draw) {
    tiles[0] = TILE(44, 2);
    tiles[1] = TILE(45, 2);
    tiles[2] = TILE(46, 2);
    tiles[3] = TILE(47, 2);
}

FDRAW(stairs_up_draw) {
    tiles[0] = TILE(48, 2);
    tiles[1] = TILE(49, 2);
    tiles[2] = TILE(50, 2);
    tiles[3] = TILE(51, 2);
}

FDRAW(infinite_fall_draw) {
    tiles[0] = TILE(32, 0);
    tiles[1] = TILE(32, 0);
    tiles[2] = TILE(32, 0);
    tiles[3] = TILE(32, 0);
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
        tiles[0] = TILE(81 - !ul * 61, 7);
    else
        tiles[0] = TILE(12 + u * 7 + l * 4, 7);

    if(u && r)
        tiles[1] = TILE(80 - !ur * 59, 7);
    else
        tiles[1] = TILE(13 + u * 4 + r * 3, 7);

    if(d && l)
        tiles[2] = TILE(80 - !dl * 58, 7);
    else
        tiles[2] = TILE(14 + d * 5 + l * 4, 7);

    if(d && r)
        tiles[3] = TILE(82 - !dr * 59, 7);
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
        tiles[0] = TILE(0 + !ul * 20, 6);
    else
        tiles[0] = TILE(12 + u * 7 + l * 4, 6);

    if(u && r)
        tiles[1] = TILE(1 + !ur * 20, 6);
    else
        tiles[1] = TILE(13 + u * 4 + r * 3, 6);

    if(d && l)
        tiles[2] = TILE(2 + !dl * 20, 6);
    else
        tiles[2] = TILE(14 + d * 5 + l * 4, 6);

    if(d && r)
        tiles[3] = TILE(3 + !dr * 20, 6);
    else
        tiles[3] = TILE(15 + d * 2 + r * 3, 6);
}

FDRAW(ore_draw) {
    tiles[0] = TILE(52, 3);
    tiles[1] = TILE(53, 3);
    tiles[2] = TILE(54, 3);
    tiles[3] = TILE(55, 3);
}

#define CALL(function)\
    function(level, xt, yt, tiles)

static inline void draw_tile(struct Level *level, u32 xt, u32 yt,
                             u16 tiles[4]) {
    switch(LEVEL_GET_TILE(level, xt, yt)) {
        case GRASS_TILE:
            CALL(grass_draw);
            break;

        case ROCK_TILE:
            CALL(rock_draw);
            break;

        case LIQUID_TILE:
            CALL(liquid_draw);
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

        case SAND_TILE:
            CALL(sand_draw);
            break;

        case CACTUS_TILE:
            CALL(cactus_draw);
            break;

        case HOLE_TILE:
            CALL(hole_draw);
            break;

        case TREE_SAPLING_TILE:
            CALL(tree_sapling_draw);
            break;

        case CACTUS_SAPLING_TILE:
            CALL(cactus_sapling_draw);
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

        case INFINITE_FALL_TILE:
            CALL(infinite_fall_draw);
            break;

        case CLOUD_TILE:
            CALL(cloud_draw);
            break;

        case HARD_ROCK_TILE:
            CALL(hard_rock_draw);
            break;

        // single cases are slightly faster
        case IRON_ORE_TILE:
            CALL(ore_draw);
            break;
        case GOLD_ORE_TILE:
            CALL(ore_draw);
            break;
        case GEM_ORE_TILE:
            CALL(ore_draw);
            break;
        case CLOUD_CACTUS_TILE:
            CALL(ore_draw);
            break;
    }
}

#undef CALL
