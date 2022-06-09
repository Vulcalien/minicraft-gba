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
#ifndef MINICRAFT_TILE
#define MINICRAFT_TILE

#include "minicraft.h"

#include "level.h"

#define GRASS_TILE          (0)
#define ROCK_TILE           (1)
#define WATER_TILE          (2)
#define FLOWER_TILE         (3)
#define TREE_TILE           (4)
#define DIRT_TILE           (5)
#define SAND_TILE           (6)
#define CACTUS_TILE         (7)
#define HOLE_TILE           (8)
#define TREE_SAPLING_TILE   (9)
#define CACTUS_SAPLING_TILE (10)
#define FARMLAND_TILE       (11)
#define WHEAT_TILE          (12)
#define LAVA_TILE           (13)
#define STAIRS_DOWN_TILE    (14)
#define STAIRS_UP_TILE      (15)
#define INFINITE_FALL_TILE  (16)
#define CLOUD_TILE          (17)
#define HARD_ROCK_TILE      (18)
#define IRON_ORE_TILE       (19)
#define GOLD_ORE_TILE       (20)
#define GEM_ORE_TILE        (21)
#define CLOUD_CACTUS_TILE   (22)

struct Tile {
    void (*tick)(struct Level *level, u32 xt, u32 yt);
    void (*draw)(struct Level *level, u32 xt, u32 yt,
                 u16 tiles[4], u16 tiles2[4]);

    struct {
        bool grass;
        bool sand;
        bool liquid;
    } connects_to;
};

#define TILES_COUNT (23)
extern const struct Tile tile_list[TILES_COUNT];

#endif // MINICRAFT_TILE
