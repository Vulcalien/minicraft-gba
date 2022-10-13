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
#include "images.h"

const u8 light_tileset[32 * 3] = {
    0xdd, 0xdd, 0xdd, 0xdd,
    0xdd, 0xdd, 0xdd, 0xdd,
    0xdd, 0xdd, 0xdd, 0xdd,
    0xdd, 0xdd, 0xdd, 0xdd,
    0xdd, 0xdd, 0xdd, 0xdd,
    0xdd, 0xdd, 0xdd, 0xdd,
    0xdd, 0xdd, 0xdd, 0xdd,
    0xdd, 0xdd, 0xdd, 0xdd,

    0xd0, 0xd0, 0xd0, 0xd0,
    0x0d, 0x0d, 0x0d, 0x0d,
    0xd0, 0xd0, 0xd0, 0xd0,
    0x0d, 0x0d, 0x0d, 0x0d,
    0xd0, 0xd0, 0xd0, 0xd0,
    0x0d, 0x0d, 0x0d, 0x0d,
    0xd0, 0xd0, 0xd0, 0xd0,
    0x0d, 0x0d, 0x0d, 0x0d,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

const u8 light_sprite_tileset[32 * 80] = {
    // Player Light
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x10,
    0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x10,
    0x00, 0x00, 0x00, 0x01,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x10, 0x10, 0x10,
    0x00, 0x01, 0x01, 0x01,
    0x10, 0x10, 0x11, 0x11,
    0x01, 0x01, 0x01, 0x01,
    0x11, 0x11, 0x11, 0x11,
    0x01, 0x11, 0x11, 0x11,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x10, 0x10, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x00,
    0x11, 0x10, 0x10, 0x10,
    0x11, 0x01, 0x01, 0x01,
    0x11, 0x11, 0x11, 0x10,
    0x11, 0x11, 0x01, 0x01,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x10, 0x10, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x11, 0x11,
    0x00, 0x00, 0x01, 0x01,
    0x00, 0x10, 0x11, 0x11,
    0x00, 0x01, 0x01, 0x11,
    0x10, 0x10, 0x11, 0x11,
    0x01, 0x01, 0x01, 0x11,
    0x00, 0x11, 0x11, 0x11,
    0x00, 0x01, 0x01, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x10, 0x10, 0x00,
    0x01, 0x00, 0x00, 0x00,
    0x11, 0x10, 0x10, 0x00,
    0x01, 0x01, 0x00, 0x00,
    0x11, 0x10, 0x10, 0x00,
    0x11, 0x01, 0x01, 0x00,
    0x11, 0x11, 0x10, 0x10,
    0x11, 0x01, 0x00, 0x00,

    0x10, 0x11, 0x11, 0x11,
    0x01, 0x01, 0x11, 0x11,
    0x00, 0x11, 0x11, 0x11,
    0x00, 0x01, 0x01, 0x11,
    0x10, 0x10, 0x11, 0x11,
    0x00, 0x01, 0x01, 0x11,
    0x00, 0x10, 0x11, 0x11,
    0x00, 0x01, 0x01, 0x01,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x01, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x10, 0x10,
    0x11, 0x01, 0x01, 0x00,
    0x11, 0x11, 0x10, 0x10,
    0x11, 0x01, 0x00, 0x00,
    0x11, 0x10, 0x10, 0x00,
    0x11, 0x01, 0x00, 0x00,
    0x11, 0x10, 0x10, 0x00,
    0x01, 0x01, 0x00, 0x00,

    0x00, 0x00, 0x11, 0x11,
    0x00, 0x00, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x10,
    0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x10,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x11, 0x11, 0x11, 0x11,
    0x01, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x01, 0x01, 0x01, 0x01,
    0x10, 0x10, 0x11, 0x11,
    0x01, 0x01, 0x01, 0x01,
    0x00, 0x10, 0x10, 0x10,
    0x00, 0x00, 0x00, 0x00,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x01,
    0x11, 0x11, 0x11, 0x10,
    0x01, 0x01, 0x01, 0x01,
    0x11, 0x10, 0x10, 0x10,
    0x01, 0x01, 0x01, 0x00,
    0x10, 0x10, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x11, 0x10, 0x10, 0x00,
    0x01, 0x00, 0x00, 0x00,
    0x10, 0x10, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    // Lantern Light
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x10,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01,
    0x10, 0x10, 0x10, 0x10,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01,
    0x10, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01,
    0x10, 0x10, 0x10, 0x10,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x00,
    0x10, 0x10, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x10,
    0x00, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01,
    0x00, 0x10, 0x10, 0x10,

    0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x10, 0x10,
    0x01, 0x01, 0x01, 0x11,
    0x10, 0x10, 0x10, 0x10,
    0x01, 0x01, 0x11, 0x11,
    0x10, 0x10, 0x10, 0x10,
    0x01, 0x11, 0x11, 0x11,
    0x10, 0x10, 0x11, 0x11,

    0x01, 0x01, 0x01, 0x01,
    0x10, 0x10, 0x10, 0x10,
    0x01, 0x11, 0x11, 0x11,
    0x10, 0x10, 0x11, 0x10,
    0x11, 0x11, 0x11, 0x11,
    0x10, 0x11, 0x10, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x01, 0x01, 0x01,
    0x10, 0x10, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x10, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x11,
    0x10, 0x11, 0x10, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x01, 0x01, 0x01, 0x01,
    0x10, 0x10, 0x10, 0x00,
    0x01, 0x11, 0x01, 0x01,
    0x10, 0x10, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x01,
    0x10, 0x10, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x10, 0x11, 0x10,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x00,
    0x10, 0x10, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01,
    0x10, 0x10, 0x10, 0x10,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x10,

    0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x10, 0x10,
    0x01, 0x01, 0x01, 0x11,
    0x00, 0x10, 0x10, 0x10,
    0x01, 0x01, 0x11, 0x11,
    0x10, 0x10, 0x10, 0x11,
    0x01, 0x11, 0x11, 0x11,
    0x10, 0x10, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x10, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x01, 0x01, 0x01,
    0x10, 0x10, 0x10, 0x00,
    0x11, 0x11, 0x01, 0x01,
    0x11, 0x10, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x01,
    0x10, 0x10, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x10, 0x10, 0x10,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x00,
    0x10, 0x10, 0x00, 0x00,

    0x00, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x10,
    0x00, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x10,
    0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x10,

    0x01, 0x01, 0x11, 0x11,
    0x10, 0x10, 0x10, 0x11,
    0x01, 0x11, 0x11, 0x11,
    0x10, 0x10, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x10, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x10, 0x10, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x01,
    0x11, 0x11, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x10,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x10,

    0x01, 0x01, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x00,
    0x10, 0x10, 0x00, 0x00,
    0x11, 0x01, 0x01, 0x00,
    0x10, 0x10, 0x00, 0x00,
    0x11, 0x01, 0x01, 0x00,
    0x10, 0x10, 0x00, 0x00,

    0x00, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x10, 0x10,
    0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x10,
    0x00, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x10,
    0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x10,

    0x11, 0x11, 0x11, 0x11,
    0x10, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x10, 0x10, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x10, 0x11, 0x11, 0x11,
    0x01, 0x11, 0x11, 0x11,
    0x10, 0x10, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x10,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x10,

    0x11, 0x01, 0x01, 0x00,
    0x10, 0x10, 0x10, 0x00,
    0x11, 0x01, 0x01, 0x00,
    0x10, 0x10, 0x00, 0x00,
    0x11, 0x01, 0x01, 0x00,
    0x10, 0x10, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x00,
    0x10, 0x10, 0x00, 0x00,

    0x00, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x10,
    0x00, 0x00, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00,

    0x11, 0x11, 0x11, 0x11,
    0x10, 0x10, 0x10, 0x11,
    0x01, 0x11, 0x11, 0x11,
    0x10, 0x10, 0x11, 0x11,
    0x01, 0x01, 0x11, 0x11,
    0x10, 0x10, 0x10, 0x11,
    0x01, 0x11, 0x11, 0x11,
    0x00, 0x10, 0x10, 0x10,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x10, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x01,
    0x10, 0x10, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x10, 0x10, 0x10,

    0x11, 0x01, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x00,
    0x10, 0x10, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x01, 0x01, 0x11, 0x01,
    0x00, 0x00, 0x10, 0x10,
    0x01, 0x01, 0x01, 0x11,
    0x00, 0x10, 0x10, 0x10,
    0x00, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x10,

    0x11, 0x11, 0x11, 0x11,
    0x10, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x10, 0x10, 0x11, 0x11,
    0x11, 0x01, 0x11, 0x11,
    0x10, 0x10, 0x10, 0x10,
    0x01, 0x11, 0x11, 0x11,
    0x10, 0x10, 0x10, 0x10,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x10, 0x11, 0x10, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x10, 0x10, 0x11, 0x10,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x10, 0x11, 0x10, 0x11,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x10, 0x10, 0x10,

    0x11, 0x11, 0x11, 0x11,
    0x11, 0x11, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x11,
    0x11, 0x10, 0x11, 0x10,
    0x11, 0x11, 0x11, 0x01,
    0x10, 0x10, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x11,
    0x10, 0x10, 0x10, 0x10,

    0x11, 0x01, 0x11, 0x01,
    0x10, 0x10, 0x10, 0x00,
    0x11, 0x11, 0x01, 0x01,
    0x10, 0x10, 0x10, 0x10,
    0x11, 0x01, 0x01, 0x01,
    0x10, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x00,
    0x10, 0x10, 0x00, 0x00,

    0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x10, 0x10,
    0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x10,
    0x00, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x11, 0x01, 0x11, 0x11,
    0x10, 0x10, 0x10, 0x10,
    0x01, 0x11, 0x01, 0x11,
    0x10, 0x10, 0x10, 0x10,
    0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00,

    0x11, 0x11, 0x11, 0x01,
    0x10, 0x10, 0x10, 0x10,
    0x01, 0x11, 0x01, 0x11,
    0x10, 0x10, 0x10, 0x10,
    0x01, 0x01, 0x01, 0x01,
    0x10, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00,

    0x11, 0x01, 0x01, 0x01,
    0x10, 0x10, 0x10, 0x00,
    0x01, 0x01, 0x01, 0x01,
    0x10, 0x10, 0x00, 0x00,
    0x01, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x01, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};
