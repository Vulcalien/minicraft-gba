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
#include "minicraft.h"

#if false

#include "level.h"
#include "tile.h"
#include "generator.h"
#include "input.h"
#include "screen.h"

#define DISPLAY_CONTROL *((vu16 *) 0x04000000)

#define VRAM ((vu16 *) 0x06000000)

int main(void) {
    DISPLAY_CONTROL = 3 << 0  | // Video mode
                      1 << 10;  // Enable BG 2

    generate_levels();

    u32 displayed_level = 0;
    while(true) {
        vsync();
        input_tick();

        if(INPUT_CLICKED(KEY_LEFT)) {
            if(displayed_level != 0)
                displayed_level--;
            else
                displayed_level = 4;
        }

        if(INPUT_CLICKED(KEY_RIGHT)) {
            if(displayed_level != 4)
                displayed_level++;
            else
                displayed_level = 0;
        }

        struct Level *level = &levels[displayed_level];
        for(u32 y = 0; y < LEVEL_H; y++) {
            for(u32 x = 0; x < LEVEL_W; x++) {
                u16 color = 0;
                switch(level->tiles[x + y * LEVEL_W]) {
                    case GRASS_TILE:
                        color = 0x03e0;
                        break;
                    case FLOWER_TILE:
                        color = 0x1b66;
                        break;
                    case LIQUID_TILE:
                        color = 0x7c00;
                        break;
                    case TREE_TILE:
                        color = 0x01e0;
                        break;
                    case ROCK_TILE:
                        color = 0x294a;
                        break;
                    case DIRT_TILE:
                        color = 0x1993;
                        break;
                    case IRON_ORE_TILE:
                        color = 0x4e7f;
                        break;
                    case GOLD_ORE_TILE:
                        color = 0x1bff;
                        break;
                    case GEM_ORE_TILE:
                        color = 0x667f;
                        break;
                    case CLOUD_TILE:
                        color = 0x7fff;
                        break;
                    case CLOUD_CACTUS_TILE:
                        color = 0x56b5;
                        break;
                }

                VRAM[(64 + x) + (24 + y) * 240] = color;
            }
        }
    }
}

#endif
