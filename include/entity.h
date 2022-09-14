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
#ifndef MINICRAFT_ENTITIES
#define MINICRAFT_ENTITIES

#include "minicraft.h"

#include "level.h"
#include "item.h"

#define ENTITY_TYPES (17)

#define ZOMBIE_ENTITY     (0)
#define SLIME_ENTITY      (1)
#define AIR_WIZARD_ENTITY (2)
#define PLAYER_ENTITY     (3)

#define WORKBENCH_ENTITY (4)
#define FURNACE_ENTITY   (5)
#define OVEN_ENTITY      (6)
#define ANVIL_ENTITY     (7)
#define CHEST_ENTITY     (8)
#define LANTERN_ENTITY   (9)

#define ITEM_ENTITY  (10)
#define SPARK_ENTITY (11)

#define TEXT_PARTICLE_ENTITY      (12)
#define SMASH_PARTICLE_ENTITY     (13)
#define ATTACK_PARTICLE_ENTITY    (14)
#define ITEM_PARTICLE_ENTITY      (15)
#define FURNITURE_PARTICLE_ENTITY (16)

#define ETICK(name)\
    IWRAM_SECTION\
    static void name(struct Level *level, struct entity_Data *data)

#define EDRAW(name)\
    IWRAM_SECTION\
    static void name(struct Level *level, struct entity_Data *data,\
                     vu16 *sprite_attribs)

#define ETOUCH_PLAYER(name)\
    static void name(struct Level *level, struct entity_Data *data,\
                     struct entity_Data *player)

struct Entity {
    void (*tick)(struct Level *level, struct entity_Data *data);

    void (*draw)(struct Level *level, struct entity_Data *data,
                 vu16 *sprite_attribs);

    // entity radius
    u8 xr;
    u8 yr;

    bool is_solid;
    void (*touch_player)(struct Level *level, struct entity_Data *data,
                         struct entity_Data *player);

    bool follow_player_through_levels;
};

#define ENTITY_S(data)\
    (&entity_list[(data)->type])

extern const struct Entity entity_list[ENTITY_TYPES];

#define SPRITE(x, y, sprite, palette, flip, shape, size, disable)\
    do {\
        sprite_attribs[0] = ((y)       & 0xff)       |\
                            ((disable) & 0x1)  << 9  |\
                            ((shape)   & 0x3)  << 14;\
\
        sprite_attribs[1] = ((x)    & 0x1ff)       |\
                            ((flip) & 0x3)   << 12 |\
                            ((size) & 0x3)   << 14;\
\
        sprite_attribs[2] = ((sprite)  & 0x3ff)       |\
                            ((2)       & 0x3)   << 10 |\
                            ((palette) & 0xf)   << 12;\
    } while(0)

extern bool entity_move(struct Level *level, struct entity_Data *data,
                        i32 xm, i32 ym);

extern bool entity_move2(struct Level *level, struct entity_Data *data,
                         i32 xm, i32 ym);

inline bool entity_intersects(struct entity_Data *data,
                              i32 x0, i32 y0, i32 x1, i32 y1) {
    const struct Entity *entity = ENTITY_S(data);

    return (data->x + entity->xr >= x0) && (data->y + entity->yr >= y0) &&
           (data->x - entity->xr <= x1) && (data->y - entity->yr <= y1);
}

// entity generators

extern void entity_add_zombie(struct Level *level, u16 x, u16 y, u8 lvl);
extern void entity_add_slime(struct Level *level, u16 x, u16 y, u8 lvl);
extern void entity_add_player(struct Level *level, u8 xt, u8 yt);

extern bool entity_add_furniture(struct Level *level, u8 xt, u8 yt,
                                 struct item_Data *item_data);

extern void entity_add_item(struct Level *level, u16 x, u16 y,
                            u8 item, bool is_tile);

extern void entity_add_text_particle(struct Level *level, u16 x, u16 y,
                                     u8 number, u8 palette);
extern void entity_add_smash_particle(struct Level *level, u8 xt, u8 yt);
extern void entity_add_attack_particle(struct Level *level, u8 time);
extern void entity_add_item_particle(struct Level *level, u8 time);
extern void entity_add_furniture_particle(struct Level *level);

#endif // MINICRAFT_ENTITIES
