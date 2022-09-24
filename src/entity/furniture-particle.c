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

#include "player.h"
#include "item.h"

static bool furniture_particle_last_removed = true;

void entity_add_furniture_particle(struct Level *level) {
    if(!furniture_particle_last_removed)
        return;

    u8 entity_id = level_new_entity(level, FURNITURE_PARTICLE_ENTITY);
    if(entity_id >= ENTITY_LIMIT)
        return;

    furniture_particle_last_removed = false;
    level_add_entity(level, entity_id);
}

ETICK(furniture_particle_tick) {
    const struct entity_Data *player = &level->entities[0];

    if(player->type >= ENTITY_TYPES ||
       player_active_item.type >= ITEM_TYPES ||
       ITEM_S(&player_active_item)->class != ITEMCLASS_FURNITURE) {
        data->should_remove = true;
        furniture_particle_last_removed = true;
        return;
    }

    // update position
    // add 1 to the y so that when the sprite is
    // sorted, it is always on top of the player
    data->x = player->x;
    data->y = player->y + 1;

    if(LEVEL_GET_TILE(level, player->x >> 4, player->y >> 4) == LIQUID_TILE)
        data->y += 4;
}

EDRAW(furniture_particle_draw) {
    const u16 sprite = 148 + 4 * (player_active_item.type - WORKBENCH_ITEM);

    SPRITE(
        data->x - 8  - level_x_offset, // x
        data->y - 24 - level_y_offset, // y
        sprite, // sprite
        6,      // palette
        0,      // flip
        0,      // shape
        1,      // size
        0       // disable
    );
}

static const struct Entity furniture_particle_entity = {
    .tick = furniture_particle_tick,
    .draw = furniture_particle_draw,

    .follow_player_through_levels = true
};
