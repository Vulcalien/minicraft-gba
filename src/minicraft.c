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
#include "minicraft.h"

#include "screen.h"
#include "input.h"
#include "scene.h"
#include "sound.h"
#include "level.h"
#include "entity.h"

u32 tick_count = 0;

u32 gametime;
u32 score;

u8 current_level;

static bool performance_debug = false;
static u16 tick_vcount;
static u16 draw_vcount;

static inline void tick(void) {
    input_tick();
    scene->tick();

    tick_count++;

    tick_vcount = VCOUNT;
    if(INPUT_DOWN(KEY_L) && INPUT_DOWN(KEY_R) && INPUT_CLICKED(KEY_SELECT))
        performance_debug = !performance_debug;
}

static inline void draw(void) {
    scene->draw();

    draw_vcount = VCOUNT;
}

static inline void draw_performance(void) {
    SCREEN_WRITE_NUMBER(tick_vcount, 16, 2, true, 0, 0, 0);
    SCREEN_WRITE_NUMBER(draw_vcount, 16, 2, true, 0, 0, 1);

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

int main(void) {
    screen_init();
    set_scene(&scene_prestart, 0);

    sound_init();

    while(true) {
        tick();

        vsync();
        draw();

        if(performance_debug && tick_count % 15 == 0)
            draw_performance();
    }
    return 0;
}
