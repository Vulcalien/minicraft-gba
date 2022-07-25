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
#include "level.h"

#include "screen.h"
#include "tile.h"
#include "entity.h"
#include "player.h"

#include "tick/tiles.c"

#include "draw/tiles.c"

EWRAM_BSS_SECTION
struct Level levels[5];

EWRAM_BSS_SECTION
u8 level_solid_entities[LEVEL_W * LEVEL_H][SOLID_ENTITIES_IN_TILE];

u32 level_x_offset = 0;
u32 level_y_offset = 0;

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
        if(level_solid_entities[tile][i] >= ENTITY_CAP) {
            level_solid_entities[tile][i] = entity_id;
            entity_data->solid_id = i;

            break;
        }
    }
}

IWRAM_SECTION
void level_tick(struct Level *level) {
    // TODO try spawn

    for(u32 i = 0; i < LEVEL_W * LEVEL_H / 50; i++) {
        u32 xt = rand() % LEVEL_W;
        u32 yt = rand() % LEVEL_H;

        tick_tile(level, xt, yt);
    }

    for(u32 i = 0; i < ENTITY_CAP; i++) {
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
void level_draw(struct Level *level) {
    if(level->player) {
        i32 x_offset = level->player->x - SCREEN_W / 2;
        i32 y_offset = level->player->y - SCREEN_H / 2;
        // TODO in the original, it's (SCREEN_H - 8) for y_offset

        if(x_offset < 16) x_offset = 16;
        if(y_offset < 16) y_offset = 16;

        if(x_offset > LEVEL_W * 16 - SCREEN_W - 16)
            x_offset = LEVEL_W * 16 - SCREEN_W - 16;

        if(y_offset > LEVEL_H * 16 - SCREEN_H - 16)
            y_offset = LEVEL_H * 16 - SCREEN_H - 16;

        level_x_offset = x_offset;
        level_y_offset = y_offset;

        // level tiles offset
        BG1_XOFFSET = level_x_offset & 0xf;
        BG1_YOFFSET = level_y_offset & 0xf;

        // sky background offset
        BG0_XOFFSET = (level_x_offset >> 2) & 0x7;
        BG0_YOFFSET = (level_y_offset >> 2) & 0x7;
    }

    u32 x0 = level_x_offset >> 4;
    u32 y0 = level_y_offset >> 4;

    // draw level tiles
    for(u32 y = 0; y <= 9; y++) {
        for(u32 x = 0; x <= 15; x++) {
            u32 xt = x + x0;
            u32 yt = y + y0;

            u16 tiles[4] = { 0 };

            draw_tile(level, xt, yt, tiles);

            // using 32bit writes instead of 16bit writes saves a little time
            vu32 *tile_0 = (vu32 *) &BG1_TILEMAP[x * 2 + y * 2 * 32];
            *(tile_0)      = (tiles[1] << 16) | tiles[0];
            *(tile_0 + 16) = (tiles[3] << 16) | tiles[2];
        }
    }

    // TODO in the original game, entities are sorted by y before rendering

    // draw entities
    u32 sprites_drawn = 0;
    for(u32 i = 0; i < ENTITY_CAP; i++) {
        struct entity_Data *data = &level->entities[i];
        if(data->type >= ENTITY_TYPES)
            continue;

        // position relative to top-left of the screen
        i32 xr = data->x - level_x_offset;
        i32 yr = data->y - level_y_offset;

        if(xr < -16 || xr >= SCREEN_W + 16 ||
           yr < -16 || yr >= SCREEN_H)
            continue;

        const struct Entity *entity = ENTITY_S(data);
        entity->draw(level, data, OAM + sprites_drawn * 4);

        sprites_drawn++;
        if(sprites_drawn == 128)
            break;

        // draw player light
        // TODO check if level has light
        if(true && data->type == PLAYER_ENTITY) {
            vu16 *sprite_attribs = OAM + sprites_drawn * 4;

            if(player_active_item.type == LANTERN_ITEM) {
                sprite_attribs[0] = ((yr - 4 - 64) & 0xff) |
                                    (1 << 8) | (1 << 9) | (2 << 10);
                sprite_attribs[1] = ((xr - 64) & 0x1ff) |
                                    (3 << 14);
                sprite_attribs[2] = 336;
            } else {
                sprite_attribs[0] = ((yr - 4 - 16) & 0xff) |
                                    (2 << 10);
                sprite_attribs[1] = ((xr - 16) & 0x1ff) |
                                    (2 << 14);
                sprite_attribs[2] = 320;
            }

            sprites_drawn++;
            if(sprites_drawn == 128)
                break;
        }
    }

    // hide remaining sprites
    for(u32 i = sprites_drawn; i < 128; i++) {
        vu16 *sprite_attribs = OAM + i * 4;
        sprite_attribs[0] = 1 << 9;
    }

    // clear light
    {
        // TODO determine is level has light
        u16 tile = 256 | (0 << 12);

        for(u32 y = 0; y < 18; y++)
            for(u32 x = 0; x < 30; x += 2)
                *((vu32 *) &BG2_TILEMAP[x + y * 32]) = (tile << 16) | tile;
    }

    // TODO draw light from tiles
}

IWRAM_SECTION
u8 level_new_entity(struct Level *level, u8 type) {
    for(u32 i = 0; i < ENTITY_CAP; i++) {
        struct entity_Data *data = &level->entities[i];

        if(data->type >= ENTITY_TYPES) {
            data->type = type;

            // clear entity data
            for(u32 j = 0; j < sizeof(data->data) / sizeof(u32); j++)
                *((u32 *) &data->data[j]) = 0;

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

    if(data->type == PLAYER_ENTITY)
        level->player = data;
}
