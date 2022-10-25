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

#define TIMER3_RELOAD  *((vu16 *) 0x0400010c)
#define TIMER3_CONTROL *((vu16 *) 0x0400010e)

// --- Channel | Feed DMA | Feed Timer | Stop Timer ---
// ---    A    |    1     |      0     |      2     ---
// ---    B    |    2     |      1     |      3     ---

IWRAM_SECTION
void sound_interrupt(u32 timer) {
    if(timer == 2) {
        // Channel A
        TIMER0_CONTROL = 0;
        TIMER2_CONTROL = 0;
        DMA1_CONTROL   = 0;
    } else if(timer == 3) {
        // Channel B
        TIMER1_CONTROL = 0;
        TIMER3_CONTROL = 0;
        DMA2_CONTROL   = 0;
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
                      1 << 14;  // Channel B Timer (1 is Timer 1)
}

IWRAM_SECTION
void sound_play(const u8 *sound, u16 length) {
    const u16 dma_value = 2 << 5  | // Dest Address Control (2 is Fixed)
                          1 << 9  | // DMA repeat
                          1 << 10 | // Transfer type (1 is 32bit)
                          3 << 12 | // Start Timing (3 is FIFO)
                          1 << 15;  // DMA Enable

    const u16 timer_value = 3 << 0 | // Prescaler Selection (3 is 1024)
                            1 << 7;  // Timer Start

    static u8 channel_flag = 0;
    channel_flag ^= 1;

    if(channel_flag) {
        // Channel A
        DMA1_SOURCE  = (u32) sound;
        DMA1_DEST    = FIFO_A;
        DMA1_CONTROL = dma_value;

        // 'Stop' timer
        TIMER2_RELOAD = 65536 - length;
        TIMER2_CONTROL = 3 << 0 | // Prescaler Selection (3 is 1024)
                         1 << 6 | // Timer IRQ Enable
                         1 << 7;  // Timer Start

        // 'Feed' timer
        TIMER0_RELOAD  = 65536 - 1;
        TIMER0_CONTROL = timer_value;
    } else {
        // Channel B
        DMA2_SOURCE  = (u32) sound;
        DMA2_DEST    = FIFO_B;
        DMA2_CONTROL = dma_value;

        // 'Stop' timer
        TIMER3_RELOAD = 65536 - length;
        TIMER3_CONTROL = 3 << 0 | // Prescaler Selection (3 is 1024)
                         1 << 6 | // Timer IRQ Enable
                         1 << 7;  // Timer Start

        // 'Feed' timer
        TIMER1_RELOAD  = 65536 - 1;
        TIMER1_CONTROL = timer_value;
    }
}
