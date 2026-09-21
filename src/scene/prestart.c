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
#include "scene.h"

#include "screen.h"
#include "i18n.h"

// considering that checksum verification takes around 40 ticks,
// the text will be fully visible for about two seconds.
#define ADDITIONAL_DELAY (80)

static u16 counter = 0;

THUMB
static void prestart_tick(void) {
    counter++;

    if(counter == 0x20 + ADDITIONAL_DELAY)
        set_scene(&scene_start, 1);
}

THUMB
static void prestart_draw(void) {
    if(counter < 0x20)
        effects_brighten(NULL, (0x20 - counter) / 2);
    else
        effects_disable();

    screen_write(text(TEXT_PRESTART), 0, 1, 1);
}

const struct Scene scene_prestart = {
    .tick = prestart_tick,
    .draw = prestart_draw
};
