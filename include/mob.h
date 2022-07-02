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
#ifndef MINICRAFT_MOB
#define MINICRAFT_MOB

#include "minicraft.h"

#include "level.h"
#include "entity.h"

struct mob_Data {
    u16 hp;
    u8 dir;

    struct {
        u8 val : 6;
        u8 dir : 2;
    } knockback;

    u8 walk_dist;
    u8 hurt_time;

    u8 data[2];
};

static_assert(sizeof(struct mob_Data) == 8, "struct mob_Data: wrong size");

extern void mob_tick(struct Level *level, struct entity_Data *data);

extern bool mob_move(struct Level *level, struct entity_Data *data,
                     i32 xm, i32 ym);

extern void mob_hurt(struct Level *level, struct entity_Data *data,
                     u8 damage, u8 knockback_dir);

#endif // MINICRAFT_MOB
