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
#include "scene.h"

#include "input.h"
#include "screen.h"
#include "inventory.h"
#include "item.h"
#include "crafting.h"
#include "player.h"

static struct crafting_Recipe *recipe_list;
static u8 crafting_recipes_count;

static void crafting_init(void) {
}

static void crafting_tick(void) {
    if(INPUT_CLICKED(KEY_B))
        set_scene(&scene_game, true);
}

static void crafting_draw(void) {
    const u8 craft_x = 5;
    const u8 craft_y = 2;
    const u8 craft_w = 12;
    const u8 craft_h = 14;

    const u8 have_x = craft_x + craft_w;
    const u8 have_y = craft_y;
    const u8 have_w = 8;
    const u8 have_h = 3;

    const u8 cost_x = have_x;
    const u8 cost_y = have_y + have_h;
    const u8 cost_w = have_w;
    const u8 cost_h = craft_h - have_h;

    screen_draw_frame("CRAFTING", craft_x, craft_y, craft_w, craft_h);
    screen_draw_frame("HAVE", have_x, have_y, have_w, have_h);
    screen_draw_frame("COST", cost_x, cost_y, cost_w, cost_h);
}

const struct Scene scene_crafting = {
    .init = crafting_init,

    .tick = crafting_tick,
    .draw = crafting_draw
};
