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

    // DEBUG
    for(u32 i = 0; i < 32 * 1024; i++)
        CHAR_BLOCK_0[i] = 0;
    for(u32 i = 0; i < 16; i++)
        CHAR_BLOCK_0[16 + i] = 0x2222;

    for(u32 i = 0; i < LEVEL_W * LEVEL_H; i++)
        level->tiles[i] = 0;

    // DEBUG
    ((vu16 *) 0x5000000)[0] = 0x00ff;
    ((vu16 *) 0x5000000)[2] = 0xff00;

    while(true) {
        tick();

        vsync();
        draw();

        // DEBUG check performance
        ((vu16 *) 0x0e000000)[1] = *((vu16 *) 0x04000006);
    }
    return 0;
}
