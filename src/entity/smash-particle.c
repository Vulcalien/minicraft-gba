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

#include "sound.h"

struct smash_Data {
    u8 time;

    u8 unused[7];
};

static_assert(sizeof(struct smash_Data) == 8, "struct smash_Data: wrong size");

void entity_add_smash_particle(struct Level *level, u8 xt, u8 yt) {
    SOUND_PLAY(sound_monster_hurt);

    u8 entity_id = level_new_entity(level, SMASH_PARTICLE_ENTITY);
    if(entity_id >= ENTITY_LIMIT)
        return;

    struct entity_Data *data = &level->entities[entity_id];

    data->x = (xt << 4) + 8;
    data->y = (yt << 4) + 8;

    level_add_entity(level, entity_id);
}

ETICK(smash_particle_tick) {
    struct smash_Data *smash_data = (struct smash_Data *) &data->data;

    smash_data->time++;
    if(smash_data->time > 10)
        data->should_remove = true;
}

EDRAW(smash_particle_draw) {
    SPRITE(
        data->x - 8 - level_x_offset, // x
        data->y - 8 - level_y_offset, // y
        152, // sprite
        5,  // palette
        0,  // flip
        0,  // shape
        1,  // size
        0   // disable
    );
}

static const struct Entity smash_particle_entity = {
    .tick = smash_particle_tick,
    .draw = smash_particle_draw
};
