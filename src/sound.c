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

#define DMA1_SOURCE  *((vu32 *) 0x040000bc)
#define DMA1_DEST    *((vu32 *) 0x040000c0)
#define DMA1_CONTROL *((vu16 *) 0x040000c6)

#define DMA2_SOURCE  *((vu32 *) 0x040000c8)
#define DMA2_DEST    *((vu32 *) 0x040000cc)
#define DMA2_CONTROL *((vu16 *) 0x040000d2)

#define TIMER0_RELOAD  *((vu16 *) 0x04000100)
#define TIMER0_CONTROL *((vu16 *) 0x04000102)

#define TIMER1_RELOAD  *((vu16 *) 0x04000104)
#define TIMER1_CONTROL *((vu16 *) 0x04000106)

#define TIMER2_RELOAD  *((vu16 *) 0x04000108)
#define TIMER2_CONTROL *((vu16 *) 0x0400010a)

#define TIMER3_RELOAD  *((vu16 *) 0x0400010c)
#define TIMER3_CONTROL *((vu16 *) 0x0400010e)

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

    DMA1_DEST = FIFO_A;
    DMA2_DEST = FIFO_B;

    DMA1_CONTROL = DMA2_CONTROL = 2 << 5  | // Dest Address Control (2 is Fixed)
                                  1 << 9  | // DMA repeat
                                  1 << 10 | // Transfer type (1 is 32bit)
                                  3 << 12;  // Start Timing (3 is FIFO)

    TIMER0_RELOAD = TIMER1_RELOAD = 65536 - 1024;
}

void sound_play(const u8 *sound, u32 length) {
    static u8 channel_flag = 0;
    channel_flag ^= 1;

    if(channel_flag) {
        // Channel A
        DMA1_SOURCE = (u32) sound;
        DMA1_CONTROL |= 1 << 15;

        TIMER0_CONTROL = 1 << 7;
    } else {
        // Channel B
        DMA2_SOURCE = (u32) sound;
        DMA2_CONTROL |= 1 << 15;

        TIMER1_CONTROL = 1 << 7;
    }
}
