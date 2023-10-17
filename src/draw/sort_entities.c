/* Copyright 2023 Vulcalien
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
#include "level.h"

#include "screen.h"

// Counting Sort implementation

#define VALUE_RANGE (SCREEN_H + 16)

static u8 count_array[VALUE_RANGE];
static u8 result_array[128];

#define VAL(id) (level->entities[entities[(id)]].y)

IWRAM_SECTION
static u8 *sort_entities(struct Level *level, u8 *entities, u32 n) {
    if(n <= 1)
        return entities;

    // clear count_array
    for(u32 i = 0; i < VALUE_RANGE; i++)
        count_array[i] = 0;

    // find the minimum value
    u16 min = VAL(0);
    for(u32 i = 1; i < n; i++)
        if(min > VAL(i))
            min = VAL(i);

    // count the values
    for(u32 i = 0; i < n; i++)
        count_array[VAL(i) - min]++;

    // calculate indexes
    for(u32 i = 1; i < VALUE_RANGE; i++)
        count_array[i] += count_array[i - 1];

    // set values in result_array
    for(i32 i = n - 1; i >= 0; i--) {
        u8 *index = &count_array[VAL(i) - min];

        (*index)--;
        result_array[*index] = entities[i];
    }

    return result_array;
}
