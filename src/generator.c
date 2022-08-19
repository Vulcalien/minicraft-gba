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
#include "generator.h"

#include "level.h"
#include "tile.h"
#include "entity.h"

#if false
void generate_levels(void) {
    // DEBUG
    for(u32 l = 0; l < 5; l++) {
        struct Level *level = &levels[l];

        for(u32 t = 0; t < LEVEL_W * LEVEL_H; t++) {
            /*level->tiles[t] = (t / 16 % 2) ? 11 : 2;*/
            /*level->data[t] = 0;*/

            level->tiles[t] = rand() % TILE_TYPES;
            level->data[t] = rand() % 50;

            level->tiles[t] = rand() & 1 ? GRASS_TILE : GRASS_TILE;

            /*level->tiles[t] = 0 + (t % 17 == 0) * 4;*/
            /*level->data[t] = 0;*/
        }

        for(u32 i = 0; i < ENTITY_LIMIT; i++)
            level->entities[i].type = -1;

        if(l == 0)
            entity_add_player(level, 2, 2);

        for(u32 i = 1; i < 1000; i++)
            level_try_spawn(level, l);
    }
}
#else
void generate_levels(void) {
    for(u32 l = 0; l < 5; l++) {
        struct Level *level = &levels[l];

        u8 base_tile;
        if(l < 3)
            base_tile = DIRT_TILE;
        else if(l == 3)
            base_tile = GRASS_TILE;
        else
            base_tile = CLOUD_TILE;

        u8 other_tile;
        if(l < 4)
            other_tile = LIQUID_TILE;
        else
            other_tile = INFINITE_FALL_TILE;

        for(u32 y = 0; y < LEVEL_H; y++) {
            for(u32 x = 0; x < LEVEL_W; x++) {
                u8 other_chance = 250;
                if(y != 0 && level->tiles[(x) + (y - 1) * LEVEL_W] == base_tile)
                    other_chance /= 4;
                if(x != 0 && level->tiles[(x - 1) + (y) * LEVEL_W] == base_tile)
                    other_chance /= 4;

                // do not add water
                if(l == 2)
                    other_chance = 0;

                if(rand() % 256 <= other_chance)
                    level->tiles[x + y * LEVEL_W] = other_tile;
                else
                    level->tiles[x + y * LEVEL_W] = base_tile;
            }
        }

        // add rock
        if(l < 4) {
            for(u32 y = 0; y < LEVEL_H; y++) {
                for(u32 x = 0; x < LEVEL_W; x++) {
                    if(level->tiles[x + y * LEVEL_W] != base_tile)
                        continue;

                    u8 rock_chance = 150 + (l < 3) * 100;
                    if(y != 0 && level->tiles[(x) + (y - 1) * LEVEL_W] == base_tile)
                        rock_chance /= 3;
                    if(x != 0 && level->tiles[(x - 1) + (y) * LEVEL_W] == base_tile)
                        rock_chance /= 3;

                    if(rand() % 256 <= rock_chance)
                        level->tiles[x + y * LEVEL_W] = ROCK_TILE;
                }
            }
        }

        u8 tree_ore_tile;
        if(l == 0)
            tree_ore_tile = GEM_ORE_TILE;
        else if(l == 1)
            tree_ore_tile = GOLD_ORE_TILE;
        else if(l == 2)
            tree_ore_tile = IRON_ORE_TILE;
        else if(l == 3)
            tree_ore_tile = TREE_TILE;
        else
            tree_ore_tile = CLOUD_CACTUS_TILE;

        for(u32 i = 0; i < 3000; i++) {
            u32 t = rand() % (LEVEL_W * LEVEL_H);
            if(level->tiles[t] == base_tile)
                level->tiles[t] = tree_ore_tile;
        }

        if(l == 3) {
            for(u32 i = 0; i < 1000; i++) {
                u32 t = rand() % (LEVEL_W * LEVEL_H);
                if(level->tiles[t] == base_tile)
                    level->tiles[t] = FLOWER_TILE;
            }
        }

        for(u32 i = 0; i < ENTITY_LIMIT; i++)
            level->entities[i].type = -1;
    }

    // stairs
    for(u32 l = 0; l < 4; l++) {
        struct Level *level = &levels[l];
        struct Level *level_up = &levels[l + 1];

        // this might be done using an array, so that duplicate code is avoided
        u8 base_tile;
        if(l < 3)
            base_tile = DIRT_TILE;
        else if(l == 3)
            base_tile = GRASS_TILE;
        else
            base_tile = CLOUD_TILE;

        u32 count = 0;
        while(count < 3) {
            u16 xt = rand() % LEVEL_W;
            u16 yt = rand() % LEVEL_H;

            if(xt < 3 || xt >= LEVEL_W - 3 ||
               yt < 3 || yt >= LEVEL_H - 3)
                continue;

            if(level->tiles[xt + yt * LEVEL_W] != base_tile)
                continue;

            // check if the hard rock would replace a stairs down tile
            if(l == 3) {
                for(u32 y = yt - 1; y <= yt + 1; y++)
                    for(u32 x = xt - 1; x <= xt + 1; x++)
                        if(level->tiles[x + y * LEVEL_W] == STAIRS_DOWN_TILE)
                            goto continue_while;

                for(u32 y = yt - 1; y <= yt + 1; y++)
                    for(u32 x = xt - 1; x <= xt + 1; x++)
                        level->tiles[x + y * LEVEL_W] = HARD_ROCK_TILE;
            }

            level->tiles[xt + yt * LEVEL_W] = STAIRS_UP_TILE;
            level_up->tiles[xt + yt * LEVEL_W] = STAIRS_DOWN_TILE;

            count++;

            continue_while:;
        }
    }

    entity_add_player(&levels[3], 30, 62);
}
#endif
