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

IWRAM_SECTION
void level_tick(struct Level *level) {
    // TODO try spawn

    for(u32 i = 0; i < LEVEL_W * LEVEL_H / 50; i++) {
        u32 xt = rand() % LEVEL_W;
        u32 yt = rand() % LEVEL_H;

        LEVEL_GET_TILE_S(level, xt, yt)->tick(level, xt, yt);
    }

    // TODO more on entity ticking...
    for(u32 i = 0; i < ENTITY_CAP; i++) {
        u8 *entity_data = level->entities[i];

        u8 entity_type = entity_data[0];
        if(entity_type >= ENTITY_TYPES)
            continue;

        const struct Entity *entity = &entity_list[entity_type];
        entity->tick(level, entity, entity_data);
    }
}

IWRAM_SECTION
void level_draw(struct Level *level) {
    // DEBUG
    static u32 test = 0;

    // TODO set offset
    u32 x_offset = test;
    u32 y_offset = 0;

    // DEBUG
    test %= 500;

    BG0_XOFFSET = BG1_XOFFSET = x_offset & 0x0f;
    BG0_YOFFSET = BG1_YOFFSET = y_offset & 0x0f;

    u32 x0 = x_offset >> 4;
    u32 y0 = y_offset >> 4;

    for(u32 y = 0; y <= 10; y++) {
        for(u32 x = 0; x <= 15; x++) {
            u32 xt = x + x0;
            u32 yt = y + y0;

            const struct Tile *tile = LEVEL_GET_TILE_S(level, xt, yt);
            u16 tiles[4]  = { 32, 32, 32, 32 };
            u16 tiles2[4] = { 32, 32, 32, 32 };
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
}
