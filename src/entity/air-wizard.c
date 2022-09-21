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
#include "air-wizard.h"

#include "mob.h"
#include "player.h"
#include "scene.h"
#include "sound.h"

struct wizard_Data {
    i8 xm : 2;
    i8 ym : 2;

    u8 move_flag : 2;
    u8 attack_type : 2;

    u8 random_walk_time;
};

static_assert(
    sizeof(struct wizard_Data) == 2, "struct wizard_Data: wrong size"
);

// TODO store/load these values
u8 air_wizard_attack_delay = 0;
u8 air_wizard_attack_time  = 0;

void entity_add_air_wizard(struct Level *level) {
    u8 entity_id = level_new_entity(level, AIR_WIZARD_ENTITY);
    if(entity_id >= ENTITY_LIMIT)
        return;

    struct entity_Data *data  = &level->entities[entity_id];
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    data->x = (LEVEL_W << 4) / 2;
    data->y = (LEVEL_W << 4) / 2;

    mob_data->hp = 2000;
    mob_data->dir = 2;
}

ETICK(air_wizard_tick) {
    struct mob_Data    *mob_data    = (struct mob_Data *) &data->data;
    struct wizard_Data *wizard_data = (struct wizard_Data *) &mob_data->data;

    mob_tick(level, data);

    // spin
    if(air_wizard_attack_delay > 0) {
        if(air_wizard_attack_delay < 45)
            mob_data->dir = 2;
        else
            mob_data->dir = 3 - ((air_wizard_attack_delay - 41) / 4) % 4;

        air_wizard_attack_delay--;
        if(air_wizard_attack_delay == 0) {
            air_wizard_attack_time = 2 * 60;
            wizard_data->attack_type = (mob_data->hp < 1000) +
                                       (mob_data->hp < 200);
        }
        return;
    }

    if(air_wizard_attack_time > 0) {
        air_wizard_attack_time--;

        // approximated sin and cos functions: PI = 1024
        u32 angle = air_wizard_attack_time * 81; // 81 = 1024 / (3.14... / 0.25)
        if(air_wizard_attack_time & 1)
            angle = 2048 - (angle % 2048);

        // sin and cos: 0 to 64
        i32 sin = ((angle % 1024) / 64) * (16 - (angle % 1024) / 64);
        if((angle % 2048) > 1024)
            sin *= -1;

        angle += 512; // add half PI
        i32 cos = ((angle % 1024) / 64) * (16 - (angle % 1024) / 64);
        if((angle % 2048) > 1024)
            cos *= -1;

        // speed: 256 = 100%
        u32 speed = 179 + wizard_data->attack_type * 51;
        entity_add_spark(
            level, data->x, data->y,
            cos * speed / 256, sin * speed / 256
        );

        return;
    }

    struct entity_Data *player = &level->entities[0];
    if(player->type < ENTITY_TYPES && wizard_data->random_walk_time == 0) {
        i32 xd = player->x - data->x;
        i32 yd = player->y - data->y;

        u32 dist = xd * xd + yd * yd;

        if(dist < 32 * 32) {
            // run away
            wizard_data->xm = (xd < 0) - (xd > 0);
            wizard_data->ym = (yd < 0) - (yd > 0);
        } else if(dist > 80 * 80) {
            // come closer
            wizard_data->xm = (xd > 0) - (xd < 0);
            wizard_data->ym = (yd > 0) - (yd < 0);
        }
    }

    wizard_data->move_flag++;
    bool move_result = mob_move(
        level, data,
        wizard_data->xm * (wizard_data->move_flag != 0),
        wizard_data->ym * (wizard_data->move_flag != 0)
    );

    if(!move_result || rand() % 100 == 0) {
        wizard_data->random_walk_time = 30;
        wizard_data->xm = (rand() % 3) - 1;
        wizard_data->ym = (rand() % 3) - 1;
    }

    if(wizard_data->random_walk_time > 0) {
        wizard_data->random_walk_time--;

        if(player->type < ENTITY_TYPES && wizard_data->random_walk_time == 0) {
            i32 xd = player->x - data->x;
            i32 yd = player->y - data->y;

            u32 dist = xd * xd + yd * yd;

            if(air_wizard_attack_delay == 0 && air_wizard_attack_time == 0) {
                if(dist < 50 * 50 && rand() % 4 == 0)
                    air_wizard_attack_delay = 2 * 60;
            }
        }
    }
}

EDRAW(air_wizard_draw) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    const u8 dir = mob_data->dir;
    const u8 walk_dist = mob_data->walk_dist;
    const u8 hurt_time = mob_data->hurt_time;

    u16 sprite = 128 + (dir == 0) * 4 + (dir & 1) * 8;
    sprite += (dir & 1) * (
        ((walk_dist >> 3) & 1) * (4 + ((walk_dist >> 4) & 1) * 4)
    );

    static u32 damage_animation = 0;
    damage_animation++;

    u8 palette = 3;
    if(hurt_time > 0) {
        palette = 5;
    } else if(mob_data->hp < 200) {
        if((damage_animation / 3) & 1)
            palette = 4;
    } else if(mob_data->hp < 1000) {
        if((damage_animation / 50) & 1)
            palette = 4;
    }

    u8 flip = ((dir & 1) == 0) * ((walk_dist >> 3) & 1) + (dir == 1);

    SPRITE(
        data->x - 8 - level_x_offset,  // x
        data->y - 11 - level_y_offset, // y
        sprite,  // sprite
        palette, // palette
        flip,    // flip
        0,       // shape
        1,       // size
        0        // disable
    );
}

ETOUCH_PLAYER(air_wizard_touch_player) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;
    mob_hurt(level, player, 3, mob_data->dir);
}

static const struct Entity air_wizard_entity = {
    .tick = air_wizard_tick,
    .draw = air_wizard_draw,

    .xr = 4,
    .yr = 3,

    .is_solid = true,
    .touch_player = air_wizard_touch_player
};

void mob_air_wizard_die(struct Level *level, struct entity_Data *data) {
    struct entity_Data *player = &level->entities[0];
    if(player->type < ENTITY_TYPES) {
        score += 1000;

        player_invulnerable_time = 5 * 60;
        scene_win_timer = 3 * 60;
    }
    SOUND_PLAY(sound_boss_death);
}
