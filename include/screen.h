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

#define REG_DISPCNT *((vu16 *)(0x04000000))
#define REG_VCOUNT  *((vu16 *)(0x04000006))

#define SCREEN_W (240)
#define SCREEN_H (160)

extern void screen_init(void);

extern void vsync(void);

#endif // MINICRAFT_SCREEN
