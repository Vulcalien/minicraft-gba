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
#ifndef MINICRAFT_UTIL
#define MINICRAFT_UTIL

#include "minicraft.h"

inline void itoa(u32 number, u8 radix, char *array, u8 digits, bool zero_fill) {
    u32 pos = 0;
    for(u32 i = 0; i < digits; i++) {
        u32 digit = number;
        for(u32 j = 1; j < digits - i; j++)
            digit /= radix;
        digit %= radix;

        if(digit != 0 || pos != 0 || i == digits - 1 || zero_fill) {
            if(radix <= 10 || digit < 10)
                array[pos++] = '0' + digit;
            else
                array[pos++] = 'A' + (digit - 10);
        }
    }
}

#endif // MINICRAFT_UTIL
