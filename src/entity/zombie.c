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
#include "entity.h"

#include "mob.h"

struct zombie_Data {
    i8 xm : 2;
    i8 ym : 2;

    u8 level : 2;
    u8 random_walk_time : 6;
    u8 move_flag : 1;
};

static_assert(sizeof(struct zombie_Data) == 2, "struct zombie_Data: wrong size");

void entity_add_zombie(struct Level *level, u16 x, u16 y,
                       u8 lvl, bool add_to_level) {
    u8 entity_id = level_new_entity(level, ZOMBIE_ENTITY);
    if(entity_id >= ENTITY_LIMIT)
        return;

    struct entity_Data *data        = &level->entities[entity_id];
    struct mob_Data    *mob_data    = (struct mob_Data *)    &data->data;
    struct zombie_Data *zombie_data = (struct zombie_Data *) &mob_data->data;

    data->x = x;
    data->y = y;

    mob_data->hp = 10 * (1 + lvl) * (1 + lvl);
    mob_data->dir = 2;

    zombie_data->level = lvl;

    if(add_to_level)
        level_add_entity(level, entity_id);
}

ETICK(zombie_tick) {
    struct mob_Data    *mob_data    = (struct mob_Data *)    &data->data;
    struct zombie_Data *zombie_data = (struct zombie_Data *) &mob_data->data;

    mob_tick(level, data);

    struct entity_Data *player = &level->entities[0];
    if(player->type < ENTITY_TYPES && zombie_data->random_walk_time == 0) {
        i32 xd = player->x - data->x;
        i32 yd = player->y - data->y;

        u32 dist = xd * xd + yd * yd;

        if(dist < 50 * 50) {
            zombie_data->xm = (xd > 0) - (xd < 0);
            zombie_data->ym = (yd > 0) - (yd < 0);
        } else if(dist >= DESPAWN_DISTANCE) {
            data->should_remove = true;
            return;
        }
    }

    zombie_data->move_flag ^= 1;
    bool move_result = mob_move(
        level, data,
        zombie_data->xm * zombie_data->move_flag,
        zombie_data->ym * zombie_data->move_flag
    );

    if(!move_result || rand() % 200 == 0) {
        zombie_data->random_walk_time = 60;
        zombie_data->xm = (rand() & 1) * ((rand() % 3) - 1);
        zombie_data->ym = (rand() & 1) * ((rand() % 3) - 1);
    }

    if(zombie_data->random_walk_time > 0)
        zombie_data->random_walk_time--;
}

EDRAW(zombie_draw) {
    struct mob_Data    *mob_data    = (struct mob_Data *)    &data->data;
    struct zombie_Data *zombie_data = (struct zombie_Data *) &mob_data->data;

    const u8 dir = mob_data->dir;
    const u8 walk_dist = mob_data->walk_dist;
    const u8 hurt_time = mob_data->hurt_time;

    u16 sprite = (dir == 0) * 4 +
                 (dir == 2) * 0 +
                 (dir & 1)  * 8;

    sprite += (dir & 1) * (
        ((walk_dist >> 3) & 1) * (4 + ((walk_dist >> 4) & 1) * 4)
    );

    u8 palette = (hurt_time > 0)  * 5 +
                 (hurt_time == 0) * zombie_data->level;

    u8 flip = ((dir & 1) == 0) * ((walk_dist >> 3) & 1) + (dir == 1);

    SPRITE(
        data->x - 8 - level_x_offset,  // x
        data->y - 11 - level_y_offset, // y
        sprite,  // sprite
        palette, // palette
        flip,    // flip
        0,       // shape
        1,       // size
        0        // disable
    );
}

ETOUCH_PLAYER(zombie_touch_player) {
    struct mob_Data    *mob_data    = (struct mob_Data *)    &data->data;
    struct zombie_Data *zombie_data = (struct zombie_Data *) &mob_data->data;

    mob_hurt(level, player, 2 + zombie_data->level, mob_data->dir);
}

static const struct Entity zombie_entity = {
    .tick = zombie_tick,
    .draw = zombie_draw,

    .xr = 4,
    .yr = 3,

    .is_solid = true,
    .touch_player = zombie_touch_player
};

void mob_zombie_die(struct Level *level, struct entity_Data *data) {
    u8 drop_count = 1 + rand() % 2;
    for(u32 i = 0; i < drop_count; i++)
        entity_add_item(level, data->x, data->y, CLOTH_ITEM, false);

    struct mob_Data    *mob_data    = (struct mob_Data *)    &data->data;
    struct zombie_Data *zombie_data = (struct zombie_Data *) &mob_data->data;

    struct entity_Data *player = &level->entities[0];
    if(player->type < ENTITY_TYPES)
        score += 50 * (1 + zombie_data->level);
}
