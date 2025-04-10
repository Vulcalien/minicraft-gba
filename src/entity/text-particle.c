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
#include <random.h>

struct text_Data {
    u8 time : 6;
    u8 palette : 2;

    u8 number;

    // 64 xx = 1 x
    i8 xx;
    i8 xv;

    // 64 yy = 1 y
    i8 yy;
    i8 yv;

    // 6 zz = 1 z
    i16 zz : 10;
    i16 zv : 6;
};

static_assert(sizeof(struct text_Data) == 8, "struct text_Data: wrong size");

THUMB
void entity_add_text_particle(struct Level *level, u16 x, u16 y,
                              u8 number, u8 palette) {
    u8 entity_id = level_new_entity(level, TEXT_PARTICLE_ENTITY);
    if(entity_id >= ENTITY_LIMIT)
        return;

    struct entity_Data *data = &level->entities[entity_id];
    struct text_Data *text_data = (struct text_Data *) &data->data;

    data->x = x;
    data->y = y;

    text_data->xv = random(59) - 29;
    text_data->yv = random(39) - 19;

    text_data->zz = 12;
    text_data->zv = 12 + random(4);

    text_data->number = number;
    text_data->palette = palette;

    level_add_entity(level, entity_id);
}

ETICK(text_particle_tick) {
    struct text_Data *text_data = (struct text_Data *) &data->data;

    text_data->time++;
    if(text_data->time > 60) {
        data->should_remove = true;
        return;
    }

    // movement
    text_data->xx += text_data->xv;
    text_data->yy += text_data->yv;

    // this can overflow if xx or yy is negative, but it's not a problem
    data->x += (text_data->xx / 64);
    data->y += (text_data->yy / 64);

    text_data->xx %= 64;
    text_data->yy %= 64;

    text_data->zz += text_data->zv;
    if(text_data->zz < 0) {
        text_data->zz = 0;

        text_data->zv /= -2;

        text_data->xv = text_data->xv * 3 / 5;
        text_data->yv = text_data->yv * 3 / 5;
    }
    text_data->zv--;
}

EDRAW(text_particle_draw) {
    struct text_Data *text_data = (struct text_Data *) &data->data;

    const u8 length = 1 + (text_data->number >= 10);

    sprite_config(used_sprites, &(struct Sprite) {
        .x = data->x - length * 4 - level_x_offset,
        .y = data->y - (text_data->zz / 6) - level_y_offset,

        .priority = 2,

        .size = (length == 2 ? SPRITE_SIZE_16x8 : SPRITE_SIZE_8x8),
        .flip = 0,

        .tile = 640 + text_data->number * 2 + (length == 1),
        .palette = 4 + text_data->palette
    });
    return 1;
}

const struct Entity text_particle_entity = {
    .tick = text_particle_tick,
    .draw = text_particle_draw
};
