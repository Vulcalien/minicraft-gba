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

#define FTICK(name)\
    static inline void name(struct Level *level, u32 xt, u32 yt)

FTICK(damage_recover_tick) {
    u8 damage = LEVEL_GET_DATA(level, xt, yt);
    if(damage != 0)
        LEVEL_SET_DATA(level, xt, yt, damage - 1);
}

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

FTICK(tree_sapling_tick) {
    u8 age = LEVEL_GET_DATA(level, xt, yt) + 1;

    if(age > 100)
        LEVEL_SET_TILE(level, xt, yt, TREE_TILE, 0);
    else
        LEVEL_SET_DATA(level, xt, yt, age);
}

FTICK(cactus_sapling_tick) {
    u8 age = LEVEL_GET_DATA(level, xt, yt) + 1;

    if(age > 100)
        LEVEL_SET_TILE(level, xt, yt, CACTUS_TILE, 0);
    else
        LEVEL_SET_DATA(level, xt, yt, age);
}

FTICK(farmland_tick) {
    u8 age = LEVEL_GET_DATA(level, xt, yt);
    if(age < 5)
        LEVEL_SET_DATA(level, xt, yt, age + 1);
}

FTICK(wheat_tick) {
    if(rand() & 1)
        return;

    u8 age = LEVEL_GET_DATA(level, xt, yt);
    if(age < 50)
        LEVEL_SET_DATA(level, xt, yt, age + 1);
}

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

#define CALL(function)\
    function(level, xt, yt)

static inline void tick_tile(struct Level *level, u32 xt, u32 yt) {
    switch(LEVEL_GET_TILE(level, xt, yt)) {
        case ROCK_TILE:
        case TREE_TILE:
        case SAND_TILE:
        case CACTUS_TILE:
        case HARD_ROCK_TILE:
            CALL(damage_recover_tick);
            break;

        case GRASS_TILE:
        case FLOWER_TILE:
            CALL(grass_tick);
            break;

        case WATER_TILE:
            CALL(water_tick);
            break;

        case TREE_SAPLING_TILE:
            CALL(tree_sapling_tick);
            break;

        case CACTUS_SAPLING_TILE:
            CALL(cactus_sapling_tick);
            break;

        case FARMLAND_TILE:
            CALL(farmland_tick);
            break;

        case WHEAT_TILE:
            CALL(wheat_tick);
            break;

        case LAVA_TILE:
            CALL(lava_tick);
            break;
    }
}

#undef CALL
