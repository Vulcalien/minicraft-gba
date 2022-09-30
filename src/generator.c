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

        if(l == 3) {
            entity_add_player(level, 2, 2);
            entity_add_air_wizard(level);
        }

        for(u32 i = 1; i < 1000; i++)
            level_try_spawn(level, l);
    }
}
#elif false
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
    entity_add_air_wizard(&levels[4]);
}
#else

static inline i8 get(i8 *values, u32 x, u32 y) {
    if(x >= LEVEL_W || y >= LEVEL_H)
        return 0;
    return values[x + y * LEVEL_W];
}

static inline void set(i8 *values, u32 x, u32 y, i8 val) {
    if(x < LEVEL_W && y < LEVEL_H)
        values[x + y * LEVEL_W] = val;
}

static inline i32 variation(u32 step_size) {
    return ((rand() & (step_size * 4 - 1)) - step_size * 2);
}

IWRAM_SECTION
static i8 *noise(i8 *values, u32 feature_size) {
    for(u32 y = 0; y < LEVEL_H; y += feature_size)
        for(u32 x = 0; x < LEVEL_W; x += feature_size)
            set(values, x, y, rand());

    for(u32 step_size = feature_size; step_size > 1; step_size /= 2) {
        u32 half_step = step_size / 2;

        for(u32 y = 0; y < LEVEL_H; y += step_size) {
            for(u32 x = 0; x < LEVEL_W; x += step_size) {
                //     |     |
                //        f
                //     |     |
                // - - a -h- b
                //     |     |
                //  g  i  e  |
                //     |     |
                // - - c --- d
                i8 a = get(values, x, y);
                i8 b = get(values, x + step_size, y);
                i8 c = get(values, x, y + step_size);
                i8 d = get(values, x + step_size, y + step_size);

                i8 e = (a + b + c + d) / 4 + variation(step_size);

                i8 f = get(values, x + half_step, y - half_step);
                i8 g = get(values, x - half_step, y + half_step);

                i8 h = (a + b + e + f) / 4 + variation(step_size) / 2;
                i8 i = (a + c + e + g) / 4 + variation(step_size) / 2;

                set(values, x + half_step, y + half_step, e);
                set(values, x + half_step, y, h);
                set(values, x, y + half_step, i);
            }
        }
    }
    return values;
}

static inline u32 abs(i32 val) {
    u32 mask = val >> 31;
    return (val ^ mask) + (mask & 1);
}

static inline bool generate_underground(u32 lvl) {
    struct Level *level = &levels[lvl];

    u32 rock_count   = 0;
    u32 dirt_count   = 0;
    u32 ore_count    = 0;
    u32 stairs_count = 0;

    i8 *noise1 = noise((i8 *) &levels[0].data, 32);
    i8 *noise2 = noise((i8 *) &levels[1].data, 32);

    i8 *mnoise1 = noise((i8 *) &levels[2].data, 16);
    i8 *mnoise2 = noise((i8 *) &levels[3].data, 16);
    i8 *mnoise3 = noise((i8 *) &levels[4].data, 16);

    i8 *nnoise1 = noise((i8 *) level_solid_entities + 1 * (LEVEL_W * LEVEL_H), 16);
    i8 *nnoise2 = noise((i8 *) level_solid_entities + 2 * (LEVEL_W * LEVEL_H), 16);
    i8 *nnoise3 = noise((i8 *) level_solid_entities + 3 * (LEVEL_W * LEVEL_H), 16);

    i8 *wnoise3 = noise((i8 *) level_solid_entities + 4 * (LEVEL_W * LEVEL_H), 16);

    for(u32 y = 0; y < LEVEL_H; y++) {
        for(u32 x = 0; x < LEVEL_W; x++) {
            u32 i = x + y * LEVEL_W;

            i32 val = abs(noise1[i] - noise2[i]) * 3 - 256;
            i32 mval = abs(abs(mnoise1[i] - mnoise2[i]) - mnoise3[i]) * 3 - 256;
            i32 nval = abs(abs(nnoise1[i] - nnoise2[i]) - nnoise3[i]) * 3 - 256;
            i32 wval = abs(nval - wnoise3[i]) * 3 - 256;

            // distance from center
            u32 xd = abs(x - LEVEL_W / 2) * 256 / LEVEL_W;
            u32 yd = abs(y - LEVEL_H / 2) * 256 / LEVEL_H;

            u32 dist = (xd >= yd) * xd + (xd < yd) * yd;
            dist = dist * dist * dist * dist / (128 * 128 * 128);
            dist = dist * dist * dist * dist / (128 * 128 * 128);
            val += 128 - dist * 20;

            if(val > -256 && wval < 384 * (lvl != 2) - 256) {
                level->tiles[i] = LIQUID_TILE;
            } else if(val > -256 && (mval < -218 || nval < -179)) {
                level->tiles[i] = DIRT_TILE;
                dirt_count++;
            } else {
                level->tiles[i] = ROCK_TILE;
                rock_count++;
            }
        }
    }

    // add ores
    for(u32 i = 0; i < LEVEL_W * LEVEL_H / 400; i++) {
        // center of the ores
        u32 xc = rand() % LEVEL_W;
        u32 yc = rand() % LEVEL_H;

        for(u32 j = 0; j < 30; j++) {
            i32 xt = xc + rand() % 9 - 4;
            i32 yt = yc + rand() % 9 - 4;

            if(xt >= 2 && xt < LEVEL_W - 2 && yt >= 2 && yt < LEVEL_H - 2) {
                if(level->tiles[xt + yt * LEVEL_W] == ROCK_TILE) {
                    level->tiles[xt + yt * LEVEL_W] = (GEM_ORE_TILE - lvl);

                    ore_count++;
                    rock_count--;
                }
            }
        }
    }

    // add stairs down
    if(lvl != 0) {
        for(u32 i = 0; i < LEVEL_W * LEVEL_H / 100; i++) {
            u32 xt = 10 + rand() % (LEVEL_W - 20);
            u32 yt = 10 + rand() % (LEVEL_H - 20);

            // check if there is rock all around
            for(u32 y = yt - 1; y <= yt + 1; y++)
                for(u32 x = xt - 1; x <= xt + 1; x++)
                    if(level->tiles[x + y * LEVEL_W] != ROCK_TILE)
                        goto continue_stairs_down;

            level->tiles[xt + yt * LEVEL_W] = STAIRS_DOWN_TILE;

            stairs_count++;
            rock_count--;

            if(stairs_count >= 4)
                break;

            continue_stairs_down:;
        }
    }

    return rock_count >= 100 && dirt_count >= 100 && ore_count >= 20 &&
           (stairs_count >= 2 || lvl == 0);
}

static inline bool generate_top(void) {
    struct Level *level = &levels[3];

    u32 rock_count   = 0;
    u32 grass_count  = 0;
    u32 sand_count   = 0;
    u32 tree_count   = 0;
    u32 stairs_count = 0;

    i8 *noise1 = noise((i8 *) &levels[0].data, 32);
    i8 *noise2 = noise((i8 *) &levels[1].data, 32);

    i8 *mnoise1 = noise((i8 *) &levels[2].data, 16);
    i8 *mnoise2 = noise((i8 *) &levels[3].data, 16);
    i8 *mnoise3 = noise((i8 *) &levels[4].data, 16);

    for(u32 y = 0; y < LEVEL_H; y++) {
        for(u32 x = 0; x < LEVEL_W; x++) {
            u32 i = x + y * LEVEL_W;

            i32 val = abs(noise1[i] - noise2[i]) * 3 - 256;
            i32 mval = abs(abs(mnoise1[i] - mnoise2[i]) - mnoise3[i]) * 3 - 256;

            // distance from center
            u32 xd = abs(x - LEVEL_W / 2) * 256 / LEVEL_W;
            u32 yd = abs(y - LEVEL_H / 2) * 256 / LEVEL_H;

            u32 dist = (xd >= yd) * xd + (xd < yd) * yd;
            dist = dist * dist * dist * dist / (128 * 128 * 128);
            dist = dist * dist * dist * dist / (128 * 128 * 128);
            val += 128 - dist * 20;

            if(val < -64) {
                level->tiles[i] = LIQUID_TILE;
            } else if(val > 64 && mval < -192) {
                level->tiles[i] = ROCK_TILE;
                rock_count++;
            } else {
                level->tiles[i] = GRASS_TILE;
                grass_count++;
            }
        }
    }

    // add deserts
    for(u32 i = 0; i < LEVEL_W * LEVEL_H / 2800; i++) {
        // center of the desert
        u32 xc = rand() % LEVEL_W;
        u32 yc = rand() % LEVEL_H;

        for(u32 j = 0; j < 10; j++) {
            i32 xj = xc + rand() % 21 - 10;
            i32 yj = yc + rand() % 21 - 10;

            for(u32 k = 0; k < 85; k++) {
                i32 xk = xj + rand() % 9 - 4;
                i32 yk = yj + rand() % 9 - 4;

                for(i32 yt = yk - 1; yt <= yk + 1; yt++) {
                    if(yt < 0 || yt >= LEVEL_H)
                        continue;

                    for(i32 xt = xk - 1; xt <= xk + 1; xt++) {
                        if(xt < 0 || xt >= LEVEL_W)
                            continue;

                        if(level->tiles[xt + yt * LEVEL_W] == GRASS_TILE) {
                            level->tiles[xt + yt * LEVEL_W] = SAND_TILE;

                            sand_count++;
                            grass_count--;
                        }
                    }
                }
            }
        }
    }

    // add forests
    for(u32 i = 0; i < LEVEL_W * LEVEL_H / 400; i++) {
        // center of the forest
        u32 xc = rand() % LEVEL_W;
        u32 yc = rand() % LEVEL_H;

        for(u32 j = 0; j < 175; j++) {
            i32 xt = xc + rand() % 29 - 14;
            i32 yt = yc + rand() % 29 - 14;

            if(xt >= 0 && xt < LEVEL_W && yt >= 0 && yt < LEVEL_H) {
                if(level->tiles[xt + yt * LEVEL_W] == GRASS_TILE) {
                    level->tiles[xt + yt * LEVEL_W] = TREE_TILE;

                    tree_count++;
                    grass_count--;
                }
            }
        }
    }

    // add flowers
    for(u32 i = 0; i < LEVEL_W * LEVEL_H / 400; i++) {
        // center of the flowers
        u32 xc = rand() % LEVEL_W;
        u32 yc = rand() % LEVEL_H;

        for(u32 j = 0; j < 30; j++) {
            i32 xt = xc + rand() % 9 - 4;
            i32 yt = yc + rand() % 9 - 4;

            if(xt >= 0 && xt < LEVEL_W && yt >= 0 && yt < LEVEL_H) {
                if(level->tiles[xt + yt * LEVEL_W] == GRASS_TILE) {
                    level->tiles[xt + yt * LEVEL_W] = FLOWER_TILE;

                    grass_count--;
                }
            }
        }
    }

    // add cactus
    for(u32 i = 0; i < LEVEL_W * LEVEL_H / 100; i++) {
        u32 xt = rand() % LEVEL_W;
        u32 yt = rand() % LEVEL_H;

        if(level->tiles[xt + yt * LEVEL_W] == SAND_TILE) {
            level->tiles[xt + yt * LEVEL_W] = CACTUS_TILE;

            sand_count--;
        }
    }

    // add stairs down
    for(u32 i = 0; i < LEVEL_W * LEVEL_H / 100; i++) {
        u32 xt = 1 + rand() % (LEVEL_W - 2);
        u32 yt = 1 + rand() % (LEVEL_H - 2);

        // check if there is rock all around
        for(u32 y = yt - 1; y <= yt + 1; y++)
            for(u32 x = xt - 1; x <= xt + 1; x++)
                if(level->tiles[x + y * LEVEL_W] != ROCK_TILE)
                    goto continue_stairs_down;

        level->tiles[xt + yt * LEVEL_W] = STAIRS_DOWN_TILE;

        stairs_count++;
        rock_count--;

        if(stairs_count >= 4)
            break;

        continue_stairs_down:;
    }

    return rock_count >= 100 && grass_count >= 100 &&
           sand_count >= 100 && tree_count  >= 100 &&
           stairs_count >= 2;
}

static inline bool generate_sky(void) {
    struct Level *level = &levels[4];

    u32 cloud_count  = 0;
    u32 stairs_count = 0;

    i8 *noise1 = noise((i8 *) &levels[0].data, 8);
    i8 *noise2 = noise((i8 *) &levels[1].data, 8);

    for(u32 y = 0; y < LEVEL_H; y++) {
        for(u32 x = 0; x < LEVEL_W; x++) {
            u32 i = x + y * LEVEL_W;

            i32 val = abs(noise1[i] - noise2[i]) * 3 - 256;

            // distance from center
            u32 xd = abs(x - LEVEL_W / 2) * 256 / LEVEL_W;
            u32 yd = abs(y - LEVEL_H / 2) * 256 / LEVEL_H;

            u32 dist = (xd >= yd) * xd + (xd < yd) * yd;
            dist = dist * dist * dist * dist / (128 * 128 * 128);
            dist = dist * dist * dist * dist / (128 * 128 * 128);
            val = -val - 282;
            val += 128 - dist * 20;

            if(val < -32) {
                level->tiles[i] = INFINITE_FALL_TILE;
            } else {
                level->tiles[i] = CLOUD_TILE;
                cloud_count++;
            }
        }
    }

    // add cloud cactus
    for(u32 i = 0; i < LEVEL_W * LEVEL_H / 50; i++) {
        u32 xt = 1 + rand() % (LEVEL_W - 2);
        u32 yt = 1 + rand() % (LEVEL_H - 2);

        // check if there is cloud all around
        for(u32 y = yt - 1; y <= yt + 1; y++)
            for(u32 x = xt - 1; x <= xt + 1; x++)
                if(level->tiles[x + y * LEVEL_W] != CLOUD_TILE)
                    goto continue_cloud_cactus;

        level->tiles[xt + yt * LEVEL_W] = CLOUD_CACTUS_TILE;
        cloud_count--;

        continue_cloud_cactus:;
    }

    // add stairs down
    for(u32 i = 0; i < LEVEL_W * LEVEL_H / 100; i++) {
        u32 xt = 1 + rand() % (LEVEL_W - 2);
        u32 yt = 1 + rand() % (LEVEL_H - 2);

        // check if there is cloud all around
        for(u32 y = yt - 1; y <= yt + 1; y++)
            for(u32 x = xt - 1; x <= xt + 1; x++)
                if(level->tiles[x + y * LEVEL_W] != CLOUD_TILE)
                    goto continue_stairs_down;

        level->tiles[xt + yt * LEVEL_W] = STAIRS_DOWN_TILE;

        stairs_count++;
        cloud_count--;

        if(stairs_count >= 4)
            break;

        continue_stairs_down:;
    }

    return cloud_count >= 1750 && stairs_count >= 2;
}

static inline void generate_stairs_up(void) {
    for(u32 l = 0; l < 4; l++) {
        for(u32 i = 0; i < LEVEL_W * LEVEL_H; i++) {
            if(levels[l + 1].tiles[i] == STAIRS_DOWN_TILE) {
                u8 border_tile = l == 3 ? HARD_ROCK_TILE : DIRT_TILE;

                // clear area around
                const u32 xt = i % LEVEL_W;
                const u32 yt = i / LEVEL_W;
                for(u32 y = yt - 1; y <= yt + 1; y++)
                    for(u32 x = xt - 1; x <= xt + 1; x++)
                        levels[l].tiles[x + y * LEVEL_W] = border_tile;

                levels[l].tiles[i] = STAIRS_UP_TILE;
            }
        }
    }
}

static inline void generate_data(void) {
    for(u32 l = 0; l < 5; l++)
        for(u32 i = 0; i < LEVEL_W * LEVEL_H; i++)
            levels[l].data[i] = 0;

    // flower data
    struct Level *top_level = &levels[3];
    for(u32 i = 0; i < LEVEL_W * LEVEL_H; i++) {
        if(top_level->tiles[i] == FLOWER_TILE)
            top_level->data[i] = rand() & 1;
    }
}

static inline void clear_entities(void) {
    for(u32 l = 0; l < 5; l++)
        for(u32 i = 1; i < ENTITY_LIMIT; i++)
            levels[l].entities[i].type = -1;
}

void generate_levels(void) {
    while(!generate_underground(0));
    while(!generate_underground(1));
    while(!generate_underground(2));
    while(!generate_top());
    while(!generate_sky());

    generate_stairs_up();

    generate_data();

    clear_entities();
}

#endif
