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

ETICK(zombie_tick) {
    struct mob_Data    *mob_data    = (struct mob_Data *)    &data->data;
    struct zombie_Data *zombie_data = (struct zombie_Data *) &mob_data->data;

    mob_tick(level, data);

    if(level->player && zombie_data->random_walk_time == 0) {
        i32 xd = level->player->x - data->x;
        i32 yd = level->player->y - data->y;

        if(xd * xd + yd * yd < 50 * 50) {
            zombie_data->xm = (xd > 0) - (xd < 0);
            zombie_data->ym = (yd > 0) - (yd < 0);
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

    if(zombie_data->random_walk_time)
        zombie_data->random_walk_time--;
}

EDRAW(zombie_draw) {
    struct mob_Data    *mob_data    = (struct mob_Data *)    &data->data;
    struct zombie_Data *zombie_data = (struct zombie_Data *) &mob_data->data;

    const u8 dir = mob_data->dir;
    u16 sprite = (dir == 0) * 4 +
                 (dir == 2) * 0 +
                 (dir & 1)  * 8;

    const u8 walk_dist = mob_data->walk_dist;
    sprite += (dir & 1) * (
        ((walk_dist >> 3) & 1) * (4 + ((walk_dist >> 4) & 1) * 4)
    );

    const u8 hurt_time = mob_data->hurt_time;
    u8 palette = (hurt_time > 0)  * 5 +
                 (hurt_time == 0) * zombie_data->level;

    u8 flip = ((dir & 1) == 0) * ((walk_dist >> 3) & 1) + (dir == 1);

    sprite_attribs[0] = ((data->y - 11 - level_y_offset) & 0xff);
    sprite_attribs[1] = ((data->x - 8  - level_x_offset) & 0x1ff) |
                        (flip << 12) | (1 << 14);
    sprite_attribs[2] = (sprite & 0x3ff) | (0 << 10) | (palette << 12);
}

static const struct Entity zombie_entity = {
    .tick = zombie_tick,
    .draw = zombie_draw,

    .xr = 4,
    .yr = 3
};
