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
#include "util.h"

// TODO make this inline ???

#define DMA3_SOURCE *((vu32 *) 0x040000d4)
#define DMA3_DEST   *((vu32 *) 0x040000d8)

#define DMA3_COUNT   *((vu16 *) 0x040000dc)
#define DMA3_CONTROL *((vu16 *) 0x040000de)

void memcpy16(vu16 *dest, const vu16 *src, u32 n) {
    DMA3_SOURCE  = (u32) src;
    DMA3_DEST    = (u32) dest;

    DMA3_COUNT   = n;
    DMA3_CONTROL = (0 << 10) | // Transfer type (0 is 16bit, 1 is 32bit)
                   (1 << 15);  // DMA Enable
}

// pseudorandom number generator
static u32 seed = 0;

IWRAM_SECTION
u16 rand(void) {
    seed = seed * 0x248f7b13 + 0xc21840c5;
    return seed >> 16;
}

void srand(u32 new_seed) {
    seed = new_seed;
}
