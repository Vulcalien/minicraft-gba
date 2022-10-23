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

struct item_shadow_Data {
    struct entity_Data *item_entity;

    u8 item_type;

    u8 unused[3];
};

static_assert(
    sizeof(struct item_shadow_Data) == 8,
    "struct item_shadow_Data: wrong size"
);

void entity_add_item_shadow(struct Level *level,
                            struct entity_Data *item_entity, u8 item) {
    u8 entity_id = level_new_entity(level, ITEM_SHADOW_ENTITY);
    if(entity_id >= ENTITY_LIMIT)
        return;

    struct entity_Data *data = &level->entities[entity_id];
    struct item_shadow_Data *item_shadow_data =
        (struct item_shadow_Data *) &data->data;

    item_shadow_data->item_entity = item_entity;
    item_shadow_data->item_type = item;

    level_add_entity(level, entity_id);
}

ETICK(item_shadow_tick) {
    struct item_shadow_Data *item_shadow_data =
        (struct item_shadow_Data *) &data->data;

    struct entity_Data *item_entity_data = item_shadow_data->item_entity;

    // doesn't work and might be buggy:
    // what if the item is removed, and a mob dies
    // and drops an item with the same entity_id?
    /*if(item_entity_data->type != ITEM_ENTITY) {*/
        /*data->should_remove = true;*/
    /*}*/

    // TODO put a despawn time or something similar

    // update position
    // subtract 1 to the y so that when the sprite
    // is sorted, it is always down the item
    data->x = item_entity_data->x;
    data->y = item_entity_data->y - 1;
}

EDRAW(item_shadow_draw) {
    struct item_shadow_Data *item_shadow_data =
        (struct item_shadow_Data *) &data->data;

    const u16 sprite = 256 + item_shadow_data->item_type;

    SPRITE(
        data->x - 4 - level_x_offset, // x
        data->y - 3 - level_y_offset, // y
        sprite, // sprite
        7,      // palette
        0,      // flip
        0,      // shape
        0,      // size
        0       // disable TODO it needs to, unless removed before
    );
}

static const struct Entity item_shadow_entity = {
    .tick = item_shadow_tick,
    .draw = item_shadow_draw
};
