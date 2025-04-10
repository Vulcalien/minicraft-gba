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
#include "furniture.h"

#include <gba/sprite.h>

#include "level.h"
#include "item.h"
#include "mob.h"
#include "player.h"

SBSS_SECTION
struct Inventory chest_inventories[CHEST_LIMIT];
u8 chest_count;

u8 chest_opened_id;

struct furniture_Data {
    i8 push_x;
    i8 push_y;

    u8 push_delay;

    u8 chest_id;

    u8 unused[4];
};

static_assert(
    sizeof(struct furniture_Data) == 8,
    "struct furniture_Data: wrong size"
);

THUMB
bool entity_add_furniture(struct Level *level, u8 xt, u8 yt,
                          struct item_Data *item_data) {
    u8 entity_id = level_new_entity(
        level, WORKBENCH_ENTITY + (item_data->type - WORKBENCH_ITEM)
    );
    if(entity_id >= ENTITY_LIMIT)
        return false;

    struct entity_Data *data = &level->entities[entity_id];
    struct furniture_Data *furn_data = (struct furniture_Data *) &data->data;

    data->x = (xt << 4) + 8;
    data->y = (yt << 4) + 8;

    furn_data->chest_id = item_data->chest_id;

    level_add_entity(level, entity_id);
    return true;
}

ETICK(furniture_tick) {
    struct furniture_Data *furn_data = (struct furniture_Data *) &data->data;

    entity_move(level, data, furn_data->push_x, furn_data->push_y);
    furn_data->push_x = 0;
    furn_data->push_y = 0;

    if(furn_data->push_delay > 0)
        furn_data->push_delay--;
}

EDRAW(furniture_draw) {
    sprite_config(used_sprites, &(struct Sprite) {
        .x = data->x - 8 - level_x_offset,
        .y = data->y - 12 - level_y_offset,

        .priority = 2,

        .size = SPRITE_SIZE_16x16,
        .flip = 0,

        .tile = 148 + 4 * (data->type - WORKBENCH_ENTITY),
        .palette = 6
    });
    return 1;
}

ETOUCH_PLAYER(furniture_touch_player) {
    struct furniture_Data *furn_data = (struct furniture_Data *) &data->data;

    if(furn_data->push_delay > 0)
        return;

    struct mob_Data *mob_data = (struct mob_Data *) &player->data;
    const u8 dir = mob_data->dir;

    furn_data->push_x = (dir == 3) - (dir == 1);
    furn_data->push_y = (dir == 2) - (dir == 0);

    furn_data->push_delay = 10;
}

#define GENERATE_STRUCT(name, yr_)\
    const struct Entity name = {\
        .tick = furniture_tick,\
        .draw = furniture_draw,\
\
        .xr = 3,\
        .yr = yr_,\
\
        .is_solid = true,\
        .touch_player = furniture_touch_player\
    }

GENERATE_STRUCT(workbench_entity, 2);
GENERATE_STRUCT(furnace_entity,   2);
GENERATE_STRUCT(oven_entity,      2);
GENERATE_STRUCT(anvil_entity,     2);
GENERATE_STRUCT(chest_entity,     3);
GENERATE_STRUCT(lantern_entity,   2);

THUMB
void furniture_take(struct entity_Data *data) {
    struct furniture_Data *furn_data = (struct furniture_Data *) &data->data;

    player_active_item = (struct item_Data) {
        .type = WORKBENCH_ITEM + (data->type - WORKBENCH_ENTITY),
        .chest_id = furn_data->chest_id
    };
    data->should_remove = true;
}

THUMB
void furniture_set_opened_chest(struct entity_Data *data) {
    struct furniture_Data *furn_data = (struct furniture_Data *) &data->data;

    chest_opened_id = furn_data->chest_id;
}

THUMB
u8 furniture_new_chest_id(void) {
    if(chest_count >= CHEST_LIMIT)
        return -1;

    return chest_count++;
}
