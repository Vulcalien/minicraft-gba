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

static i8 crafting_selected;

static u8 can_craft_size;
static u8 can_craft_recipes[16];

static u8 cannot_craft_size;
static u8 cannot_craft_recipes[16];

static void crafting_check_craftable(void) {
    can_craft_size = 0;
    cannot_craft_size = 0;

    for(u32 r = 0; r < crafting_current_recipes_size; r++) {
        const struct crafting_Recipe *recipe = &crafting_current_recipes[r];

        bool can_craft = true;
        for(u32 i = 0; i < CRAFTING_MAX_REQUIRED; i++) {
            u8 item_type = recipe->required.items[i];
            u8 count = recipe->required.count[i];

            if(count == 0)
                continue;

            // search for the item
            bool has_enought = false;
            for(u32 j = 0; j < player_inventory.size; j++) {
                struct item_Data *item_data = &player_inventory.items[j];

                // Bug in the original game: the possibility that items
                // might be split in two different slots (due to another
                // bug in inventory menu) is ignored, so only the first
                // item's count is checked.
                if(item_data->type == item_type) {
                    if(item_data->count >= count)
                        has_enought = true;
                    break;
                }
            }

            if(!has_enought) {
                can_craft = false;
                break;
            }
        }

        if(can_craft)
            can_craft_recipes[can_craft_size++] = r;
        else
            cannot_craft_recipes[cannot_craft_size++] = r;
    }
}

static void crafting_init(void) {
    crafting_selected = 0;

    crafting_check_craftable();
}

static void crafting_tick(void) {
    if(INPUT_CLICKED(KEY_B))
        set_scene(&scene_game, true);

    if(INPUT_CLICKED(KEY_UP))
        crafting_selected--;
    if(INPUT_CLICKED(KEY_DOWN))
        crafting_selected++;

    if(crafting_selected < 0)
        crafting_selected = crafting_current_recipes_size - 1;
    if(crafting_selected >= crafting_current_recipes_size)
        crafting_selected = 0;

    if(INPUT_CLICKED(KEY_A)) {
        if(crafting_selected < can_craft_size) {
            const struct crafting_Recipe *recipe = &crafting_current_recipes[
                can_craft_recipes[crafting_selected]
            ];

            if(crafting_craft(&player_inventory, recipe))
                crafting_check_craftable();
        }
    }
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

    i8 item0 = crafting_selected - (craft_h - 2) / 2;
    if(item0 > crafting_current_recipes_size - (craft_h - 2))
        item0 = crafting_current_recipes_size - (craft_h - 2);
    if(item0 < 0)
        item0 = 0;

    // draw items
    for(u32 i = 0; i < craft_h - 2; i++) {
        if(item0 + i >= crafting_current_recipes_size)
            break;

        u8 recipe_id;
        u8 palette;

        if(item0 + i < can_craft_size) {
            recipe_id = can_craft_recipes[item0 + i];
            palette = 4;
        } else {
            recipe_id = cannot_craft_recipes[item0 + i - can_craft_size];
            palette = 5;
        }

        const struct crafting_Recipe *recipe =
            &crafting_current_recipes[recipe_id];

        struct item_Data data = {
            .type = recipe->result,
            .tool_level = recipe->tool_level
        };

        item_draw_icon(&data, craft_x + 1, craft_y + 1 + i, false);
        item_write_name(&data, palette, craft_x + 2, craft_y + 1 + i);
    }

    // draw cursor arrows
    screen_write(
        ">", 4,
        craft_x            , craft_y + 1 + (crafting_selected - item0)
    );
    screen_write(
        "<", 4,
        craft_x + craft_w - 1, craft_y + 1 + (crafting_selected - item0)
    );

    // draw 'HAVE' item and count
    {
        const struct crafting_Recipe *selected_recipe;
        if(crafting_selected < can_craft_size) {
            selected_recipe = &crafting_current_recipes[
                can_craft_recipes[crafting_selected]
            ];
        } else {
            selected_recipe = &crafting_current_recipes[
                cannot_craft_recipes[crafting_selected - can_craft_size]
            ];
        }

        struct item_Data result = {
            .type = selected_recipe->result,
            .tool_level = selected_recipe->tool_level
        };
        item_draw_icon(&result, have_x + 1, have_y + 1, false);

        u16 count = 0;
        if(item_is_resource(result.type)) {
            for(u32 i = 0; i < player_inventory.size; i++) {
                struct item_Data *item_data = &player_inventory.items[i];

                // Bug in the original game: the possibility that items
                // might be split in two different slots (due to another
                // bug in inventory menu) is ignored, so only the first
                // item's count is checked.
                if(item_data->type == result.type) {
                    count = item_data->count;
                    break;
                }
            }
        } else {
            for(u32 i = 0; i < player_inventory.size; i++) {
                struct item_Data *item_data = &player_inventory.items[i];

                // FIXED BUG - Inventory.java:63
                // all furniture items are considered the same item
                if(item_data->type == result.type) {
                    const struct Item *item = ITEM_S(item_data);

                    if(item->class != ITEMCLASS_TOOL ||
                       item_data->tool_level == result.tool_level)
                        count++;
                }
            }
        }

        char count_text[6] = { 0 };
        itoa(count, count_text, 5);
        screen_write(count_text, 4, have_x + 2, have_y + 1);
    }
}

const struct Scene scene_crafting = {
    .init = crafting_init,

    .tick = crafting_tick,
    .draw = crafting_draw
};
