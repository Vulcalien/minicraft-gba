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

ETICK(player_tick) {
    mob_tick(level, data);

    // DEBUG movement
    i32 xm = 0;
    i32 ym = 0;
    if(INPUT_DOWN(KEY_UP))    ym--;
    if(INPUT_DOWN(KEY_LEFT))  xm--;
    if(INPUT_DOWN(KEY_DOWN))  ym++;
    if(INPUT_DOWN(KEY_RIGHT)) xm++;

    // DEBUG should be mob_move
    entity_move(level, data, xm, ym);

    // TODO ...
}

EDRAW(player_draw) {
    sprite_attribs[0] = ((data->y - 11 - level_y_offset) & 0xff);
    sprite_attribs[1] = ((data->x - 8  - level_x_offset) & 0x1ff) |
                        (1 << 14);
    sprite_attribs[2] = (0 & 0x3ff) | (0 << 10) | (0 << 12);
}

static const struct Entity player_entity = {
    .tick = player_tick,
    .draw = player_draw,

    .xr = 4,
    .yr = 3
};
