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

#include "screen.h"
#include "level.h"
#include "menu.h"

struct Menu  *menu  = NULL;
struct Level *level = NULL;

// DEBUG
EWRAM_SECTION
struct Level level_0 = {};

void tick(void) {
    if(menu)
        menu->tick();
    else if(level)
        level_tick(level);
}

void draw(void) {
    if(menu)
        menu->draw();

    if(level)
        level_draw(level);
}

int main(void) {
    screen_init();

    // DEBUG
    level = &level_0;
    for(u32 i = 0; i < LEVEL_W * LEVEL_H; i++) {
        level->tiles[i] = 0;
    }

    LEVEL_SET_TILE(level, 5, 5, 1);
    LEVEL_SET_TILE(level, 4, 5, 1);
    LEVEL_SET_TILE(level, 5, 4, 1);
    LEVEL_SET_TILE(level, 6, 5, 1);
    LEVEL_SET_TILE(level, 5, 6, 1);
    LEVEL_SET_TILE(level, 6, 6, 1);
    LEVEL_SET_TILE(level, 6, 7, 1);

    LEVEL_SET_TILE(level, 8, 7, 2);
    LEVEL_SET_TILE(level, 8, 8, 2);
    LEVEL_SET_TILE(level, 9, 9, 2);

    while(true) {
        tick();

        vsync();
        draw();

        // DEBUG check performance
        ((vu16 *) 0x0e000000)[1] = *((vu16 *) 0x04000006);
    }
    return 0;
}

// TODO make this inline ???

#define DMA3_SOURCE *((vu32 *) 0x040000d4)
#define DMA3_DEST   *((vu32 *) 0x040000d8)

#define DMA3_COUNT   *((vu16 *) 0x040000dc)
#define DMA3_CONTROL *((vu16 *) 0x040000de)

void memcpy16(vu16 *dest, const vu16 *src, u32 n) {
    DMA3_SOURCE  = (u32) src;
    DMA3_DEST    = (u32) dest;

    DMA3_COUNT   = n;
    DMA3_CONTROL = (0 << 10) | // Transfer type (0 is 16bit, 1 is 32bit)
                   (1 << 15);  // DMA Enable
}
