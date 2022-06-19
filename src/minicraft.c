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
#include "input.h"
#include "level.h"
#include "menu.h"

u32 tick_count = 0;

static const struct Menu *menu  = NULL;
static struct Level *level = NULL;

EWRAM_BSS_SECTION
static struct Level levels[5];

void tick(void) {
    input_tick();

    if(menu)
        menu->tick();
    else if(level)
        level_tick(level);

    tick_count++;
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
    level = &levels[0];
    srand(4);
    for(u32 i = 0; i < LEVEL_W * LEVEL_H; i++) {
        /*level->tiles[i] = (i/2)%23;*/
        /*level->tiles[i] = rand() % 23;*/
        level->tiles[i] = (i/16%2) ? 11 : 6;
        level->data[i] = 0;
    }

    // DEBUG
    for(u32 i = 0; i < ENTITY_CAP; i++)
        level->entities[i].type = i < 1 ? 0:-1;
    level->entities[0].type = 3;
    level->player = &level->entities[0];

    // DEBUG: calculate header checksum
    #ifdef GENERATE_CHECKSUM
        u8 checksum = 0;
        for(u32 i = 0xa0; i <= 0xbc; i++)
            checksum -= ((vu8 *) 0x8000000)[i];

        checksum -= 0x19;
        ((vu8 *) 0x0e000000)[0] = checksum;
    #endif // GENERATE_CHECKSUM

    /*menu = &menu_start;*/

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
