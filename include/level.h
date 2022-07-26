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
#ifndef MINICRAFT_LEVEL
#define MINICRAFT_LEVEL

#include "minicraft.h"

#define LEVEL_W (112)
#define LEVEL_H (112)

#define ENTITY_LIMIT (255)

struct entity_Data {
    u8 type;

    u8 should_remove : 1;
    u8 solid_id : 7;

    u16 x;
    u16 y;

    u8 data[8];
};

static_assert(sizeof(struct entity_Data) == 14, "struct entity_Data: wrong size");

struct Level {
    u8 tiles[LEVEL_W * LEVEL_H];
    u8  data[LEVEL_W * LEVEL_H];

    struct entity_Data *player;
    struct entity_Data entities[ENTITY_LIMIT];
};

extern struct Level levels[5];

// TODO might want to increase this
#define SOLID_ENTITIES_IN_TILE (4)
extern u8 level_solid_entities[LEVEL_W * LEVEL_H][SOLID_ENTITIES_IN_TILE];

extern u32 level_x_offset;
extern u32 level_y_offset;

extern void level_tick(struct Level *level);
extern void level_draw(struct Level *level);

#define LEVEL_GET_TILE(level, xt, yt)\
    (((xt) < 0 || (xt) >= LEVEL_W || (yt) < 0 || (yt) >= LEVEL_H) ?\
        ROCK_TILE : level->tiles[(xt) + (yt) * LEVEL_W])
#define LEVEL_SET_TILE(level, xt, yt, val, data_val) do {\
    if((xt) >= 0 && (xt) < LEVEL_W && (yt) >= 0 && (yt) < LEVEL_H) {\
        level->tiles[(xt) + (yt) * LEVEL_W] = val;\
        level->data[(xt) + (yt) * LEVEL_W]  = data_val;\
    }\
} while(0)

// returns 'struct Tile *' instead of the ID
#define LEVEL_GET_TILE_S(level, xt, yt)\
    (TYPE_S(LEVEL_GET_TILE(level, xt, yt)))

#define LEVEL_GET_DATA(level, xt, yt)\
    (((xt) < 0 || (xt) >= LEVEL_W || (yt) < 0 || (yt) >= LEVEL_H) ?\
        0 : level->data[(xt) + (yt) * LEVEL_W])
#define LEVEL_SET_DATA(level, xt, yt, val) do {\
    if((xt) >= 0 && (xt) < LEVEL_W && (yt) >= 0 && (yt) < LEVEL_H)\
        level->data[(xt) + (yt) * LEVEL_W] = val;\
} while(0)

extern u8 level_new_entity(struct Level *level, u8 type);

extern void level_add_entity(struct Level *level, u8 entity_id);

#endif // MINICRAFT_LEVEL
