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
#include "item.h"

#define TILE_TYPES (22)

#define GRASS_TILE          (0)
#define ROCK_TILE           (1)
#define LIQUID_TILE         (2)
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
#define STAIRS_DOWN_TILE    (13)
#define STAIRS_UP_TILE      (14)
#define INFINITE_FALL_TILE  (15)
#define CLOUD_TILE          (16)
#define HARD_ROCK_TILE      (17)
#define IRON_ORE_TILE       (18)
#define GOLD_ORE_TILE       (19)
#define GEM_ORE_TILE        (20)
#define CLOUD_CACTUS_TILE   (21)

struct Tile {
    bool is_solid;
    u8 may_pass;

    u8 touch_damage;

    void (*stepped_on)(struct Level *level, u32 xt, u32 yt,
                       struct entity_Data *entity_data);

    void (*interact)(struct Level *level, u32 xt, u32 yt,
                     struct item_Data *item);

    struct {
        bool grass;
        bool sand;
        bool liquid;
    } connects_to;
};

extern const struct Tile tile_list[TILE_TYPES];

#endif // MINICRAFT_TILE
