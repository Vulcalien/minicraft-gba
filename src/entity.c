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
        u32 xt = data->x / 16;
        u32 yt = data->y / 16;

        const struct Tile *tile = LEVEL_GET_TILE_S(level, xt, yt);
        if(tile->stepped_on)
            tile->stepped_on(level, xt, yt, data->type);
    }

    return !stopped;
}

IWRAM_SECTION
bool entity_move2(struct Level *level, struct entity_Data *data,
                  i32 xm, i32 ym) {
    struct Entity *entity = ENTITY_S(data);

    // current position
    i32 xto0 = (data->x - entity->xr) / 16;
    i32 yto0 = (data->y - entity->yr) / 16;
    i32 xto1 = (data->x + entity->xr) / 16;
    i32 yto1 = (data->y + entity->yr) / 16;

    // position after moving
    i32 xt0 = (data->x + xm - entity->xr) / 16;
    i32 yt0 = (data->y + ym - entity->yr) / 16;
    i32 xt1 = (data->x + xm + entity->xr) / 16;
    i32 yt1 = (data->y + ym + entity->yr) / 16;

    // TODO optimize this if possible
    for(i32 yt = yt0; yt <= yt1; yt++) {
        for(i32 xt = xt0; xt <= xt1; xt++) {
            // ignore the tiles the entity already is in
            if(xt >= xto0 && xt <= xto1 &&
               yt >= yto0 && yt <= yto1)
                continue;

            const struct Tile *tile = LEVEL_GET_TILE_S(level, xt, yt);
            if(tile->bumped_into)
                tile->bumped_into(level, xt, yt, data);

            if(tile->is_solid && tile->may_pass != data->type)
                return false;
        }
    }

    // TODO collision with other entities

    data->x += xm;
    data->y += ym;
    return true;
}
