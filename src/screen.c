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
#include "screen.h"

#include "images.h"

#define DISPLAY_CONTROL *((vu16 *) 0x04000000)
#define VCOUNT          *((vu16 *) 0x04000006)

#define BG0_CONTROL *((vu16 *) 0x04000008)
#define BG1_CONTROL *((vu16 *) 0x0400000a)
#define BG2_CONTROL *((vu16 *) 0x0400000c)
#define BG3_CONTROL *((vu16 *) 0x0400000e)

#define BG_PALETTE ((vu16 *) 0x05000000)

void screen_init(void) {
    // TODO enable BG2 and BG3
    DISPLAY_CONTROL = (0)       | // Video mode
                      (1 << 8)  | // Enable BG 0
                      (1 << 9)  | // Enable BG 1
                      (0 << 10) | // Enable BG 2
                      (0 << 11);  // Enable BG 3

    // Level tiles - 1st layer
    BG0_CONTROL = (3)       | // BG Priority (0 is highest, 3 is lowest)
                  (0 << 2)  | // Tileset character block
                  (0 << 7)  | // Color mode (0 is 4bpp with 16/16 palettes)
                  (16 << 8) | // Tilemap screen block
                  (0 << 14);  // BG size (0 is 256x256)

    // Level tiles - 2nd layer
    BG1_CONTROL = (2)       | // BG Priority (0 is highest, 3 is lowest)
                  (0 << 2)  | // Tileset character block
                  (0 << 7)  | // Color mode (0 is 4bpp with 16/16 palettes)
                  (17 << 8) | // Tilemap screen block
                  (0 << 14);  // BG size (0 is 256x256)

    // Light system ???
    // BG2_CONTROL = ...

    // Text and GUI
    BG3_CONTROL = (0)       | // BG Priority (0 is highest, 3 is lowest)
                  (1 << 2)  | // Tileset character block
                  (0 << 7)  | // Color mode (0 is 4bpp with 16/16 palettes)
                  (19 << 8) | // Tilemap screen block
                  (0 << 14);  // BG size (0 is 256x256)

    // copy bg_palette
    memcpy16(BG_PALETTE, bg_palette, 256);

    // NOTE: this code is very inefficient
    for(u32 i = 0; i < sizeof(level_tileset) / sizeof(u16); i++) {
        u8 b0 = level_tileset[i * 2];
        u8 b1 = level_tileset[i * 2 + 1];

        // in a byte, lower 4 bits are for left, upper 4 bits for right
        b0 = (b0 << 4) | (b0 >> 4);
        b1 = (b1 << 4) | (b1 >> 4);

        CHAR_BLOCK_0[i] = (b1 << 8) | b0;
    }
}

void vsync(void) {
    while(VCOUNT >= SCREEN_H);
    while(VCOUNT < SCREEN_H);
}
