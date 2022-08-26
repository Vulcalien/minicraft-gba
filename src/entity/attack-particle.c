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
#include "tile.h"

struct attack_Data {
    u8 dir;
    u8 time;

    u8 unused[6];
};

static_assert(sizeof(struct attack_Data) == 8, "struct attack_Data: wrong size");

void entity_add_attack_particle(struct Level *level, u8 time) {
    static u8 entity_id = 0; // entity 0 can never be an attack particle

    if(level->entities[entity_id].type != ATTACK_PARTICLE_ENTITY) {
        entity_id = level_new_entity(level, ATTACK_PARTICLE_ENTITY);
        if(entity_id >= ENTITY_LIMIT)
            return;
    }

    struct entity_Data *data        = &level->entities[entity_id];
    struct attack_Data *attack_data = (struct attack_Data *) &data->data;

    struct entity_Data *player = &level->entities[0];

    attack_data->dir = ((struct mob_Data *) player->data)->dir;
    attack_data->time = time;

    level_add_entity(level, entity_id);
}

ETICK(attack_particle_tick) {
    const struct entity_Data *player = &level->entities[0];

    struct attack_Data *attack_data = (struct attack_Data *) &data->data;

    attack_data->time--;
    if(attack_data->time == 0 || !player) {
        data->should_remove = true;
        return;
    }

    // update position
    const u8 dir = attack_data->dir;

    data->x = player->x + ((dir == 3) - (dir == 1)) * 8;
    data->y = player->y + ((dir == 2) - (dir == 0)) * 8 - 3;

    if(LEVEL_GET_TILE(level, player->x >> 4, player->y >> 4) == LIQUID_TILE)
        data->y += 4;
}

EDRAW(attack_particle_draw) {
    struct attack_Data *attack_data = (struct attack_Data *) &data->data;

    const u8 dir = attack_data->dir;

    const u16 sprite = 96 + (dir & 1) * 2;
    const u8 palette = 5;

    const u8 shape = 1 + (dir & 1);
    const u8 flip  = 2 * (dir == 2) + 1 * (dir == 3);

    // TODO adjust position
    SPRITE(
        data->x - 4 * (1 + ((dir & 1) == 0)) - level_x_offset, // x
        data->y - 4 * (1 + ((dir & 1) == 1)) - level_y_offset, // y
        sprite,  // sprite
        palette, // palette
        flip,    // flip
        shape,   // shape
        0,       // size
        0        // disable
    );
}

static const struct Entity attack_particle_entity = {
    .tick = attack_particle_tick,
    .draw = attack_particle_draw,

    .follow_player_through_levels = true
};
