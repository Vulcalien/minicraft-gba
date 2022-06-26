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

const u8 sprite_tileset[32 * 96] = {
    // zombie/player down 0
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x03, 0x33, 0x33,
    0x00, 0x03, 0x33, 0x13,
    0x00, 0x32, 0x31, 0x31,
    0x00, 0x32, 0x33, 0x11,

    // zombie/player down 1
    0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x31, 0x33, 0x00, 0x00,
    0x13, 0x13, 0x30, 0x00,
    0x11, 0x32, 0x23, 0x00,

    // zombie/player down 2
    0x00, 0x03, 0x33, 0x33,
    0x00, 0x00, 0x32, 0x22,
    0x00, 0x00, 0x32, 0x23,
    0x00, 0x00, 0x03, 0x22,
    0x00, 0x00, 0x03, 0x22,
    0x00, 0x00, 0x03, 0x22,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x00, 0x00,

    // zombie/player down 3
    0x33, 0x22, 0x23, 0x00,
    0x23, 0x11, 0x23, 0x00,
    0x23, 0x11, 0x30, 0x00,
    0x32, 0x33, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x30, 0x00, 0x00, 0x00,
    0x30, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    // zombie/player up 0
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x03, 0x33, 0x33,
    0x00, 0x32, 0x31, 0x33,
    0x00, 0x32, 0x33, 0x11,

    // zombie/player up 1
    0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x33, 0x13, 0x00, 0x00,
    0x11, 0x33, 0x00, 0x00,

    // zombie/player up 2
    0x00, 0x32, 0x33, 0x33,
    0x00, 0x03, 0x32, 0x22,
    0x00, 0x00, 0x32, 0x22,
    0x00, 0x00, 0x33, 0x22,
    0x00, 0x00, 0x03, 0x22,
    0x00, 0x00, 0x03, 0x22,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x00, 0x00,

    // zombie/player up 3
    0x33, 0x22, 0x30, 0x00,
    0x22, 0x22, 0x13, 0x00,
    0x32, 0x31, 0x13, 0x00,
    0x32, 0x33, 0x30, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x30, 0x00, 0x00, 0x00,
    0x30, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    // zombie/player 1st right 0
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x13,
    0x00, 0x00, 0x03, 0x11,

    // zombie/player 1st right 1
    0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x31, 0x13, 0x00, 0x00,
    0x13, 0x13, 0x00, 0x00,
    0x11, 0x13, 0x00, 0x00,
    0x11, 0x30, 0x00, 0x00,

    // zombie/player 1st right 2
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x00, 0x32,
    0x00, 0x00, 0x00, 0x32,
    0x00, 0x00, 0x00, 0x31,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x00, 0x32,
    0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x00,

    // zombie/player 1st right 3
    0x33, 0x00, 0x00, 0x00,
    0x23, 0x00, 0x00, 0x00,
    0x23, 0x00, 0x00, 0x00,
    0x13, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x00, 0x00,
    0x23, 0x00, 0x00, 0x00,
    0x30, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    // zombie/player 2nd right 0
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x13,
    0x00, 0x00, 0x33, 0x11,

    // zombie/player 2nd right 1
    0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x31, 0x13, 0x00, 0x00,
    0x13, 0x13, 0x00, 0x00,
    0x11, 0x13, 0x00, 0x00,
    0x11, 0x33, 0x00, 0x00,

    // zombie/player 2nd right 2
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x03, 0x12, 0x22,
    0x00, 0x03, 0x12, 0x32,
    0x00, 0x00, 0x33, 0x22,
    0x00, 0x00, 0x32, 0x23,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    // zombie/player 2nd right 3
    0x33, 0x31, 0x30, 0x00,
    0x23, 0x21, 0x30, 0x00,
    0x23, 0x33, 0x00, 0x00,
    0x22, 0x30, 0x00, 0x00,
    0x22, 0x30, 0x00, 0x00,
    0x32, 0x23, 0x00, 0x00,
    0x03, 0x33, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    // zombie/player 3rd right 0
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x13,
    0x00, 0x00, 0x33, 0x11,

    // zombie/player 3rd right 1
    0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x31, 0x13, 0x00, 0x00,
    0x13, 0x13, 0x00, 0x00,
    0x11, 0x13, 0x00, 0x00,
    0x11, 0x33, 0x00, 0x00,

    // zombie/player 3rd right 2
    0x00, 0x03, 0x13, 0x33,
    0x00, 0x03, 0x12, 0x32,
    0x00, 0x00, 0x33, 0x32,
    0x00, 0x00, 0x03, 0x22,
    0x00, 0x00, 0x03, 0x22,
    0x00, 0x00, 0x32, 0x23,
    0x00, 0x00, 0x33, 0x30,
    0x00, 0x00, 0x00, 0x00,

    // zombie/player 3rd right 3
    0x33, 0x33, 0x00, 0x00,
    0x22, 0x21, 0x30, 0x00,
    0x23, 0x21, 0x30, 0x00,
    0x22, 0x33, 0x00, 0x00,
    0x32, 0x23, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    // -----

    // slime still 0
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x32, 0x22,

    // slime still 1
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x22, 0x23, 0x00, 0x00,

    // slime still 2
    0x00, 0x03, 0x22, 0x22,
    0x00, 0x32, 0x24, 0x22,
    0x00, 0x32, 0x22, 0x22,
    0x00, 0x32, 0x22, 0x22,
    0x00, 0x03, 0x32, 0x22,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    // slime still 3
    0x22, 0x22, 0x30, 0x00,
    0x22, 0x22, 0x23, 0x00,
    0x22, 0x22, 0x23, 0x00,
    0x22, 0x22, 0x23, 0x00,
    0x22, 0x23, 0x30, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    // slime jumping 0
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x03, 0x22,
    0x00, 0x00, 0x32, 0x22,
    0x00, 0x00, 0x32, 0x42,

    // slime jumping 1
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x00, 0x00,
    0x22, 0x30, 0x00, 0x00,
    0x22, 0x23, 0x00, 0x00,
    0x22, 0x23, 0x00, 0x00,

    // slime jumping 2
    0x00, 0x03, 0x22, 0x42,
    0x00, 0x03, 0x22, 0x22,
    0x00, 0x03, 0x22, 0x22,
    0x00, 0x03, 0x22, 0x22,
    0x00, 0x00, 0x32, 0x22,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    // slime jumping 3
    0x22, 0x22, 0x30, 0x00,
    0x22, 0x22, 0x30, 0x00,
    0x22, 0x22, 0x30, 0x00,
    0x22, 0x22, 0x30, 0x00,
    0x22, 0x23, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    // Player swimming

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x03, 0x33, 0x33,
    0x00, 0x03, 0x33, 0x13,
    0x00, 0x32, 0x31, 0x31,
    0x04, 0x32, 0x33, 0x11,

    0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x31, 0x33, 0x40, 0x00,
    0x13, 0x13, 0x34, 0x00,
    0x11, 0x32, 0x23, 0x40,

    0x00, 0x45, 0x56, 0x66,
    0x00, 0x04, 0x45, 0x55,
    0x00, 0x00, 0x04, 0x44,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x66, 0x65, 0x54, 0x00,
    0x55, 0x54, 0x40, 0x00,
    0x44, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x03, 0x33, 0x33,
    0x00, 0x32, 0x31, 0x33,
    0x04, 0x32, 0x33, 0x11,

    0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x33, 0x33, 0x40, 0x00,
    0x33, 0x13, 0x54, 0x00,
    0x11, 0x33, 0x65, 0x40,

    0x00, 0x45, 0x56, 0x66,
    0x00, 0x04, 0x45, 0x55,
    0x00, 0x00, 0x04, 0x44,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x66, 0x65, 0x54, 0x00,
    0x55, 0x54, 0x40, 0x00,
    0x44, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x04, 0x33, 0x33,
    0x00, 0x45, 0x33, 0x13,
    0x04, 0x56, 0x63, 0x11,

    0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x31, 0x13, 0x00, 0x00,
    0x13, 0x13, 0x40, 0x00,
    0x11, 0x13, 0x54, 0x00,
    0x11, 0x36, 0x65, 0x40,

    0x00, 0x45, 0x56, 0x66,
    0x00, 0x04, 0x45, 0x55,
    0x00, 0x00, 0x04, 0x44,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x66, 0x65, 0x54, 0x00,
    0x55, 0x54, 0x40, 0x00,
    0x44, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x04, 0x33, 0x33,
    0x00, 0x45, 0x33, 0x13,
    0x04, 0x56, 0x33, 0x11,

    0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x31, 0x13, 0x00, 0x00,
    0x13, 0x13, 0x40, 0x00,
    0x11, 0x13, 0x54, 0x00,
    0x11, 0x33, 0x65, 0x40,

    0x00, 0x45, 0x56, 0x66,
    0x00, 0x04, 0x45, 0x55,
    0x00, 0x00, 0x04, 0x44,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x66, 0x65, 0x54, 0x00,
    0x55, 0x54, 0x40, 0x00,
    0x44, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x04, 0x33, 0x33,
    0x00, 0x45, 0x33, 0x13,
    0x04, 0x56, 0x33, 0x11,

    0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x31, 0x13, 0x00, 0x00,
    0x13, 0x13, 0x40, 0x00,
    0x11, 0x13, 0x54, 0x00,
    0x11, 0x33, 0x65, 0x40,

    0x00, 0x45, 0x56, 0x66,
    0x00, 0x04, 0x45, 0x55,
    0x00, 0x00, 0x04, 0x44,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x66, 0x65, 0x54, 0x00,
    0x55, 0x54, 0x40, 0x00,
    0x44, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x03, 0x33, 0x33,
    0x00, 0x03, 0x33, 0x13,
    0x00, 0x32, 0x31, 0x31,
    0x00, 0x32, 0x33, 0x11,

    0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x31, 0x33, 0x00, 0x00,
    0x13, 0x13, 0x30, 0x00,
    0x11, 0x32, 0x23, 0x00,

    0x00, 0x04, 0x45, 0x55,
    0x00, 0x00, 0x04, 0x44,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x55, 0x54, 0x40, 0x00,
    0x44, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x03, 0x33, 0x33,
    0x00, 0x32, 0x31, 0x33,
    0x00, 0x32, 0x33, 0x11,

    0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x33, 0x13, 0x40, 0x00,
    0x11, 0x33, 0x54, 0x00,

    0x00, 0x04, 0x45, 0x55,
    0x00, 0x00, 0x04, 0x44,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x55, 0x54, 0x40, 0x00,
    0x44, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x04, 0x33, 0x13,
    0x00, 0x45, 0x53, 0x11,

    0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x31, 0x13, 0x00, 0x00,
    0x13, 0x13, 0x00, 0x00,
    0x11, 0x13, 0x40, 0x00,
    0x11, 0x35, 0x54, 0x00,

    0x00, 0x04, 0x45, 0x55,
    0x00, 0x00, 0x04, 0x44,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x55, 0x54, 0x40, 0x00,
    0x44, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x04, 0x33, 0x13,
    0x00, 0x45, 0x33, 0x11,

    0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x31, 0x13, 0x00, 0x00,
    0x13, 0x13, 0x00, 0x00,
    0x11, 0x13, 0x40, 0x00,
    0x11, 0x33, 0x54, 0x00,

    0x00, 0x04, 0x45, 0x55,
    0x00, 0x00, 0x04, 0x44,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x55, 0x54, 0x40, 0x00,
    0x44, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x33,
    0x00, 0x00, 0x03, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x00, 0x33, 0x33,
    0x00, 0x04, 0x33, 0x13,
    0x00, 0x45, 0x33, 0x11,

    0x00, 0x00, 0x00, 0x00,
    0x33, 0x00, 0x00, 0x00,
    0x33, 0x30, 0x00, 0x00,
    0x33, 0x33, 0x00, 0x00,
    0x31, 0x13, 0x00, 0x00,
    0x13, 0x13, 0x00, 0x00,
    0x11, 0x13, 0x40, 0x00,
    0x11, 0x33, 0x54, 0x00,

    0x00, 0x04, 0x45, 0x55,
    0x00, 0x00, 0x04, 0x44,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x55, 0x54, 0x40, 0x00,
    0x44, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};
