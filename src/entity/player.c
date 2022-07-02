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

#include "mob.h"
#include "input.h"
#include "scene.h"

#define MAX_STAMINA (10)

#define IS_SWIMMING(on_tile)\
    ((on_tile) == WATER_TILE || (on_tile) == LAVA_TILE)

EWRAM_BSS_SECTION
struct Inventory player_inventory;

// TODO should be accessible by GUI
static u8 player_stamina = MAX_STAMINA;
static u8 player_stamina_recharge_delay = 0;

static u8 player_invulnerable_time = 0;
static u8 player_tick_time = 0;

ALWAYS_INLINE
static inline void player_attack(struct Level *level, struct entity_Data *data) {
}

ALWAYS_INLINE
static inline bool player_use(struct Level *level, struct entity_Data *data) {
    return false;
}

ETICK(player_tick) {
    // DEBUG add stuff to inventory
    player_inventory.size = 0;
    for(u32 i = 0; i < ITEM_TYPES; i++) {
        player_inventory.items[player_inventory.size++] = (struct item_Data) {
            .type = i, .tool_level = i % 4
        };
    }

    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    player_tick_time++;

    u8 on_tile = LEVEL_GET_TILE(level, data->x >> 4, data->y >> 4);

    if(on_tile == LAVA_TILE)
        ; // TODO lava damage

    mob_tick(level, data);

    if(player_invulnerable_time > 0)
        player_invulnerable_time--;

    // check if on stairs
    static u8 on_stairs_delay = 0;
    if(on_tile == STAIRS_DOWN_TILE || on_tile == STAIRS_UP_TILE) {
        if(on_stairs_delay == 0) {
            // TODO change level
            on_stairs_delay = 10;
            return;
        }
        on_stairs_delay = 10;
    } else if(on_stairs_delay > 0) {
        on_stairs_delay--;
    }

    // stamina
    static u8 stamina_recharge = 0;

    // TODO ugly code
    if(player_stamina_recharge_delay == 0 &&
       player_stamina == 0 &&
       stamina_recharge == 0)
        player_stamina_recharge_delay = 40;

    if(player_stamina_recharge_delay > 0)
        player_stamina_recharge_delay--;

    if(player_stamina_recharge_delay == 0) {
        stamina_recharge++;

        if(IS_SWIMMING(on_tile))
            stamina_recharge = 0;

        // this fixes what probably is a bug in the original game
        // (Player.java: 78-79)
        if(stamina_recharge == 10) {
            stamina_recharge = 0;

            if(player_stamina < MAX_STAMINA)
                player_stamina++;
        }
    }

    if(IS_SWIMMING(on_tile) && player_tick_time % 60 == 0) {
        if(player_stamina > 0)
            player_stamina--;
        else
            mob_hurt(level, data, 1, mob_data->dir ^ 2);
    }

    // movement
    i32 xm = (INPUT_DOWN(KEY_RIGHT) != 0) - (INPUT_DOWN(KEY_LEFT) != 0);
    i32 ym = (INPUT_DOWN(KEY_DOWN)  != 0) - (INPUT_DOWN(KEY_UP)   != 0);

    if((player_stamina_recharge_delay & 1) == 0) {
        static u8 swim_move_flag = 0;
        if(IS_SWIMMING(on_tile))
            swim_move_flag ^= 1;

        if(!swim_move_flag)
            mob_move(level, data, xm, ym);
    }

    if(player_stamina > 0 && INPUT_CLICKED(KEY_A)) {
        player_stamina--;
        stamina_recharge = 0;

        player_attack(level, data);
    }

    if(INPUT_CLICKED(KEY_B)) {
        if(!player_use(level, data))
            set_scene(&scene_inventory, true);
    }
}

EDRAW(player_draw) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    const u8 dir = mob_data->dir;
    const u8 walk_dist = mob_data->walk_dist;
    const u8 hurt_time = mob_data->hurt_time;

    u8 on_tile = LEVEL_GET_TILE(level, data->x >> 4, data->y >> 4);

    u16 sprite = (dir == 0) * 4 +
                 (dir == 2) * 0 +
                 (dir & 1)  * 8;
    sprite += (dir & 1) * (
        ((walk_dist >> 3) & 1) * (4 + ((walk_dist >> 4) & 1) * 4)
    );
    sprite += IS_SWIMMING(on_tile) * (28 + ((player_tick_time / 8) & 1) * 20);

    u8 palette = 4 + (hurt_time > 0) * 1;

    u8 flip = ((dir & 1) == 0) * ((walk_dist >> 3) & 1) + (dir == 1);

    sprite_attribs[0] = (
        (data->y - 11 + IS_SWIMMING(on_tile) * 4 - level_y_offset) & 0xff
    );
    sprite_attribs[1] = ((data->x - 8  - level_x_offset) & 0x1ff) |
                        (flip << 12) | (1 << 14);
    sprite_attribs[2] = (sprite & 0x3ff) | (0 << 10) | (palette << 12);

}

static const struct Entity player_entity = {
    .tick = player_tick,
    .draw = player_draw,

    .xr = 4,
    .yr = 3,

    .is_solid = true
};

#undef MAX_STAMINA
#undef IS_SWIMMING
