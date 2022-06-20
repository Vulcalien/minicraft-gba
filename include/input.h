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
#ifndef MINICRAFT_INPUT
#define MINICRAFT_INPUT

#include "minicraft.h"

#define KEY_A      (1 << 0)
#define KEY_B      (1 << 1)
#define KEY_SELECT (1 << 2)
#define KEY_START  (1 << 3)

#define KEY_RIGHT  (1 << 4)
#define KEY_LEFT   (1 << 5)
#define KEY_UP     (1 << 6)
#define KEY_DOWN   (1 << 7)

#define KEY_L      (1 << 8)
#define KEY_R      (1 << 9)

#define INPUT_DOWN(key)\
    (!(input_keys_down & (key)))

#define INPUT_CLICKED(key)\
    (!(input_keys_clicked & (key)))

extern u16 input_keys_down;
extern u16 input_keys_clicked;

extern void input_tick(void);

#endif // MINICRAFT_INPUT
