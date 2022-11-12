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
#include "level.h"

// reverse quicksort implementation

static inline void swap(u8 *a, u8 *b) {
    if(*a == *b)
        return;

    u8 temp = *a;
    *a = *b;
    *b = temp;
}

#define VAL(id) (level->entities[entities[(id)]].y)
static inline u32 partition(struct Level *level, u8 *entities,
                            i32 low, i32 high) {
    const u16 pivot_value = VAL(high);

    u32 j = low;
    for(u32 i = low; i < high; i++) {
        const u16 val = VAL(i);
        if(val >= pivot_value) {
            swap(&entities[i], &entities[j]);
            j++;
        }
    }

    swap(&entities[j], &entities[high]);
    return j;
}
#undef VAL

IWRAM_SECTION
static void sort_entities(struct Level *level, u8 *entities,
                          i32 low, i32 high) {
    if(low >= high)
        return;

    u32 pivot_index = partition(level, entities, low, high);

    sort_entities(level, entities, low, pivot_index - 1);
    sort_entities(level, entities, pivot_index + 1, high);
}
