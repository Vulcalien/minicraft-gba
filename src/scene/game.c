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

static struct Level *game_level = NULL;

static bool game_should_refresh = false;

static inline void game_move_player(struct Level *old_level,
                                    struct Level *new_level) {
    struct entity_Data *new_player = &new_level->entities[0];

    // FIXME adjusting the player position doesn't move player entities
    *new_player = old_level->entities[0];
    new_player->x = (new_player->x & 0xfff0) + 8;
    new_player->y = (new_player->y & 0xfff0) + 8;

    // move entities that follow player through levels
    for(u32 i = 1; i < ENTITY_LIMIT; i++) {
        struct entity_Data *old_data = &old_level->entities[i];
        if(old_data->type >= ENTITY_TYPES)
            continue;

        const struct Entity *entity = ENTITY_S(old_data);
        if(!entity->follow_player_through_levels)
            continue;

        // find the new entity_id
        for(u32 j = 1; j < ENTITY_LIMIT; j++) {
            struct entity_Data *new_data = &new_level->entities[j];

            if(new_data->type >= ENTITY_TYPES) {
                *new_data = *old_data;
                old_data->type = -1;
                break;

                // since all entities that follow the player are
                // non-solid, do not call level_add_entity
            }
        }
    }
}

static void game_init(void) {
    static u8 last_level = -1;
    if(last_level != current_level) {
        last_level = current_level;

        struct Level *old_level = game_level;
        game_level = &levels[current_level];

        // move the player to the new level
        if(old_level)
            game_move_player(old_level, game_level);

        level_load(game_level);

        vsync();
        screen_update_level_specific();
    }

    if(scene != &scene_transition)
        game_should_refresh = true;
}

static void game_tick(void) {
    level_tick(game_level);
}

static void game_draw(void) {
    if(game_should_refresh) {
        game_should_refresh = false;

        // clear the screen (fully transparent)
        for(u32 y = 0; y < 18; y++)
            for(u32 x = 0; x < 30; x += 2)
                *((vu32 *) &BG3_TILEMAP[x + y * 32]) = 0;
    }

    level_draw(game_level);

    // draw hp and stamina
    struct entity_Data *player = &game_level->entities[0];
    if(player->type < ENTITY_TYPES) {
        struct mob_Data *mob_data = (struct mob_Data *) &player->data;

        for(u32 i = 0; i < 10; i++) {
            BG3_TILEMAP[i + 18 * 32] = (91 + (mob_data->hp <= i))   | 5 << 12;
            BG3_TILEMAP[i + 19 * 32] = (93 + (player_stamina <= i)) | 5 << 12;
        }

        if(player_stamina_recharge_delay != 0 &&
           (player_stamina_recharge_delay & 4) == 0) {
            screen_set_bg_palette_color(5, 0xa, 0x7bde);
        } else {
            screen_set_bg_palette_color(5, 0xa, 0x0cc6);
        }

        // clear active item area
        for(u32 x = 20; x < 30; x += 2)
            *((vu32 *) &BG3_TILEMAP[x + 18 * 32]) = 29 | 29 << 16;

        // draw active item
        if(player_active_item.type < ITEM_TYPES) {
            // copy item palette
            const struct Item *item = ITEM_S(&player_active_item);
            screen_load_active_item_palette(item->palette);

            item_draw_icon(&player_active_item, 20, 18, true);
            item_write(&player_active_item, 0, 21, 18);
        }
    }
}

const struct Scene scene_game = {
    .init = game_init,

    .tick = game_tick,
    .draw = game_draw
};
