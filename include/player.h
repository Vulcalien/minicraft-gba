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
#ifndef MINICRAFT_PLAYER
#define MINICRAFT_PLAYER

#include "minicraft.h"

#include "inventory.h"
#include "item.h"

extern struct Inventory player_inventory;
extern struct item_Data player_active_item;

extern u8 player_stamina;
extern u8 player_stamina_recharge_delay;

extern u16 player_invulnerable_time;

INLINE bool player_pay_stamina(u8 amount) {
    if(player_stamina >= amount) {
        player_stamina -= amount;
        return true;
    }
    return false;
}

#endif // MINICRAFT_PLAYER
