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
#include "scene.h"
#include "sound.h"

#define PERFORMANCE_CHECK

#ifdef PERFORMANCE_CHECK
    #include "level.h"
    #include "entity.h"
#endif

u32 tick_count = 0;

u32 gametime = 0;
u32 score = 0;

u8 current_level = 3;

static void tick(void) {
    input_tick();
    scene->tick();

    tick_count++;
}

static void draw(void) {
    scene->draw();
}

int main(void) {
    screen_init();
    set_scene(&scene_prestart, false);

    sound_init();

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

        #ifdef PERFORMANCE_CHECK
        if(tick_count % 15 == 0) {
            u32 entity_count = 0;
            for(u32 i = 0; i < ENTITY_LIMIT; i++)
                if(levels[current_level].entities[i].type < ENTITY_TYPES)
                    entity_count++;

            char c[3] = { '\0', '\0', '\0' };
            if((entity_count >> 4) < 10)
                c[0] = '0' + (entity_count >> 4);
            else
                c[0] = 'A' + (entity_count >> 4) - 10;

            if((entity_count & 0x0f) < 10)
                c[1] = '0' + (entity_count & 0x0f);
            else
                c[1] = 'A' + (entity_count & 0x0f) - 10;

            screen_write(c, 0, 28, 0);
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
