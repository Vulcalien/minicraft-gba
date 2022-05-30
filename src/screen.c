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

#define DISPLAY_CONTROL *((vu16 *) 0x04000000)
#define VCOUNT          *((vu16 *) 0x04000006)

#define VIDEOMODE_0 (0)

#define BG0_ENABLE (0x0100)
#define BG1_ENABLE (0x0200)

#define BG0_CONTROL *((vu16 *) 0x04000008)
#define BG1_CONTROL *((vu16 *) 0x0400000a)

#define BG0_XOFFSET *((vu16 *) 0x04000010)
#define BG1_YOFFSET *((vu16 *) 0x04000012)

void screen_init(void) {
    // bg0 -> tiles
    // bg1 -> GUI
    DISPLAY_CONTROL = VIDEOMODE_0 | BG0_ENABLE | BG1_ENABLE;

    BG0_CONTROL = (3)      | // BG Priority (3 is lowest)
                  (0 << 2) | // Tileset character block
                  (0 << 6) | // Mosaic flag
                  (1 << 7) | // Color mode (1 is 256 palette)
                  (8 << 8) | // Tilemap screen block
                  (0 << 14); // BG size (0 is 256x256)
}

void vsync(void) {
    while(VCOUNT >= SCREEN_H);
    while(VCOUNT < SCREEN_H);
}
