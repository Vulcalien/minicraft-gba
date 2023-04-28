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
#include "performance.h"

#include "screen.h"
#include "input.h"
#include "level.h"
#include "entity.h"

static bool show_performance = false;
static u16 tick_vcount;
static u16 draw_vcount;

static u16 ticks = 0, frames = 0;
static u16 tps   = 0, fps    = 0;

void performance_tick(void) {
    tick_vcount = VCOUNT;
    ticks++;

    if(INPUT_DOWN(KEY_L) && INPUT_DOWN(KEY_R) && INPUT_CLICKED(KEY_SELECT))
        show_performance = !show_performance;
}

void performance_draw(void) {
    draw_vcount = VCOUNT;
    frames++;

    if(!show_performance || tick_count % 15 != 0)
        return;

    SCREEN_WRITE_NUMBER(tick_vcount, 16, 2, true, 0, 0, 0);
    SCREEN_WRITE_NUMBER(draw_vcount, 16, 2, true, 0, 0, 1);

    SCREEN_WRITE_NUMBER(tps, 10, 2, true, 0, 0, 3);
    SCREEN_WRITE_NUMBER(fps, 10, 2, true, 0, 0, 4);

    // count entities
    u32 entity_count = 0;
    for(u32 i = 0; i < ENTITY_LIMIT; i++)
        if(levels[current_level].entities[i].type < ENTITY_TYPES)
            entity_count++;

    // count sprites
    u32 sprite_count = 0;
    for(u32 i = 0; i < 128; i++)
        if((OAM[i * 4] & (1 << 9)) == 0)
            sprite_count++;

    SCREEN_WRITE_NUMBER(entity_count, 16, 2, true, 0, 28, 0);
    SCREEN_WRITE_NUMBER(sprite_count, 16, 2, true, 0, 28, 1);
}

IWRAM_SECTION
void performance_vblank(void) {
    static u32 vblanks = 0;
    vblanks++;

    if(vblanks == 60) {
        vblanks = 0;

        tps = ticks;
        fps = frames;

        ticks = 0;
        frames = 0;
    }
}
