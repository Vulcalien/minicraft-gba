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
#ifndef MINICRAFT_INVENTORY
#define MINICRAFT_INVENTORY

#include "minicraft.h"

#include "item.h"

#define INVENTORY_SIZE (64)

struct Inventory {
    u8 size;
    struct item_Data items[INVENTORY_SIZE];
};

extern struct Inventory inventory;

extern bool inventory_add_resource(struct Inventory *inventory, u8 item_type);

extern bool inventory_add_top(struct Inventory *inventory,
                              struct item_Data *data);

extern void inventory_remove(struct Inventory *inventory, u8 slot,
                             struct item_Data *removed_item);

#endif // MINICRAFT_INVENTORY
