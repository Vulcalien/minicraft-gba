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

struct attack_Data {
    u8 dir;
    u8 time;

    u8 unused[6];
};

static_assert(sizeof(struct attack_Data) == 8, "struct attack_Data: wrong size");

void entity_add_attack_particle(struct Level *level, u16 x, u16 y,
                                u8 dir, u8 time) {
    // TODO reuse the same entity if it's already present: there can be only
    // one attack particle at a time

    u8 entity_id = level_new_entity(level, ATTACK_PARTICLE_ENTITY);
    if(entity_id >= ENTITY_LIMIT)
        return;

    struct entity_Data *data        = &level->entities[entity_id];
    struct attack_Data *attack_data = (struct attack_Data *) &data->data;

    data->x = x;
    data->y = y;

    attack_data->dir = dir;
    attack_data->time = time;

    level_add_entity(level, entity_id);
}

ETICK(attack_particle_tick) {
    struct attack_Data *attack_data = (struct attack_Data *) &data->data;

    attack_data->time--;
    if(attack_data->time == 0)
        data->should_remove = true;
}

EDRAW(attack_particle_draw) {
    struct attack_Data *attack_data = (struct attack_Data *) &data->data;

    const u8 dir = attack_data->dir;

    const u16 sprite = 96 + (dir & 1) * 2;
    const u8 palette = 5;

    const u8 size = 1 + (dir & 1);
    const u8 flip = 2 * (dir == 2) + 1 * (dir == 3);

    // TODO adjust position
    sprite_attribs[0] = ((data->y - 8 - level_y_offset) & 0xff) | size << 14;
    sprite_attribs[1] = ((data->x - 8 - level_x_offset) & 0x1ff) |
                        flip << 12 | 0 << 14;
    sprite_attribs[2] = (sprite & 0x3ff) | 2 << 10 | palette << 12;
}

static const struct Entity attack_particle_entity = {
    .tick = attack_particle_tick,
    .draw = attack_particle_draw
};
