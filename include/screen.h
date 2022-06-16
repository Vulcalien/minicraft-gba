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

#define BG0_TILEMAP ((vu16 *) 0x06008000)
#define BG1_TILEMAP ((vu16 *) 0x06008800)
#define BG2_TILEMAP ((vu16 *) 0x06009000)
#define BG3_TILEMAP ((vu16 *) 0x06009800)

#define BG0_XOFFSET *((vu16 *) 0x04000010)
#define BG0_YOFFSET *((vu16 *) 0x04000012)

#define BG1_XOFFSET *((vu16 *) 0x04000014)
#define BG1_YOFFSET *((vu16 *) 0x04000016)

extern void screen_init(void);

extern void vsync(void);

extern void screen_write(char *text, u32 palette, u32 x, u32 y);

#endif // MINICRAFT_SCREEN
