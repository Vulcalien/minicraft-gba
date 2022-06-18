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

u32 level_x_offset = 0;
u32 level_y_offset = 0;

IWRAM_SECTION
void level_tick(struct Level *level) {
    // TODO try spawn

    for(u32 i = 0; i < LEVEL_W * LEVEL_H / 50; i++) {
        u32 xt = rand() % LEVEL_W;
        u32 yt = rand() % LEVEL_H;

        const struct Tile *tile = LEVEL_GET_TILE_S(level, xt, yt);
        if(tile->tick)
            tile->tick(level, xt, yt);
    }

    // TODO more on entity ticking...
    for(u32 i = 0; i < ENTITY_CAP; i++) {
        struct entity_Data *entity_data = &level->entities[i];
        if(entity_data->type >= ENTITY_TYPES)
            continue;

        const struct Entity *entity = ENTITY_S(entity_data);
        entity->tick(level, entity_data);
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

        BG0_XOFFSET = BG1_XOFFSET = level_x_offset & 0x0f;
        BG0_YOFFSET = BG1_YOFFSET = level_y_offset & 0x0f;
    }

    u32 x0 = level_x_offset >> 4;
    u32 y0 = level_y_offset >> 4;

    // draw level tiles
    for(u32 y = 0; y <= 10; y++) {
        for(u32 x = 0; x <= 15; x++) {
            u32 xt = x + x0;
            u32 yt = y + y0;

            u16 tiles[4]  = { 32, 32, 32, 32 };
            u16 tiles2[4] = { 32, 32, 32, 32 };

            const struct Tile *tile = LEVEL_GET_TILE_S(level, xt, yt);
            if(tile->draw)
                tile->draw(level, xt, yt, tiles, tiles2);

            // using 32bit writes instead of 16bit writes saves a little time
            vu32 *bg0_tile_0 = (vu32 *) &BG0_TILEMAP[x * 2 + y * 2 * 32];
            *(bg0_tile_0)      = (tiles[1] << 16) | tiles[0];
            *(bg0_tile_0 + 16) = (tiles[3] << 16) | tiles[2];

            vu32 *bg1_tile_0 = (vu32 *) &BG1_TILEMAP[x * 2 + y * 2 * 32];
            *(bg1_tile_0)      = (tiles2[1] << 16) | tiles2[0];
            *(bg1_tile_0 + 16) = (tiles2[3] << 16) | tiles2[2];
        }
    }

    // TODO in the original game, entities are sorted by y before rendering

    // draw entities
    u32 sprites_drawn = 0;
    for(u32 i = 0; i < ENTITY_CAP; i++) {
        struct entity_Data *entity_data = &level->entities[i];
        if(entity_data->type >= ENTITY_TYPES)
            continue;

        // TODO check if entity is visible in screen

        const struct Entity *entity = ENTITY_S(entity_data);
        entity->draw(level, entity_data, OAM + i * 4);

        sprites_drawn++;
        if(sprites_drawn == 128)
            break;
    }

    // hide remaining sprites
    for(u32 i = sprites_drawn; i < 128; i++) {
        vu16 *sprite_attribs = OAM + i * 4;
        sprite_attribs[0] = 1 << 9;
    }
}
