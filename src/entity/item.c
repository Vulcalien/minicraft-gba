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

struct item_entity_Data {
    u8 item_id;

    u8 unused[7];
};

static_assert(
    sizeof(struct item_entity_Data) == 8,
    "struct item_entity_Data: wrong size"
);

ETICK(item_tick) {
}

EDRAW(item_draw) {
}

static const struct Entity item_entity = {
    .tick = item_tick,
    .draw = item_draw,

    .xr = 3,
    .yr = 3
};
