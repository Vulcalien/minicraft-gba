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
#include "interrupt.h"

#include "sound.h"
#include "performance.h"

#define IME *((vu32 *) 0x04000208)
#define IE  *((vu16 *) 0x04000200)
#define IF  *((vu16 *) 0x04000202)

#define IF_BIOS *((vu16 *) 0x03007ff8)

#define INTERRUPT_HANDLER *((vu32 *) 0x03007ffc)

#define VBLANK  (1 << 0)
#define TIMER_2 (1 << 5)
#define TIMER_3 (1 << 6)

IWRAM_SECTION
static void interrupt_handler(void) {
    if(IF & VBLANK) {
        IF_BIOS |= VBLANK;
        IF = VBLANK;

        expected_tickcount++;
        performance_vblank();
    }

    if(IF & TIMER_2) {
        sound_interrupt(2);
        IF = TIMER_2;
    }

    if(IF & TIMER_3) {
        sound_interrupt(3);
        IF = TIMER_3;
    }
}

void interrupt_enable(void) {
    INTERRUPT_HANDLER = (u32) &interrupt_handler;

    IE = VBLANK | TIMER_2 | TIMER_3;

    IME = 1;
}
