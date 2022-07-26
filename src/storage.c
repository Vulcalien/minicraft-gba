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
#include "storage.h"

#include "level.h"

/*
Storage Layout (128 KB)

120 KB - 5 * level:
     7 KB - tiles
    13 KB - data
     4 KB - entities

3 KB - padding

4 KB - chest inventories
1 KB - player and other data:
    128 B - inventory
      3 B - active item

      1 B - stamina
      1 B - stamina recharge delay
      1 B - invulnerable time

      4 B - score

      1 B - chest count
 */

void storage_load(void) {

}

void storage_save(void) {

}
