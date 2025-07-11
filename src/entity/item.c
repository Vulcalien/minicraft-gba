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

#include "tile.h"
#include "player.h"
#include "inventory.h"
#include "sound.h"

struct item_entity_Data {
    u16 item_type : 6;
    u16 time : 10;

    // 64 xx = 1 x
    i8 xx;
    i8 xv;

    // 64 yy = 1 y
    i8 yy;
    i8 yv;

    // 6 zz = 1 z
    i16 zz : 10;
    i16 zv : 6;
};

static_assert(
    sizeof(struct item_entity_Data) == 8,
    "struct item_entity_Data: wrong size"
);

void entity_add_item(struct Level *level, u16 x, u16 y,
                     u8 item, bool is_tile) {
    u8 entity_id = level_new_entity(level, ITEM_ENTITY);
    if(entity_id >= ENTITY_LIMIT)
        return;

    struct entity_Data *data = &level->entities[entity_id];
    struct item_entity_Data *item_entity_data =
        (struct item_entity_Data *) &data->data;

    if(is_tile) {
        data->x = (x << 4) + 3 + random(10);
        data->y = (y << 4) + 3 + random(10);
    } else {
        data->x = x - 5 + random(11);
        data->y = y - 5 + random(11);
    }

    item_entity_data->xv = random(59) - 29;
    item_entity_data->yv = random(39) - 19;

    item_entity_data->zz = 12;
    item_entity_data->zv = 6 + random(4);

    item_entity_data->item_type = item;
    item_entity_data->time = 10 * 60 + random(60);

    // use solid_id to store the take delay
    data->solid_id = 30;

    level_add_entity(level, entity_id);
}

ETICK(item_tick) {
    struct item_entity_Data *item_entity_data =
        (struct item_entity_Data *) &data->data;

    item_entity_data->time--;
    if(item_entity_data->time == 0) {
        data->should_remove = true;
        return;
    }

    // use solid_id to store the take delay
    if(data->solid_id > 0)
        data->solid_id--;

    // check if player can take
    struct entity_Data *player = &level->entities[0];
    if(data->solid_id == 0 && player->type < ENTITY_TYPES) {
        const struct Entity *entity = ENTITY_S(data);

        i32 x0 = data->x - entity->xr;
        i32 y0 = data->y - entity->yr;
        i32 x1 = data->x + entity->xr;
        i32 y1 = data->y + entity->yr;

        if(entity_intersects(player, x0, y0, x1, y1)) {
            bool could_add = inventory_add_resource(
                &player_inventory,
                item_entity_data->item_type, 1,
                player_inventory.size
            );

            if(could_add) {
                data->should_remove = true;

                score++;

                SOUND_PLAY(sound_pickup);
            }
        }
    }

    // movement
    item_entity_data->xx += item_entity_data->xv;
    item_entity_data->yy += item_entity_data->yv;

    item_entity_data->zz += item_entity_data->zv;
    if(item_entity_data->zz < 0) {
        item_entity_data->zz = 0;

        item_entity_data->zv /= -2;

        item_entity_data->xv = item_entity_data->xv * 3 / 5;
        item_entity_data->yv = item_entity_data->yv * 3 / 5;
    }
    item_entity_data->zv--;

    entity_move(
        level, data,
        item_entity_data->xx / 64,
        item_entity_data->yy / 64
    );

    item_entity_data->xx %= 64;
    item_entity_data->yy %= 64;
}

EDRAW(item_draw) {
    struct item_entity_Data *item_entity_data =
        (struct item_entity_Data *) &data->data;

    if(item_entity_data->time < 2 * 60)
        if(((item_entity_data->time / 6) & 1) == 0)
            return 0;

    const struct Item *item = &item_list[item_entity_data->item_type];
    const u16 sprite = 256 + item_entity_data->item_type;

    // draw item sprite
    sprite_config(used_sprites, &(struct Sprite) {
        .x = data->x - 4 - level_x_offset,
        .y = data->y - 4 - (item_entity_data->zz / 6) - level_y_offset,

        .priority = 2,

        .size = SPRITE_SIZE_8x8,
        .flip = 0,

        .tile = sprite,
        .palette = 12 + item->palette
    });

    bool should_draw_shadow = (item_entity_data->zz != 0);
    if(should_draw_shadow && used_sprites < 128 - 1) {
        used_sprites++;

        // draw shadow sprite
        sprite_config(used_sprites, &(struct Sprite) {
            .x = data->x - 4 - level_x_offset,
            .y = data->y - 4 - level_y_offset,

            .priority = 2,

            .size = SPRITE_SIZE_8x8,
            .flip = 0,

            .tile = sprite,
            .palette = 7
        });
    }

    return 1 + should_draw_shadow;
}

const struct Entity item_entity = {
    .tick = item_tick,
    .draw = item_draw,

    .xr = 3,
    .yr = 3
};
