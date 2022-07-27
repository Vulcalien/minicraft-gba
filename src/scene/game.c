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
#include "scene.h"

#include "level.h"
#include "screen.h"
#include "mob.h"
#include "player.h"

struct Level *level;

static void game_init(void) {
    // DEBUG
    current_level = 0;

    level = &levels[current_level];
    level_load(level);

    // FIXME this should not be done here, but in game_draw
    // clear the screen (fully transparent)
    for(u32 y = 0; y < 18; y++)
        for(u32 x = 0; x < 30; x += 2)
            *((vu32 *) &BG3_TILEMAP[x + y * 32]) = 0;

    // TODO is this necessary for level transition or useless?
    // clear two bottom lines
    /*for(u32 x = 10; x < 30; x++) {*/
        /*SET_TILE(x, 18, 29, 1);*/
        /*SET_TILE(x, 19, 29, 1);*/
    /*}*/
}

static void game_tick(void) {
    level_tick(level);
}

#define SET_TILE(x, y, id, palette)\
    do {\
        BG3_TILEMAP[(x) + (y) * 32] = (id) | (palette) << 12;\
    } while(0)
static void game_draw(void) {
    level_draw(level);

    // draw hp and stamina
    if(level->player) {
        struct mob_Data *mob_data = (struct mob_Data *) &level->player->data;

        for(u32 i = 0; i < 10; i++) {
            SET_TILE(i, 18, 91 + (mob_data->hp <= i), 5);
            SET_TILE(i, 19, 93 + (player_stamina <= i), 5);
        }

        if(player_stamina_recharge_delay != 0 &&
           (player_stamina_recharge_delay & 4) == 0) {
            screen_set_bg_palette_color(5, 0xa, 0x7bde);
        } else {
            screen_set_bg_palette_color(5, 0xa, 0x0cc6);
        }

        // draw active item
        if(player_active_item.type < ITEM_TYPES) {
            item_draw_icon(&player_active_item, 20, 18, true);
            item_write(&player_active_item, 0, 21, 18);
        }
    }
}
#undef SET_TILE

const struct Scene scene_game = {
    .init = game_init,

    .tick = game_tick,
    .draw = game_draw
};
