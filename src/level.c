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
#include "level.h"

#include <gba/background.h>
#include <gba/sprite.h>

#include "screen.h"
#include "tile.h"
#include "entity.h"
#include "player.h"

#include "tick/tiles.c"

#include "draw/tiles.c"
#include "draw/sort_entities.c"

SBSS_SECTION
struct Level levels[5];

SBSS_SECTION
u8 level_solid_entities[LEVEL_W * LEVEL_H][SOLID_ENTITIES_IN_TILE];

u32 level_x_offset = 0;
u32 level_y_offset = 0;

static u8 entities_render_buffer[128];

static inline void remove_solid_entity(u8 xt, u8 yt,
                                       struct entity_Data *entity_data,
                                       u8 entity_id) {
    const u32 tile = xt + yt * LEVEL_W;
    if(level_solid_entities[tile][entity_data->solid_id] == entity_id)
        level_solid_entities[tile][entity_data->solid_id] = -1;
}

static inline void insert_solid_entity(u8 xt, u8 yt,
                                       struct entity_Data *entity_data,
                                       u8 entity_id) {
    const u32 tile = xt + yt * LEVEL_W;
    for(u32 i = 0; i < SOLID_ENTITIES_IN_TILE; i++) {
        if(level_solid_entities[tile][i] >= ENTITY_LIMIT) {
            level_solid_entities[tile][i] = entity_id;
            entity_data->solid_id = i;

            break;
        }
    }
}

void level_load(struct Level *level) {
    for(u32 t = 0; t < LEVEL_W * LEVEL_H; t++)
        for(u32 i = 0; i < SOLID_ENTITIES_IN_TILE; i++)
            level_solid_entities[t][i] = -1;

    for(u32 i = 0; i < ENTITY_LIMIT; i++) {
        struct entity_Data *data = &level->entities[i];

        if(data->type < ENTITY_TYPES)
            level_add_entity(level, i);
    }
}

static inline void tick_tiles(struct Level *level) {
    for(u32 i = 0; i < LEVEL_W * LEVEL_H / 50; i++) {
        u32 xt = rand() % LEVEL_W;
        u32 yt = rand() % LEVEL_H;

        tick_tile(level, xt, yt);
    }
}

static inline void tick_entities(struct Level *level) {
    for(u32 i = 0; i < ENTITY_LIMIT; i++) {
        struct entity_Data *entity_data = &level->entities[i];
        if(entity_data->type >= ENTITY_TYPES)
            continue;

        u32 xt0 = entity_data->x >> 4;
        u32 yt0 = entity_data->y >> 4;

        const struct Entity *entity = ENTITY_S(entity_data);
        entity->tick(level, entity_data);

        if(entity_data->should_remove) {
            if(entity->is_solid)
                remove_solid_entity(xt0, yt0, entity_data, i);

            entity_data->type = -1;
        } else {
            u32 xt1 = entity_data->x >> 4;
            u32 yt1 = entity_data->y >> 4;

            if(entity->is_solid && (xt1 != xt0 || yt1 != yt0)) {
                remove_solid_entity(xt0, yt0, entity_data, i);
                insert_solid_entity(xt1, yt1, entity_data, i);
            }
        }
    }
}

IWRAM_SECTION
void level_tick(struct Level *level) {
    level_try_spawn(level, current_level);

    tick_tiles(level);
    tick_entities(level);
}

static inline void update_offset(struct Level *level) {
    struct entity_Data *player = &level->entities[0];
    if(player->type < ENTITY_TYPES) {
        i32 x_offset = player->x - SCREEN_W / 2;
        i32 y_offset = player->y - SCREEN_H / 2 + 4;

        if(x_offset < 16) x_offset = 16;
        if(y_offset < 16) y_offset = 16;

        if(x_offset > LEVEL_W * 16 - SCREEN_W - 16)
            x_offset = LEVEL_W * 16 - SCREEN_W - 16;

        if(y_offset > LEVEL_H * 16 - SCREEN_H - 16)
            y_offset = LEVEL_H * 16 - SCREEN_H - 16;

        level_x_offset = x_offset;
        level_y_offset = y_offset;

        // level tiles offset
        background_set_offset(
            BG1,
            level_x_offset & 0xf,
            level_y_offset & 0xf
        );

        // light offset
        background_set_offset(
            BG2,
            level_x_offset & 0xf,
            level_y_offset & 0xf
        );

        // sky background offset
        background_set_offset(
            BG0,
            (level_x_offset >> 2) & 0x7,
            (level_y_offset >> 2) & 0x7
        );
    }
}

static inline void draw_tiles(struct Level *level) {
    const u32 x0 = level_x_offset >> 4;
    const u32 y0 = level_y_offset >> 4;

    for(u32 t = 0; t < 16 * 10; t++) {
        const u32 x = t % 16;
        const u32 y = t / 16;

        u16 tiles[4] = { 0 };

        draw_tile(level, x0 + x, y0 + y, tiles);

        // using 32bit writes instead of 16bit writes saves a little time
        vu32 *tile_0 = (vu32 *) &BG1_TILEMAP[x * 2 + y * 2 * 32];
        *(tile_0)      = (tiles[1] << 16) | tiles[0];
        *(tile_0 + 16) = (tiles[3] << 16) | tiles[2];
    }
}

static inline void draw_lantern_light(struct entity_Data *data) {
    i32 xr = ((data->x + 4) >> 3) - ((level_x_offset >> 3) & ~1);
    i32 yr = ((data->y + 4) >> 3) - ((level_y_offset >> 3) & ~1);

    i32 xl0 = xr - 7;
    i32 yl0 = yr - 7;
    i32 xl1 = xr + 7;
    i32 yl1 = yr + 7;

    if(xl1 < 0 || yl1 < 0)
        return;

    if(xl0 < 0) xl0 = 0;
    if(yl0 < 0) yl0 = 0;
    if(xl1 > 32) xl1 = 32;
    if(yl1 > 20) yl1 = 20;

    for(i32 yl = yl0; yl < yl1; yl++) {
        u32 yd = (yl - yr + (yl >= yr)) * (yl - yr + (yl >= yr));

        for(i32 xl = xl0; xl < xl1; xl++) {
            u32 xd = (xl - xr + (xl >= xr)) * (xl - xr + (xl >= xr));

            u32 val = 256 + (xd + yd <= 66) + (xd + yd <= 46);
            if(BG2_TILEMAP[xl + yl * 32] < val)
                BG2_TILEMAP[xl + yl * 32] = val;
        }
    }
}

static inline void draw_light_sprite(vu16 sprite_attribs[4],
                                     i32 x, i32 y, u32 sprite, bool is_large) {
    sprite_attribs[0] = ((y - level_y_offset) & 0xff) | 2 << 10;
    sprite_attribs[1] = ((x - level_x_offset) & 0x1ff) | (2 + is_large) << 14;
    sprite_attribs[2] = (sprite & 0x3ff) | 2 << 10;
}

static inline void draw_player_light(struct Level *level, u32 *used_sprites) {
    struct entity_Data *player = &level->entities[0];

    if(player_active_item.type == LANTERN_ITEM) {
        // overwrite the last 4 sprites if necessary
        if(*used_sprites > 128 - 4)
            *used_sprites = 128 - 4;
        vu16 *sprite_attribs = OAM + *used_sprites * 4;

        // top-left
        draw_light_sprite(
            &sprite_attribs[0],
            player->x - 64, player->y - 3 - 64,
            336, true
        );

        // top-right
        draw_light_sprite(
            &sprite_attribs[4],
            player->x, player->y - 3 - 64,
            400, true
        );

        // bottom-left
        draw_light_sprite(
            &sprite_attribs[8],
            player->x - 64, player->y - 3,
            464, true
        );

        // bottom-right
        draw_light_sprite(
            &sprite_attribs[12],
            player->x, player->y - 3,
            528, true
        );

        *used_sprites += 4;
    } else {
        // overwrite the last sprite if necessary
        if(*used_sprites > 128 - 1)
            *used_sprites = 128 - 1;

        draw_light_sprite(
            OAM + *used_sprites * 4,
            player->x - 16, player->y - 3 - 16,
            320, false
        );

        *used_sprites += 1;
    }
}

static inline void draw_entities(struct Level *level) {
    u8 *entities_to_render = entities_render_buffer;

    u32 to_render_size = 0;
    for(u32 i = 0; i < ENTITY_LIMIT; i++) {
        struct entity_Data *data = &level->entities[i];
        if(data->type >= ENTITY_TYPES)
            continue;

        // position relative to top-left of the screen
        i32 xr = data->x - level_x_offset;
        i32 yr = data->y - level_y_offset;

        // draw lantern light
        if(level < &levels[3] && data->type == LANTERN_ENTITY)
            draw_lantern_light(data);

        if(xr < -16 || xr >= SCREEN_W + 16 ||
           yr < -16 || yr >= SCREEN_H)
            continue;

        entities_to_render[to_render_size++] = i;
        if(to_render_size == 128)
            break;
    }

    entities_to_render = sort_entities(
        level, entities_to_render, to_render_size
    );

    u32 used_sprites = 0;
    for(u32 i = 0; i < to_render_size && used_sprites < 128; i++) {
        struct entity_Data *data = &level->entities[
            entities_to_render[to_render_size - i - 1]
        ];

        const struct Entity *entity = ENTITY_S(data);
        used_sprites += entity->draw(level, data, used_sprites);
    }

    // draw player light
    if(level < &levels[3])
        draw_player_light(level, &used_sprites);

    // hide remaining sprites
    sprite_hide_range(used_sprites, SPRITE_COUNT);
}

static inline void clear_light(void) {
    for(u32 i = 0; i < 32 * 20; i++)
        BG2_TILEMAP[i] = 256;
}

static inline void draw_lava_light(struct Level *level) {
    const u32 x0 = level_x_offset >> 4;
    const u32 y0 = level_y_offset >> 4;

    for(i32 y = -2; y < 10 + 2; y++) {
        for(i32 x = -2; x < 16 + 2; x++) {
            if(LEVEL_GET_TILE(level, x0 + x, y0 + y) != LIQUID_TILE)
                continue;

            if((x0 + x) & 1 &&
               LEVEL_GET_TILE(level, x0 + x - 1, y0 + y) == LIQUID_TILE &&
               LEVEL_GET_TILE(level, x0 + x + 1, y0 + y) == LIQUID_TILE)
                continue;

            if((y0 + y) & 1 &&
               LEVEL_GET_TILE(level, x0 + x, y0 + y - 1) == LIQUID_TILE &&
               LEVEL_GET_TILE(level, x0 + x, y0 + y + 1) == LIQUID_TILE)
                continue;

            i32 xr = x * 2 + 1;
            i32 yr = y * 2 + 1;

            i32 xl0 = xr - 5;
            i32 yl0 = yr - 5;
            i32 xl1 = xr + 5;
            i32 yl1 = yr + 5;

            if(xl0 < 0) xl0 = 0;
            if(yl0 < 0) yl0 = 0;
            if(xl1 > 32) xl1 = 32;
            if(yl1 > 20) yl1 = 20;

            for(i32 yl = yl0; yl < yl1; yl++) {
                u32 yd = (yl - yr + (yl >= yr)) * (yl - yr + (yl >= yr));

                for(i32 xl = xl0; xl < xl1; xl++) {
                    u32 xd = (xl - xr + (xl >= xr)) * (xl - xr + (xl >= xr));

                    u32 val = 256 + (xd + yd <= 34) + (xd + yd <= 24);
                    if(BG2_TILEMAP[xl + yl * 32] < val)
                        BG2_TILEMAP[xl + yl * 32] = val;
                }
            }
        }
    }
}

IWRAM_SECTION
void level_draw(struct Level *level) {
    update_offset(level);

    if(level < &levels[3])
        clear_light();

    draw_tiles(level);
    draw_entities(level);

    if(level == &levels[0])
        draw_lava_light(level);
}

IWRAM_SECTION
u8 level_new_entity(struct Level *level, u8 type) {
    for(u32 i = 1; i < ENTITY_LIMIT; i++) {
        struct entity_Data *data = &level->entities[i];

        if(data->type >= ENTITY_TYPES) {
            data->type = type;

            // clear entity data
            for(u32 b = 0; b < sizeof(data->data); b++)
                data->data[b] = 0;

            return i;
        }
    }
    return -1;
}

IWRAM_SECTION
void level_add_entity(struct Level *level, u8 entity_id) {
    struct entity_Data *data = &level->entities[entity_id];
    const struct Entity *entity = ENTITY_S(data);

    data->should_remove = false;

    if(entity->is_solid) {
        u32 xt = data->x >> 4;
        u32 yt = data->y >> 4;

        insert_solid_entity(xt, yt, data, entity_id);
    }
}

IWRAM_SECTION
void level_try_spawn(struct Level *level, u8 level_index) {
    u8 xt = rand() % LEVEL_W;
    u8 yt = rand() % LEVEL_H;

    if(LEVEL_GET_TILE_S(level, xt, yt)->is_solid)
        return;

    u16 x = (xt << 4) + 8;
    u16 y = (yt << 4) + 8;

    struct entity_Data *player = &level->entities[0];
    if(player->type < ENTITY_TYPES) {
        i32 xd = player->x - x;
        i32 yd = player->y - y;

        u32 dist = xd * xd + yd * yd;

        if(dist < 80 * 80 || dist >= DESPAWN_DISTANCE)
            return;
    }

    u32 r = ((level_index == 3) ? 8 : 4) * 16;

    i32 x0 = x - r;
    i32 y0 = y - r;
    i32 x1 = x + r;
    i32 y1 = y + r;

    for(u32 i = 0; i < ENTITY_LIMIT; i++) {
        struct entity_Data *e_data = &level->entities[i];
        if(e_data->type >= ENTITY_TYPES)
            continue;

        if(entity_intersects(e_data, x0, y0, x1, y1))
            return;
    }

    u8 entity_level;
    if(level_index == 3)
        entity_level = 0;
    else if(level_index == 4)
        entity_level = 3;
    else
        entity_level = rand() % (4 - level_index);

    if(rand() & 1)
        entity_add_slime(level, x, y, entity_level);
    else
        entity_add_zombie(level, x, y, entity_level);
}
