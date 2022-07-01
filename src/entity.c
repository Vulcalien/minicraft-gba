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

#include "level.h"
#include "tile.h"

#include "entity/zombie.c"
#include "entity/slime.c"
#include "entity/air-wizard.c"
#include "entity/player.c"

IWRAM_RODATA_SECTION
const struct Entity entity_list[ENTITY_TYPES] = {
    zombie_entity,
    slime_entity,
    air_wizard_entity,
    player_entity,
    /*
    text_particle_entity,
    smash_particle_entity,
    spark_entity,
    furniture_entity,
    lantern_entity,
    item_entity
    */
};

// COMPATIBILITY NOTE
// 'x >> n' is assumed to be a floor division by 2^n
// even for negative numbers: this is implementation-dependent

IWRAM_SECTION
bool entity_move(struct Level *level, struct entity_Data *data,
                 i32 xm, i32 ym) {
    if(xm == 0 && ym == 0)
        return true;

    // TODO surely this logic can be improved
    bool stopped = true;
    if(xm != 0 && entity_move2(level, data, xm, 0))
        stopped = false;
    if(ym != 0 && entity_move2(level, data, 0, ym))
        stopped = false;

    if(!stopped) {
        i32 xt = data->x >> 4;
        i32 yt = data->y >> 4;

        const struct Tile *tile = LEVEL_GET_TILE_S(level, xt, yt);
        if(tile->stepped_on)
            tile->stepped_on(level, xt, yt, data);
    }

    return !stopped;
}

IWRAM_SECTION
bool entity_move2(struct Level *level, struct entity_Data *data,
                  i32 xm, i32 ym) {
    const struct Entity *entity = ENTITY_S(data);

    u32 tiles_to_check;
    i32 tiles[2][2];

    i32 xto0 = (data->x - entity->xr) >> 4;
    i32 yto0 = (data->y - entity->yr) >> 4;
    i32 xto1 = (data->x + entity->xr) >> 4;
    i32 yto1 = (data->y + entity->yr) >> 4;

    i32 xt0 = (data->x + xm - entity->xr) >> 4;
    i32 yt0 = (data->y + ym - entity->yr) >> 4;
    i32 xt1 = (data->x + xm + entity->xr) >> 4;
    i32 yt1 = (data->y + ym + entity->yr) >> 4;

    if(xm < 0) {
        tiles[0][0] = xt0; tiles[0][1] = yt0;
        tiles[1][0] = xt0; tiles[1][1] = yt1;

        tiles_to_check = (xt0 != xto0) * (1 + (yt0 != yt1));
    } else if(xm > 0) {
        tiles[0][0] = xt1; tiles[0][1] = yt0;
        tiles[1][0] = xt1; tiles[1][1] = yt1;

        tiles_to_check = (xt1 != xto1) * (1 + (yt0 != yt1));
    } else if(ym < 0) {
        tiles[0][0] = xt0; tiles[0][1] = yt0;
        tiles[1][0] = xt1; tiles[1][1] = yt0;

        tiles_to_check = (yt0 != yto0) * (1 + (xt0 != xt1));
    } else {
        tiles[0][0] = xt0; tiles[0][1] = yt1;
        tiles[1][0] = xt1; tiles[1][1] = yt1;

        tiles_to_check = (yt1 != yto1) * (1 + (xt0 != xt1));
    }

    for(u32 i = 0; i < tiles_to_check; i++) {
        const i32 *t = tiles[i];

        const struct Tile *tile = LEVEL_GET_TILE_S(level, t[0], t[1]);
        if(tile->bumped_into)
            tile->bumped_into(level, t[0], t[1], data);

        if(tile->is_solid && tile->may_pass != data->type)
            return false;
    }

    // TODO try to optimize this as much as possible
    // FIXME sometimes, entities can pass anyway

    // solid entity collision
    bool blocked_by_entity = false;
    for(u32 yt = (yt0 - 1) * (yt0 - 1 >= 0); yt <= (yt1 + 1); yt++) {
        for(u32 xt = (xt0 - 1) * (xt0 - 1 >= 0); xt <= (xt1 + 1); xt++) {
            const u32 tile = xt + yt * LEVEL_W;

            for(u32 i = 0; i < SOLID_ENTITIES_IN_TILE; i++) {
                const u8 entity_id = level_solid_entities[tile][i];
                if(entity_id >= ENTITY_CAP)
                    continue;

                struct entity_Data *e_data = &level->entities[entity_id];

                if(e_data == data)
                    continue;

                if(entity_intersects(
                    e_data,
                    data->x - entity->xr + xm, data->y - entity->yr + ym,
                    data->x + entity->xr + xm, data->y + entity->yr + ym
                )) {
                    blocked_by_entity = true;

                    // TODO touch entity
                }
            }
        }
    }

    if(blocked_by_entity)
        return false;

    data->x += xm;
    data->y += ym;
    return true;
}
