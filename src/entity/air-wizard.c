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

ETICK(air_wizard_tick) {
    mob_tick(level, data);

    // TODO ...
}

EDRAW(air_wizard_draw) {
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
