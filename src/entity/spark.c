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

#include <gba/sprite.h>

#include "mob.h"

struct spark_Data {
    u16 time;

    // 64 xx = 1 x
    // 64 yy = 1 y
    i16 xx;
    i16 yy;

    // velocity
    i8 xv;
    i8 yv;
};

static_assert(sizeof(struct spark_Data) == 8, "struct spark_Data: wrong size");

void entity_add_spark(struct Level *level, u16 x, u16 y,
                      i8 xv, i8 yv) {
    u8 entity_id = level_new_entity(level, SPARK_ENTITY);
    if(entity_id >= ENTITY_LIMIT)
        return;

    struct entity_Data *data = &level->entities[entity_id];
    struct spark_Data *spark_data = (struct spark_Data *) &data->data;

    data->x = x;
    data->y = y;

    spark_data->xv = xv;
    spark_data->yv = yv;

    spark_data->time = 10 * 60 + (rand() % 30);

    level_add_entity(level, entity_id);
}

ETICK(spark_tick) {
    struct spark_Data *spark_data = (struct spark_Data *) &data->data;

    spark_data->time--;
    if(spark_data->time == 0) {
        data->should_remove = true;
        return;
    }

    // movement
    spark_data->xx += spark_data->xv;
    spark_data->yy += spark_data->yv;

    // this can overflow if xx or yy is negative, but it's not a problem
    data->x += (spark_data->xx / 64);
    data->y += (spark_data->yy / 64);

    spark_data->xx %= 64;
    spark_data->yy %= 64;

    // despawn if too far
    struct entity_Data *player = &level->entities[0];
    if(player->type < ENTITY_TYPES) {
        i32 xd = player->x - data->x;
        i32 yd = player->y - data->y;

        u32 dist = xd * xd + yd * yd;
        if(dist >= (20 * 16) * (20 * 16)) {
            data->should_remove = true;
            return;
        }
    }

    // hurt mobs
    const u16 x = data->x;
    const u16 y = data->y;

    u16 xt = (x >> 4);
    u16 yt = (y >> 4);

    if(xt < LEVEL_W && yt < LEVEL_H) {
        u8 *solid_entities = level_solid_entities[xt + yt * LEVEL_W];

        for(u32 i = 0; i < SOLID_ENTITIES_IN_TILE; i++) {
            struct entity_Data *e_data = &level->entities[solid_entities[i]];
            struct mob_Data *mob_data = (struct mob_Data *) &e_data->data;

            switch(e_data->type) {
                case ZOMBIE_ENTITY:
                case SLIME_ENTITY:
                case PLAYER_ENTITY:
                    break;

                default:
                    continue;
            }

            if(entity_intersects(e_data, x, y, x, y))
                mob_hurt(level, e_data, 1, mob_data->dir ^ 2);
        }
    }
}

EDRAW(spark_draw) {
    struct spark_Data *spark_data = (struct spark_Data *) &data->data;

    if(spark_data->time < 2 * 60)
        if(((spark_data->time / 6) & 1) == 0)
            return 0;

    sprite_config(used_sprites, &(struct Sprite) {
        .x = data->x - 4 - level_x_offset,
        .y = data->y - 8 - level_y_offset,

        .priority = 2,

        .shape = 2, // vertical
        .size  = 0, // 8x16
        .flip  = 0,

        .tile = 180 + (rand() % 16) * 2,
        .palette = 4
    });

    return 1;
}

const struct Entity spark_entity = {
    .tick = spark_tick,
    .draw = spark_draw
};
