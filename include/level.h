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
#ifndef MINICRAFT_LEVEL
#define MINICRAFT_LEVEL

#include "minicraft.h"

#define LEVEL_W (128)
#define LEVEL_H (128)

struct Level {
    u8 tiles[LEVEL_W * LEVEL_H];
    u8  data[LEVEL_W * LEVEL_H];
};

extern void level_tick(struct Level *level);

#endif // MINICRAFT_LEVEL
