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
#include "sound.h"

static i8 crafting_selected;

static u8 crafting_sorted_recipes[16];
static bool crafting_can_craft[16];

static u16 crafting_count(struct Inventory *inventory,
                          u8 item_type, u8 tool_level) {
    if(item_is_resource(item_type)) {
        for(u32 i = 0; i < inventory->size; i++) {
            struct item_Data *item_data = &inventory->items[i];

            // Bug in the original game: the possibility that items
            // might be split in two different slots (due to another
            // bug in inventory menu) is ignored, so only the first
            // item's count is checked.
            if(item_data->type == item_type)
                return item_data->count;
        }
    } else {
        u16 count = 0;
        for(u32 i = 0; i < inventory->size; i++) {
            struct item_Data *item_data = &inventory->items[i];

            // FIXED BUG - Inventory.java:63
            // all furniture items are considered the same item
            if(item_data->type == item_type) {
                if(ITEM_S(item_data)->class != ITEMCLASS_TOOL ||
                   item_data->tool_level == tool_level)
                    count++;
            }
        }
        return count;
    }
    return 0;
}

static void crafting_check_craftable(void) {
    for(u32 r = 0; r < crafting_current_recipes_size; r++) {
        const struct crafting_Recipe *recipe = &crafting_current_recipes[r];

        crafting_can_craft[r] = true;
        for(u32 i = 0; i < CRAFTING_MAX_REQUIRED; i++) {
            u8 item_type = recipe->required.items[i];
            u8 count = recipe->required.count[i];

            if(count == 0)
                break;

            u16 has_count = crafting_count(&player_inventory, item_type, 0);
            if(has_count < count) {
                crafting_can_craft[r] = false;
                break;
            }
        }
    }
}

static void crafting_init(void) {
    crafting_selected = 0;

    crafting_check_craftable();

    u32 pos = 0;
    for(u32 i = 0; i < crafting_current_recipes_size; i++)
        if(crafting_can_craft[i])
            crafting_sorted_recipes[pos++] = i;

    for(u32 i = 0; i < crafting_current_recipes_size; i++)
        if(!crafting_can_craft[i])
            crafting_sorted_recipes[pos++] = i;
}

static void crafting_tick(void) {
    gametime++;

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
        u8 recipe_id = crafting_sorted_recipes[crafting_selected];

        if(crafting_can_craft[recipe_id]) {
            const struct crafting_Recipe *recipe = &crafting_current_recipes[
                recipe_id
            ];

            if(crafting_craft(&player_inventory, recipe)) {
                crafting_check_craftable();

                SOUND_PLAY(sound_craft);
            }
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

        u8 recipe_id = crafting_sorted_recipes[item0 + i];
        u8 palette;
        if(crafting_can_craft[recipe_id])
            palette = 4;
        else
            palette = 5;

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

    const struct crafting_Recipe *selected_recipe =
        &crafting_current_recipes[crafting_sorted_recipes[crafting_selected]];

    // draw 'HAVE' item and count
    {
        struct item_Data result = {
            .type = selected_recipe->result,
            .tool_level = selected_recipe->tool_level
        };
        item_draw_icon(&result, have_x + 1, have_y + 1, false);

        u16 count = crafting_count(
            &player_inventory, result.type, result.tool_level
        );

        SCREEN_WRITE_NUMBER(count, 5, 4, have_x + 2, have_y + 1);
    }

    // draw 'COST' items and count
    for(u32 i = 0; i < CRAFTING_MAX_REQUIRED; i++) {
        u8 item_type = selected_recipe->required.items[i];
        u8 required_count = selected_recipe->required.count[i];

        if(required_count == 0)
            break;

        struct item_Data required_item = { .type = item_type };
        item_draw_icon(&required_item, cost_x + 1, cost_y + 1 + i, false);

        u16 has_count = crafting_count(&player_inventory, item_type, 0);
        if(has_count > 99)
            has_count = 99;

        char cost_text[6] = { 0 };
        itoa(required_count, cost_text, 2);
        cost_text[1 + (required_count > 9)] = '/';
        itoa(has_count, cost_text + 2 + (required_count > 9), 2);

        u8 palette = 4 + (has_count < required_count);
        screen_write(cost_text, palette, cost_x + 2, cost_y + 1 + i);
    }
}

const struct Scene scene_crafting = {
    .init = crafting_init,

    .tick = crafting_tick,
    .draw = crafting_draw
};
