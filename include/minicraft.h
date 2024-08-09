/* Copyright 2022-2023 Vulcalien
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
#ifndef MINICRAFT_CORE
#define MINICRAFT_CORE

#include <base.h>

#include "util.h"

extern u32 tick_count;
extern u32 expected_tickcount;

extern u32 gametime; // measured in ticks
extern u32 score;

extern u8 current_level;

// The behavior of right shift for negative values is
// implementation-dependent, but Arithmetic Right Shift is required
static_assert((-1) >> 1 == -1, "Arithmetic Right Shift is required");

typedef   signed long long i64;
typedef unsigned long long u64;

static_assert(sizeof(u64) == 8, "size of u64 is incorrect");
static_assert(sizeof(i64) == 8, "size of i64 is incorrect");

#endif // MINICRAFT_CORE
