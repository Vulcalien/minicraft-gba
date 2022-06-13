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

u32 tick_counter = 0;

static struct Menu  *menu  = NULL;
static struct Level *level = NULL;

// DEBUG
EWRAM_SECTION
static struct Level level_0 = {};

void tick(void) {
    if(menu)
        menu->tick();
    else if(level)
        level_tick(level);

    tick_counter++;
}

void draw(void) {
    if(menu)
        menu->draw();
    else if(level)
        level_draw(level);
}

int main(void) {
    screen_init();

    // DEBUG
    level = &level_0;
    for(u32 i = 0; i < LEVEL_W * LEVEL_H; i++) {
        level->tiles[i] = (i/2)%23;
        level->data[i] = i;
    }

    // DEBUG
    /*menu = &menu_start;*/

    // DEBUG: calculate header checksum
    u8 checksum = 0;
    for(u32 i = 0xa0; i <= 0xbc; i++) {
        checksum -= ((vu8 *) 0x8000000)[i];
    }
    checksum -= 0x19;
    ((vu8 *) 0x0e000000)[0] = checksum;

    while(true) {
        tick();

        // DEBUG check performance
        ((vu8 *) 0x0e000000)[1] = *((vu8 *) 0x04000006);

        vsync();
        draw();

        // DEBUG check performance
        ((vu8 *) 0x0e000000)[0] = *((vu8 *) 0x04000006);
    }
    return 0;
}
