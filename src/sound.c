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
#include "sound.h"

#define SOUND_CONTROL_H *((vu16 *) 0x04000082)
#define SOUND_CONTROL_X *((vu16 *) 0x04000084)

#define FIFO_A (0x040000a0)
#define FIFO_B (0x040000a4)

// DMA
#define DMA1_SOURCE  *((vu32 *) 0x040000bc)
#define DMA1_DEST    *((vu32 *) 0x040000c0)
#define DMA1_CONTROL *((vu16 *) 0x040000c6)

#define DMA2_SOURCE  *((vu32 *) 0x040000c8)
#define DMA2_DEST    *((vu32 *) 0x040000cc)
#define DMA2_CONTROL *((vu16 *) 0x040000d2)

// Timers
#define TIMER0_RELOAD  *((vu16 *) 0x04000100)
#define TIMER0_CONTROL *((vu16 *) 0x04000102)

#define TIMER1_RELOAD  *((vu16 *) 0x04000104)
#define TIMER1_CONTROL *((vu16 *) 0x04000106)

#define TIMER2_RELOAD  *((vu16 *) 0x04000108)
#define TIMER2_CONTROL *((vu16 *) 0x0400010a)

// Interrupt
#define IME *((vu32 *) 0x04000208)
#define IE  *((vu16 *) 0x04000200)
#define IF  *((vu16 *) 0x04000202)

#define INTERRUPT_HANDLER *((vu32 *) 0x03fffffc)

IWRAM_SECTION
static void interrupt_handler(void) {
    if(IF == 1 << 4) { // Timer 1
        TIMER1_CONTROL = 0;
        DMA1_CONTROL = 0;

        IF = 1 << 4;
    } else if(IF == 1 << 5) { // Timer 2
        TIMER2_CONTROL = 0;
        DMA2_CONTROL = 0;

        IF = 1 << 5;
    }
}

void sound_init(void) {
    SOUND_CONTROL_X = 1 << 7; // Enable Sound

    SOUND_CONTROL_H = 1 << 2  | // Channel A Volume (1 is 100%)
                      1 << 3  | // Channel B Volume (1 is 100%)
                      1 << 8  | // Enable Channel A RIGHT
                      1 << 9  | // Enable Channel A LEFT
                      0 << 10 | // Channel A Timer (0 is Timer 0)
                      1 << 12 | // Enable Channel B RIGHT
                      1 << 13 | // Enable Channel B LEFT
                      0 << 14;  // Channel B Timer (0 is Timer 0)

    DMA1_DEST = FIFO_A;
    DMA2_DEST = FIFO_B;

    TIMER0_RELOAD = 65536 - 1;

    // enable interrupts
    INTERRUPT_HANDLER = (u32) &interrupt_handler;

    IE = 1 << 4 | // Timer 1
         1 << 5;  // Timer 2

    IME = 1;
}

IWRAM_SECTION
void sound_play(const u8 *sound, u16 length) {
    const u16 dma_value = 2 << 5  | // Dest Address Control (2 is Fixed)
                          1 << 9  | // DMA repeat
                          1 << 10 | // Transfer type (1 is 32bit)
                          3 << 12 | // Start Timing (3 is FIFO)
                          1 << 15;  // DMA Enable

    static u8 channel_flag = 0;
    channel_flag ^= 1;

    if(channel_flag) {
        // Channel A
        DMA1_SOURCE = (u32) sound;
        DMA1_CONTROL = dma_value;

        TIMER1_RELOAD = 65536 - length;
        TIMER1_CONTROL = 3 << 0 | // Prescaler Selection (3 is 1024)
                         1 << 6 | // Timer IRQ Enable
                         1 << 7;  // Timer Start
    } else {
        // Channel B
        DMA2_SOURCE = (u32) sound;
        DMA2_CONTROL = dma_value;

        TIMER2_RELOAD = 65536 - length;
        TIMER2_CONTROL = 3 << 0 | // Prescaler Selection (3 is 1024)
                         1 << 6 | // Timer IRQ Enable
                         1 << 7;  // Timer Start
    }

    TIMER0_CONTROL = 3 << 0 | // Prescaler Selection (3 is 1024)
                     1 << 7;  // Timer Start
}
