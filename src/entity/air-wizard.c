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

static u32 air_wizard_tick_time = 0;

ETICK(air_wizard_tick) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    air_wizard_tick_time++;
    mob_tick(level, data);

    // TODO ...
}

EDRAW(air_wizard_draw) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    const u8 dir = mob_data->dir;
    const u8 walk_dist = mob_data->walk_dist;
    const u8 hurt_time = mob_data->hurt_time;

    u16 sprite = 128 + (dir == 0) * 4 + (dir & 1) * 8;
    sprite += (dir & 1) * (
        ((walk_dist >> 3) & 1) * (4 + ((walk_dist >> 4) & 1) * 4)
    );

    u8 palette = 3;
    if(hurt_time > 0) {
        palette = 5;
    } else if(mob_data->hp < 200) {
        if((air_wizard_tick_time / 3) & 1)
            palette = 4;
    } else if(mob_data->hp < 1000) {
        if((air_wizard_tick_time / 50) & 1)
            palette = 4;
    }

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

ETOUCH_PLAYER(air_wizard_touch_player) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;
    mob_hurt(level, player, 3, mob_data->dir);
}

static const struct Entity air_wizard_entity = {
    .tick = air_wizard_tick,
    .draw = air_wizard_draw,

    .xr = 4,
    .yr = 3,

    .is_solid = true,
    .touch_player = air_wizard_touch_player
};

void mob_air_wizard_die(struct Level *level, struct entity_Data *data) {
    // TODO air wizard death
}
