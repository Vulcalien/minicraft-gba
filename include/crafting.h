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
#ifndef MINICRAFT_CRAFTING
#define MINICRAFT_CRAFTING

#include "minicraft.h"

#include "inventory.h"

#define CRAFTING_MAX_REQUIRED (3)

struct crafting_Recipe {
    struct {
        u8 items[CRAFTING_MAX_REQUIRED];
        u8 count[CRAFTING_MAX_REQUIRED];
    } required;

    u8 result;

    u8 tool_level;
};

#define WORKBENCH_RECIPES (16)
#define FURNACE_RECIPES   (3)
#define OVEN_RECIPES      (1)
#define ANVIL_RECIPES     (15)

extern const struct crafting_Recipe workbench_recipes[WORKBENCH_RECIPES];
extern const struct crafting_Recipe furnace_recipes[FURNACE_RECIPES];
extern const struct crafting_Recipe oven_recipes[OVEN_RECIPES];
extern const struct crafting_Recipe anvil_recipes[ANVIL_RECIPES];

extern bool crafting_craft(struct Inventory *inventory,
                           struct crafting_Recipe *recipe);

#endif // MINICRAFT_CRAFTING
