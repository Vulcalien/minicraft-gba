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

#include "images/level.c"

// TODO organize this mess...
const u16 bg_palette[256] = {
    // --- palette 0 --- (grass, flower, tree, sapling)
    // BG color
    0x7c1f,

    // grass
    0x328c, 0x4751, 0x19c6,

    // flower (last color is shared with tree and sapling)
    0x7bde, 0x2ed6, 0x08a2,

    // tree
    0x372d, 0x2655, 0x1d8f,

    // sapling
    0x2268, 0x2709,

    //unused
    0, 0, 0,

    // dirt (level-specific)
    0x35b0, // TODO make -1

    // --- palette 1 --- (rock and stairs)
    // transparent
    0,

    // rock
    0x6318, 0x4a52, 0x1ce7, 0x7bde,

    // stairs
    0x0421,

    // unused
    0, 0, 0, 0, 0, 0, 0, 0, 0,

    // dirt (level-specific)
    0x35b0, // TODO make -1

    // --- palette 2 --- (water+dirt, ore/cloud cactus)
    // transparent
    0,

    // water
    0x4442, 0x4d08,

    // ore/cloud cactus (level-specific)
    0x673b, 0x35b0, 0x0844,

    // unused
    0, 0, 0, 0, 0, 0, 0, 0,

    // dirt (level-specific)
    0x1cea, 0x35b0,

    // --- palette 3 --- (water+sand, farmland and wheat)
    // transparent
    0,

    // water
    0x4442, 0x4d08,

    // farmland (level-specific)
    0x35b0, 0x210e, 0x1869,

    // wheat
    0x2709, 0x2268, 0x3354, 0x26af, 0x3778, 0x0421, 0x0000,

    // unused
    0,

    // sand
    0x2ed6, 0x379c,

    // --- palette 4 --- (sand, cactus)
    // transparent
    0,

    // sand
    0x379c, 0x2ed6, 0x2ed6, 0x539c,

    // cactus
    0x2709, 0x2268, 0x1144,

    // unused
    0, 0, 0, 0, 0, 0, 0,

    // dirt (level-specific)
    0x35b0,

    // --- palette 5 --- (hole+dirt)
    // transparent
    0,

    // hole
    0x1ce7, 0x0cc6,

    // unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    // dirt (level-specific)
    0x1cea, 0x35b0,

    // --- palette 6 --- (hole+sand)
    // transparent
    0,

    // hole
    0x1ce7, 0x0cc6,

    // unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    // sand
    0x2ed6, 0x379c,

    // --- palette 7 --- (cloud)
    // transparent
    0,

    // cloud
    0x6318, 0, 0x4a52, 0x7bde,

    // unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    // --- palette 8 --- (hard rock)
    // transparent
    0,

    // hard rock
    0x5652, 0x3d8c, 0x1021, 0x6f18,

    // unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
