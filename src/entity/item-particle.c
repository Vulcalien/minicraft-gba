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

#include "item.h"
#include "player.h"
#include "mob.h"
#include "tile.h"

struct item_particle_Data {
    u16 sprite;
    u8 palette;

    u8 dir;
    u8 time;

    u8 unused[3];
};

static_assert(
    sizeof(struct item_particle_Data) == 8,
    "struct item_particle_Data: wrong size"
);

void entity_add_item_particle(struct Level *level, u8 time) {
    // if the player has no item, do nothing
    if(player_active_item.type >= ITEM_TYPES)
        return;

    static u8 entity_id = 0; // entity 0 can never be an item particle

    if(level->entities[entity_id].type != ITEM_PARTICLE_ENTITY) {
        entity_id = level_new_entity(level, ITEM_PARTICLE_ENTITY);
        if(entity_id >= ENTITY_LIMIT)
            return;
    }

    struct entity_Data *data = &level->entities[entity_id];
    struct item_particle_Data *item_particle_data =
        (struct item_particle_Data *) &data->data;

    struct entity_Data *player = &level->entities[0];

    item_particle_data->dir = ((struct mob_Data *) player->data)->dir;
    item_particle_data->time = time;

    // set sprite and palette
    const struct Item *item = ITEM_S(&player_active_item);

    item_particle_data->sprite = 256 + player_active_item.type +
        (item->class == ITEMCLASS_TOOL) * (player_active_item.tool_level * 5);
    item_particle_data->palette = 12 + item->palette;

    level_add_entity(level, entity_id);
}

ETICK(item_particle_tick) {
    const struct entity_Data *player = &level->entities[0];

    struct item_particle_Data *item_particle_data =
        (struct item_particle_Data *) &data->data;

    item_particle_data->time--;
    if(item_particle_data->time == 0 || player->type >= ENTITY_TYPES) {
        data->should_remove = true;
        return;
    }

    // update position
    // add 1 to the y so that when the sprite is sorted,
    // it is always on top of the attack particle
    data->x = player->x;
    data->y = player->y + 1;
}

EDRAW(item_particle_draw) {
    struct item_particle_Data *item_particle_data =
        (struct item_particle_Data *) &data->data;

    const u8 dir = item_particle_data->dir;

    u16 x = data->x - 4 + ((dir == 3) - (dir == 1)) * 8;
    u16 y = data->y - 8 + ((dir == 2) - (dir == 0)) * 8;

    const struct entity_Data *player = &level->entities[0];
    if(LEVEL_GET_TILE(level, player->x >> 4, player->y >> 4) == LIQUID_TILE)
        y += 4;

    const u16 sprite = item_particle_data->sprite;
    const u8 palette = item_particle_data->palette;

    SPRITE(
        x - level_x_offset, // x
        y - level_y_offset, // y
        sprite,  // sprite
        palette, // palette
        0,       // flip
        0,       // shape
        0,       // size
        0        // disable
    );

    // TODO remove this file
    return 1;
}

static const struct Entity item_particle_entity = {
    .tick = item_particle_tick,
    .draw = item_particle_draw,

    .follow_player_through_levels = true
};
