/* Copyright 2022-2023 Vulcalien
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

static struct Level *level = NULL;

static bool should_clear = false;

static inline void game_move_player(struct Level *old_level,
                                    struct Level *new_level) {
    struct entity_Data *new_player = &new_level->entities[0];

    // adjust position
    *new_player = old_level->entities[0];
    u16 old_player_x = new_player->x;
    u16 old_player_y = new_player->y;

    new_player->x = (old_player_x & 0xfff0) + 8;
    new_player->y = (old_player_y & 0xfff0) + 8;
}

static void game_init(u8 flags) {
    if(flags & 2) {
        struct Level *old_level = level;
        level = &levels[current_level];

        // move the player to the new level
        if(old_level && !(flags & 4))
            game_move_player(old_level, level);

        level_load(level);

        vsync();
        screen_update_level_specific();
    }

    should_clear = true;
}

IWRAM_SECTION
static void game_tick(void) {
    if(scene_death_timer > 0) {
        scene_death_timer--;
        if(scene_death_timer == 0)
            set_scene(&scene_death, 1);
    }

    if(scene_win_timer > 0) {
        scene_win_timer--;
        if(scene_win_timer == 0)
            set_scene(&scene_win, 1);
    }

    level_tick(level);
}

static inline void clear_screen(void) {
    if(!should_clear)
        return;

    // clear level area (fully transparent)
    for(u32 y = 0; y < 18; y++)
        for(u32 x = 0; x < 30; x++)
            BG3_TILEMAP[x + y * 32] = 0;

    // clear status bar (black)
    for(u32 y = 18; y < 20; y++)
        for(u32 x = 0; x < 30; x++)
            BG3_TILEMAP[x + y * 32] = 29;

    should_clear = false;
}

static inline void draw_status_bar(void) {
    struct entity_Data *player = &level->entities[0];
    if(player->type >= ENTITY_TYPES)
        return;

    struct mob_Data *mob_data = (struct mob_Data *) &player->data;

    // draw hp and stamina
    for(u32 i = 0; i < 10; i++) {
        BG3_TILEMAP[i + 18 * 32] = (91 + (mob_data->hp <= i))   | 5 << 12;
        BG3_TILEMAP[i + 19 * 32] = (93 + (player_stamina <= i)) | 5 << 12;
    }

    // set stamina blinking color
    if(player_stamina_recharge_delay != 0 &&
       (player_stamina_recharge_delay & 4) == 0) {
        screen_set_bg_palette_color(5, 0xa, 0x7bde);
    } else {
        screen_set_bg_palette_color(5, 0xa, 0x0cc6);
    }

    // clear active item area
    for(u32 x = 20; x < 30; x++)
        BG3_TILEMAP[x + 18 * 32] = 29;

    // draw active item
    if(player_active_item.type < ITEM_TYPES) {
        // copy item palette
        const struct Item *item = ITEM_S(&player_active_item);
        screen_load_active_item_palette(item->palette);

        item_draw_icon(&player_active_item, 20, 18, true);
        item_write(&player_active_item, 0, 21, 18);
    }
}

IWRAM_SECTION
static void game_draw(void) {
    clear_screen();
    level_draw(level);
    draw_status_bar();
}

const struct Scene scene_game = {
    .init = game_init,

    .tick = game_tick,
    .draw = game_draw
};
