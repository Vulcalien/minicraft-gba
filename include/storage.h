/* Copyright 2022-2024 Vulcalien
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
#ifndef MINICRAFT_STORAGE
#define MINICRAFT_STORAGE

#include "minicraft.h"

extern bool storage_check(void);
extern bool storage_verify_checksum(void);

extern void storage_srand(void);
extern void storage_load_options(void);

extern void storage_load(void);
extern void storage_save(void);

#endif // MINICRAFT_STORAGE
