/* Copyright 2022-2023 Vulcalien
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

#include <gba/interrupt.h>
#include <gba/sound.h>

#include "screen.h"
#include "input.h"
#include "scene.h"
#include "performance.h"

u32 tick_count = 0;
u32 expected_tickcount = 0;

u32 gametime;
u32 score;

u8 current_level;

static inline void tick(void) {
    input_tick();
    scene->tick();

    performance_tick();

    tick_count++;
}

static inline void draw(void) {
    scene->draw();

    performance_draw();
}

IWRAM_SECTION
static void vblank(void) {
    expected_tickcount++;
    performance_vblank();
}

int AgbMain(void) {
    interrupt_init();
    sound_init();

    interrupt_enable(IRQ_VBLANK);
    interrupt_set_isr(IRQ_VBLANK, vblank);

    screen_init();
    set_scene(&scene_prestart, 0);

    while(true) {
        tick();

        // if necessary, skip a frame
        if(tick_count < expected_tickcount) {
            tick();
            expected_tickcount = tick_count; // drop any remaining ticks
        }

        vsync();
        draw();
    }
    return 0;
}
