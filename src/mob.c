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
#include "mob.h"

#include "level.h"
#include "entity.h"
#include "player.h"
#include "item.h"

static inline void mob_die(struct Level *level, struct entity_Data *data) {
    switch(data->type) {
        case ZOMBIE_ENTITY:
            mob_zombie_die(level, data);
            break;

        case SLIME_ENTITY:
            mob_slime_die(level, data);
            break;

        case AIR_WIZARD_ENTITY:
            mob_air_wizard_die(level, data);
            break;

        case PLAYER_ENTITY:
            mob_player_die(level, data);
            break;
    }
    data->should_remove = true;
}

IWRAM_SECTION
void mob_tick(struct Level *level, struct entity_Data *data) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    if(mob_data->hp <= 0)
        mob_die(level, data);

    if(mob_data->hurt_time > 0)
        mob_data->hurt_time--;
}

IWRAM_SECTION
bool mob_move(struct Level *level, struct entity_Data *data,
              i32 xm, i32 ym) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    if(mob_data->knockback.val > 0) {
        i32 xk = 0;
        i32 yk = 0;

        if     (mob_data->knockback.dir == 0) yk--;
        else if(mob_data->knockback.dir == 1) xk--;
        else if(mob_data->knockback.dir == 2) yk++;
        else if(mob_data->knockback.dir == 3) xk++;

        entity_move2(level, data, xk, yk);
        mob_data->knockback.val--;
    }

    if(mob_data->hurt_time > 0)
        return true;

    if(xm != 0 || ym != 0) {
        mob_data->dir = (ym == 0) * ((xm < 0) * 1 + (xm > 0) * 3) +
                                    ((ym < 0) * 0 + (ym > 0) * 2);

        mob_data->walk_dist++;
    }

    return entity_move(level, data, xm, ym);
}

IWRAM_SECTION
void mob_hurt(struct Level *level, struct entity_Data *data,
              u8 damage, u8 knockback_dir) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    if(mob_data->hurt_time > 0)
        return;

    if(data->type == PLAYER_ENTITY && player_invulnerable_time > 0)
        return;

    mob_data->hp = (mob_data->hp - damage) * (mob_data->hp >= damage);

    mob_data->hurt_time = 10;

    mob_data->knockback.val = 6;
    mob_data->knockback.dir = knockback_dir;

    if(data->type == PLAYER_ENTITY) {
        player_invulnerable_time = 30;

        entity_add_text_particle(level, data->x, data->y, damage, 1);
        // TODO player hurt sound
    } else {
        entity_add_text_particle(level, data->x, data->y, damage, 0);

        struct entity_Data *player = &level->entities[0];
        if(player < ENTITY_TYPES) {
            i32 xd = player->x - data->x;
            i32 yd = player->y - data->y;

            if(xd * xd + yd * yd < 80 * 80)
                ; // TODO mob hurt sound
        }
    }
}
