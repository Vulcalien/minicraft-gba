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
#include "interrupt.h"

#include "sound.h"

#define IME *((vu32 *) 0x04000208)
#define IE  *((vu16 *) 0x04000200)
#define IF  *((vu16 *) 0x04000202)

#define INTERRUPT_HANDLER *((vu32 *) 0x03fffffc)

#define TIMER_1 (1 << 4)
#define TIMER_2 (1 << 5)

IWRAM_SECTION
static void interrupt_handler(void) {
    if(IF & TIMER_1) {
        sound_interrupt(1);
        IF = TIMER_1;
    }

    if(IF & TIMER_2) {
        sound_interrupt(2);
        IF = TIMER_2;
    }
}

void interrupt_enable(void) {
    INTERRUPT_HANDLER = (u32) &interrupt_handler;

    IE = 1 << 4 | // Timer 1
         1 << 5;  // Timer 2

    IME = 1;
}
