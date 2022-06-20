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
#include "input.h"

// TODO should be accessible by GUI
static u8 player_stamina;

static u8 player_invulnerable_time = 0;

ETICK(player_tick) {
    u8 on_tile = LEVEL_GET_TILE(level, data->x >> 4, data->y >> 4);

    if(on_tile == LAVA_TILE)
        ; // TODO lava damage

    mob_tick(level, data);

    if(player_invulnerable_time > 0)
        player_invulnerable_time--;

    // check if on stairs
    static u8 on_stairs_delay = 0;
    if(on_tile == STAIRS_DOWN_TILE || on_tile == STAIRS_UP_TILE) {
        if(on_stairs_delay == 0) {
            // TODO change level
            on_stairs_delay = 10;
            return;
        }
        on_stairs_delay = 10;
    } else if(on_stairs_delay > 0) {
        on_stairs_delay--;
    }

    // TODO stamina

    // TODO swimming

    // movement
    i32 xm = (INPUT_DOWN(KEY_RIGHT) != 0) - (INPUT_DOWN(KEY_LEFT) != 0);
    i32 ym = (INPUT_DOWN(KEY_DOWN)  != 0) - (INPUT_DOWN(KEY_UP)   != 0);

    if(true) { // TODO stamina recharge delay
        mob_move(level, data, xm, ym);
    }

    // TODO attack and use
}

EDRAW(player_draw) {
    sprite_attribs[0] = ((data->y - 11 - level_y_offset) & 0xff);
    sprite_attribs[1] = ((data->x - 8  - level_x_offset) & 0x1ff) |
                        (1 << 14);
    sprite_attribs[2] = (0 & 0x3ff) | (0 << 10) | (4 << 12);
}

static const struct Entity player_entity = {
    .tick = player_tick,
    .draw = player_draw,

    .xr = 4,
    .yr = 3
};
