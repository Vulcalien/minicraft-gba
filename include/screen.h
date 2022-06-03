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
#ifndef MINICRAFT_SCREEN
#define MINICRAFT_SCREEN

#include "minicraft.h"

#define SCREEN_W (240)
#define SCREEN_H (160)

#define CHAR_BLOCK_0 ((vu16 *) 0x06000000)
#define CHAR_BLOCK_1 ((vu16 *) 0x06004000)
#define CHAR_BLOCK_2 ((vu16 *) 0x06008000)
#define CHAR_BLOCK_3 ((vu16 *) 0x0600c000)

#define BG0_TILEMAP ((vu16 *) CHAR_BLOCK_2)
#define BG1_TILEMAP ((vu16 *) (CHAR_BLOCK_2 + 0x0800))

#define BG0_XOFFSET *((vu16 *) 0x04000010)
#define BG0_YOFFSET *((vu16 *) 0x04000012)

extern void screen_init(void);

extern void vsync(void);

#endif // MINICRAFT_SCREEN
