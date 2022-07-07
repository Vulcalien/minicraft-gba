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

struct text_Data {
    u8 time : 6;
    u8 palette : 2;

    u8 number;

    // 64 xx = 1 x
    // 64 yy = 1 y
    //  6 zz = 1 z
    i8 xx;
    i8 yy;
    i8 zz;

    // velocity
    i8 xv;
    i8 yv;
    i8 zv;
};

static_assert(sizeof(struct text_Data) == 8, "struct text_Data: wrong size");

ETICK(text_particle_tick) {
    struct text_Data *text_data = (struct text_Data *) &data->data;

    // TODO put this somethere else
    // without the 'if'
    if(text_data->time == 0) {
        text_data->xv = rand() % 59 - 29;
        text_data->yv = rand() % 39 - 19;

        text_data->zz = 2;
        text_data->zv = 12 + rand() % 4;
    }

    text_data->time++;
    if(text_data->time > 60)
        data->should_remove = true;

    // movement
    text_data->xx += text_data->xv;
    text_data->yy += text_data->yv;

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

    const u16 sprite = 512 + text_data->number * 2 + (length == 1);
    const u8 palette = text_data->palette;

    sprite_attribs[0] = ((data->y - (text_data->zz / 6) - level_y_offset) & 0xff) |
                        ((length == 2) << 14);
    sprite_attribs[1] = ((data->x - length * 4 - level_x_offset) & 0x1ff) |
                        (0 << 14);
    sprite_attribs[2] = (sprite & 0x3ff) | (0 << 10) | (palette << 12);
}

static const struct Entity text_particle_entity = {
    .tick = text_particle_tick,
    .draw = text_particle_draw
};
