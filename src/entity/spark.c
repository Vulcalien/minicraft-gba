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

    // TODO maybe reduce this to reduce entity count?
    spark_data->time = 60 * 10 + (rand() % 30);

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

    // FIXME can go out of bounds: data->x and y are u16
    // note: is it really a problem if that happens?
    data->x += (spark_data->xx / 64);
    data->y += (spark_data->yy / 64);

    spark_data->xx %= 64;
    spark_data->yy %= 64;

    // TODO damage
}

EDRAW(spark_draw) {
    struct spark_Data *spark_data = (struct spark_Data *) &data->data;

    // TODO test if this is faster:
    //      if(!is_invisible)
    //          sprite = ...
    const u16 sprite = 180 + (rand() % 16) * 2;

    const u8 is_invisible = (spark_data->time < 60 * 2) &&
                            (((spark_data->time / 6) & 1) == 0);

    SPRITE(
        data->x - 4 - level_x_offset, // x
        data->y - 8 - level_y_offset, // y
        sprite,      // sprite
        4,           // palette
        0,           // flip
        2,           // shape
        0,           // size
        is_invisible // disable
    );
}

static const struct Entity spark_entity = {
    .tick = spark_tick,
    .draw = spark_draw
};
