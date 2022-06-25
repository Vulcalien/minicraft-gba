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

static struct Level *level = NULL;

EWRAM_BSS_SECTION
static struct Level levels[5];

static void tick(void) {
    input_tick();

    if(menu)
        menu->tick();
    else if(level)
        level_tick(level);

    tick_count++;
}

static void draw(void) {
    if(menu)
        menu->draw();
    else if(level)
        level_draw(level);
}

int main(void) {
    screen_init();
    /*set_menu(&menu_start, true);*/

    // DEBUG
    level = &levels[0];
    srand(4);
    for(u32 i = 0; i < LEVEL_W * LEVEL_H; i++) {
        /*level->tiles[i] = (i/2)%23;*/
        /*level->tiles[i] = rand() % 23;*/
        level->tiles[i] = (i/16%2) ? 11 : 2;
        level->data[i] = 0;
    }

    // DEBUG
    for(u32 i = 1; i < ENTITY_CAP; i++) {
        if(i < 256) {
            level->entities[i].type = i % 2;
            level->entities[i].x = 80;
            level->entities[i].y = 80;
        } else {
            level->entities[i].type = -1;
        }
    }
    level->entities[0].type = 3;
    level->entities[0].x = 40;
    level->entities[0].y = 40;
    level->player = &level->entities[0];

    // DEBUG: calculate header checksum
    #ifdef GENERATE_CHECKSUM
        u8 checksum = 0;
        for(u32 i = 0xa0; i <= 0xbc; i++)
            checksum -= ((vu8 *) 0x8000000)[i];

        checksum -= 0x19;
        ((vu8 *) 0x0e000000)[0] = checksum;
    #endif

    while(true) {
        tick();

        #define PERFORMANCE_CHECK
        #ifdef PERFORMANCE_CHECK
        if(tick_count % 15 == 0) {
            u8 vcount = *((vu8 *) 0x04000006);
            for(u32 x = 0; x < 2; x++) {
                char c[2] = { '\0', '\0' };

                u8 hex_digit = (vcount >> 4 * (1 - x)) & 0x0f;
                c[0] = hex_digit +
                       (hex_digit <  0xa) * '0' +
                       (hex_digit >= 0xa) * ('A' - 0xa);

                screen_write(c, 0, x, 0);
            }
        }
        #endif

        vsync();
        draw();

        #ifdef PERFORMANCE_CHECK
        if(tick_count % 15 == 0) {
            u8 vcount = *((vu8 *) 0x04000006);
            for(u32 x = 0; x < 2; x++) {
                char c[2] = { '\0', '\0' };

                u8 hex_digit = (vcount >> 4 * (1 - x)) & 0x0f;
                c[0] = hex_digit +
                       (hex_digit <  0xa) * '0' +
                       (hex_digit >= 0xa) * ('A' - 0xa);

                screen_write(c, 0, x, 1);
            }
        }
        #endif
    }
    return 0;
}
