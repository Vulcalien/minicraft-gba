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

struct slime_Data {
    i8 xm : 2;
    i8 ym : 2;

    u8 level : 2;

    i8 jump_time;
};

static_assert(sizeof(struct slime_Data) == 2, "struct slime_Data: wrong size");

void entity_add_slime(struct Level *level, u16 x, u16 y,
                      u8 lvl, bool add_to_level) {
    u8 entity_id = level_new_entity(level, SLIME_ENTITY);
    if(entity_id >= ENTITY_LIMIT)
        return;

    struct entity_Data *data       = &level->entities[entity_id];
    struct mob_Data    *mob_data   = (struct mob_Data *)   &data->data;
    struct slime_Data  *slime_data = (struct slime_Data *) &mob_data->data;

    data->x = x;
    data->y = y;

    mob_data->hp = 5 * (1 + lvl) * (1 + lvl);

    slime_data->level = lvl;

    if(add_to_level)
        level_add_entity(level, entity_id);
}

ETICK(slime_tick) {
    struct mob_Data   *mob_data   = (struct mob_Data *)   &data->data;
    struct slime_Data *slime_data = (struct slime_Data *) &mob_data->data;

    mob_tick(level, data);

    bool move_result = mob_move(
        level, data,
        slime_data->xm,
        slime_data->ym
    );

    if(slime_data->jump_time == -10 && (!move_result || rand() % 40 == 0)) {
        slime_data->xm = (rand() % 3) - 1;
        slime_data->ym = (rand() % 3) - 1;

        struct entity_Data *player = &level->entities[0];
        if(player->type < ENTITY_TYPES) {
            i32 xd = player->x - data->x;
            i32 yd = player->y - data->y;

            u32 dist = xd * xd + yd * yd;

            if(dist < 50 * 50) {
                if(xd != 0)
                    slime_data->xm = (xd > 0) - (xd < 0);

                if(yd != 0)
                    slime_data->ym = (yd > 0) - (yd < 0);
            } else if(dist >= DESPAWN_DISTANCE) {
                data->should_remove = true;
                return;
            }
        }

        if(slime_data->xm != 0 || slime_data->ym != 0)
            slime_data->jump_time = 10;
    }

    if(slime_data->jump_time > -10)
        slime_data->jump_time--;

    if(slime_data->jump_time == 0)
        slime_data->xm = slime_data->ym = 0;
}

EDRAW(slime_draw) {
    struct mob_Data   *mob_data   = (struct mob_Data *)   &data->data;
    struct slime_Data *slime_data = (struct slime_Data *) &mob_data->data;

    u16 sprite = 0 + (slime_data->jump_time > 0) * 4;

    const u8 hurt_time = mob_data->hurt_time;
    u8 palette = (hurt_time > 0)  * 5 +
                 (hurt_time == 0) * slime_data->level;

    SPRITE(
        data->x - 8  - level_x_offset, // x
        data->y - 11 - level_y_offset, // y
        sprite,  // sprite
        palette, // palette
        0,       // flip
        0,       // shape
        1,       // size
        0        // disable
    );
}

ETOUCH_PLAYER(slime_touch_player) {
    struct mob_Data   *mob_data   = (struct mob_Data *)   &data->data;
    struct slime_Data *slime_data = (struct slime_Data *) &mob_data->data;

    mob_hurt(level, player, 1 + slime_data->level, mob_data->dir);
}

static const struct Entity slime_entity = {
    .tick = slime_tick,
    .draw = slime_draw,

    .xr = 4,
    .yr = 3,

    .is_solid = true,
    .touch_player = slime_touch_player
};

void mob_slime_die(struct Level *level, struct entity_Data *data) {
    u8 drop_count = 1 + rand() % 2;
    for(u32 i = 0; i < drop_count; i++)
        entity_add_item(level, data->x, data->y, SLIME_ITEM, false);

    struct mob_Data   *mob_data   = (struct mob_Data *)   &data->data;
    struct slime_Data *slime_data = (struct slime_Data *) &mob_data->data;

    struct entity_Data *player = &level->entities[0];
    if(player->type < ENTITY_TYPES)
        score += 25 * (1 + slime_data->level);
}
