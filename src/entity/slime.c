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

struct slime_Data {
    i8 xm : 2;
    i8 ym : 2;

    u8 level : 2;

    i8 jump_time;
};

static_assert(sizeof(struct slime_Data) == 2, "struct slime_Data: wrong size");

ETICK(slime_tick) {
    struct mob_Data   *mob_data   = (struct mob_Data *)   &data->data;
    struct slime_Data *slime_data = (struct slime_Data *) &mob_data->data;

    mob_tick(level, data);

    bool move_result = mob_move(
        level, data,
        slime_data->xm,
        slime_data->ym
    );

    if(slime_data->jump_time == -10 && (!move_result || rand() % 40 == 0)) {
        slime_data->xm = (rand() % 3) - 1;
        slime_data->ym = (rand() % 3) - 1;

        if(level->player) {
            i32 xd = level->player->x - data->x;
            i32 yd = level->player->y - data->y;

            if(xd * xd + yd * yd < 50 * 50) {
                if(xd != 0)
                    slime_data->xm = (xd > 0) - (xd < 0);

                if(yd != 0)
                    slime_data->ym = (yd > 0) - (yd < 0);
            }
        }

        if(slime_data->xm != 0 || slime_data->ym != 0)
            slime_data->jump_time = 10;
    }

    if(slime_data->jump_time > -10)
        slime_data->jump_time--;

    if(slime_data->jump_time == 0)
        slime_data->xm = slime_data->ym = 0;
}

EDRAW(slime_draw) {
    struct mob_Data   *mob_data   = (struct mob_Data *)   &data->data;
    struct slime_Data *slime_data = (struct slime_Data *) &mob_data->data;

    u16 sprite = 20 + (slime_data->jump_time > 0) * 4;

    const u8 hurt_time = mob_data->hurt_time;
    u8 palette = (hurt_time > 0)  * 5 +
                 (hurt_time == 0) * slime_data->level;

    sprite_attribs[0] = ((data->y - 11 - level_y_offset) & 0xff);
    sprite_attribs[1] = ((data->x - 8  - level_x_offset) & 0x1ff) |
                        (1 << 14);
    sprite_attribs[2] = (sprite & 0x3ff) | (2 << 10) | (palette << 12);
}

ETOUCH_PLAYER(slime_touch_player) {
    struct mob_Data   *mob_data   = (struct mob_Data *)   &data->data;
    struct slime_Data *slime_data = (struct slime_Data *) &mob_data->data;

    mob_hurt(level, player, 1 + slime_data->level, mob_data->dir);
}

static const struct Entity slime_entity = {
    .tick = slime_tick,
    .draw = slime_draw,

    .xr = 4,
    .yr = 3,

    .is_solid = true,
    .touch_player = slime_touch_player
};

void mob_slime_die(struct Level *level, struct entity_Data *data) {
    u8 drop_count = 1 + rand() % 2;
    for(u32 i = 0; i < drop_count; i++)
        entity_add_item(level, data->x, data->y, SLIME_ITEM, false);

    struct mob_Data   *mob_data   = (struct mob_Data *)   &data->data;
    struct slime_Data *slime_data = (struct slime_Data *) &mob_data->data;

    if(level->player)
        score += 25 * (1 + slime_data->level);
}
