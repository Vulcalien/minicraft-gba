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
#include "mob.h"

#include "level.h"
#include "entity.h"

IWRAM_SECTION
void mob_tick(struct Level *level, struct entity_Data *data) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    if(mob_data->hp <= 0)
        ; // TODO die

    if(mob_data->hurt_time > 0)
        mob_data->hurt_time--;
}

IWRAM_SECTION
bool mob_move(struct Level *level, struct entity_Data *data,
              i32 xm, i32 ym) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    if(mob_data->knockback.val > 0) {
        i32 xk = 0;
        i32 yk = 0;

        if     (mob_data->knockback.dir == 0) yk = -6;
        else if(mob_data->knockback.dir == 1) xk = -6;
        else if(mob_data->knockback.dir == 2) yk = +6;
        else if(mob_data->knockback.dir == 3) xk = +6;

        entity_move2(level, data, xk, yk);
        mob_data->knockback.val--;
    }

    if(mob_data->hurt_time > 0)
        return true;

    if(xm != 0 || ym != 0) {
        mob_data->dir = (ym == 0) * ((xm < 0) * 1 + (xm > 0) * 3) +
                                    ((ym < 0) * 0 + (ym > 0) * 2);

        entity_move(level, data, xm, ym);
    }
}
