/* Copyright 2022, 2024 Vulcalien
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

#include "input.h"
#include "screen.h"
#include "level.h"
#include "entity.h"
#include "tile.h"
#include "sound.h"

static u8 death_time;

static void death_init(u8 flags) {
    death_time = 0;
}

static inline void get_spawn_location(struct Level *level,
                                      u32 *spawn_x, u32 *spawn_y) {
    // try to find a grass tile
    for(u32 i = 0; i < 65536; i++) {
        u32 x = rand() % LEVEL_W;
        u32 y = rand() % LEVEL_H;

        u32 tile = LEVEL_GET_TILE(level, x, y);

        if(tile == GRASS_TILE) {
            *spawn_x = x;
            *spawn_y = y;
            return;
        }
    }

    // Try to find any non-solid tile. This cannot fail, because at the
    // very least stairs down are present.
    for(u32 y = 0; y < LEVEL_H; y++) {
        for(u32 x = 0; x < LEVEL_W; x++) {
            const u32 tile_id = LEVEL_GET_TILE(level, x, y);
            const struct Tile *tile = &tile_list[tile_id];

            if(tile->is_solid)
                continue;

            // ignore stairs up: the player could remain stuck inside
            // the surrounding box of hard rock
            if(tile_id == STAIRS_UP_TILE)
                continue;

            *spawn_x = x;
            *spawn_y = y;
            return;
        }
    }
}

static inline void respawn(void) {
    struct Level *level = &levels[3];

    // remove all hostile entities from the level
    for(u32 i = 1; i < ENTITY_LIMIT; i++) {
        struct entity_Data *data = &level->entities[i];
        if(data->type == ZOMBIE_ENTITY || data->type == SLIME_ENTITY)
            data->type = -1;
    }

    // choose a new spawn position
    u32 spawn_x = 0, spawn_y = 0;
    get_spawn_location(level, &spawn_x, &spawn_y);

    entity_add_player(level, spawn_x, spawn_y);

    // TODO keep inventory

    current_level = 3;
}

static void death_tick(void) {
    death_time++;
    if(death_time > 60) {
        if(INPUT_CLICKED(KEY_A) || INPUT_CLICKED(KEY_B)) {
            SOUND_PLAY(sound_start);
            respawn();
            set_scene(&scene_game, 7);
        }
    }
}

static void death_draw(void) {
    const u8 death_x = 5;
    const u8 death_y = 5;
    const u8 death_w = 20;
    const u8 death_h = 7;

    screen_draw_frame("", death_x, death_y, death_w, death_h);
    screen_write("YOU DIED! AWW!", 4, death_x + 1, death_y + 1);

    screen_write("TIME:", 4, death_x + 1, death_y + 2);
    screen_write_time(gametime, 8, death_x + 6, death_y + 2);

    screen_write("SCORE:", 4, death_x + 1, death_y + 3);
    SCREEN_WRITE_NUMBER(score, 10, 10, false, 8, death_x + 7, death_y + 3);

    screen_write("PRESS A TO RESPAWN", 6, death_x + 1, death_y + 5);
}

const struct Scene scene_death = {
    .init = death_init,

    .tick = death_tick,
    .draw = death_draw
};
