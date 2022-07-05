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
#include "images/gui.c"
#include "images/sprites.c"
#include "images/items.c"

// TODO organize this mess...
const u16 bg_palette[11 * 16] = {
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

    // white font - black bg
    0x7bde, 0x0421,

    // dirt (level-specific)
    0x1cea, 0x35b0,

    // --- palette 1 --- (rock, stairs, title)
    // transparent
    0,

    // rock
    0x6318, 0x4a52, 0x1ce7, 0x7bde,

    // stairs
    0x0421,

    // title
    0x479c, 0x2a0a, 0x24c3,

    // unused
    0, 0, 0,

    // gray font - black bg
    0x318c, 0x0421,

    // dirt (level-specific)
    0x1cea, 0x35b0,

    // --- palette 2 --- (water+dirt, ore/cloud cactus)
    // transparent
    0,

    // water
    0x4442, 0x4d08,

    // ore/cloud cactus (level-specific)
    0x673b, 0x35b0, 0x0844,

    // unused
    0, 0, 0, 0, 0, 0,

    // light gray font - black bg
    0x4a52, 0x0421,

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
    0x2709, 0x2268, 0x3354, 0x26af, 0x3778, 0x0421,

    // very light gray font - black bg
    0x6318, 0x0421,

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
    0, 0,

    // GUI window border
    0x6f18, 0x1021,

    // white font - blue bg
    0x7bde, 0x4442,

    // dirt (level-specific)
    0x1cea, 0x35b0,

    // --- palette 5 --- (hole+dirt, hp and stamina)
    // transparent
    0,

    // hole
    0x1ce7, 0x0cc6,

    // GUI background
    0x0421,

    // hp full
    0x0848, 0x14b3, 0x5299,

    // hp empty
    0x0844,

    // stamina full
    0x198c, 0x379c,

    // stamina empty (changes)
    0x0cc6,

    // unused
    0,

    // gray font - blue bg
    0x318c, 0x4442,

    // dirt (level-specific)
    0x1cea, 0x35b0,

    // --- palette 6 --- (hole+sand)
    // transparent
    0,

    // hole
    0x1ce7, 0x0cc6,

    // unused
    0, 0, 0, 0, 0, 0, 0, 0, 0,

    // light gray font - blue bg
    0x4a52, 0x4442,

    // sand
    0x2ed6, 0x379c,

    // --- palette 7 --- (cloud)
    // transparent
    0,

    // cloud
    0x6318, 0, 0x4a52, 0x7bde,

    // unused
    0, 0, 0, 0, 0, 0, 0,

    // very light gray font - blue bg
    0x6318, 0x4442,

    // unused
    0, 0,

    // --- palette 8 --- (hard rock)
    // transparent
    0,

    // hard rock
    0x5652, 0x3d8c, 0x1021, 0x6f18,

    // unused
    0, 0, 0, 0, 0, 0, 0,

    // yellow font - blue bg
    0x379c, 0x4442,

    // unused
    0, 0,
};

const u16 sprite_palette[11 * 16] = {
    // --- palette 0 --- (zombie and slime - level 1)
    // transparent
    0,

    // zombie
    0x2268, 0x4751, 0x08a2,

    // slime
    0x7bde,

    // unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    // --- palette 1 --- (zombie and slime - level 2)
    // transparent
    0,

    // zombie
    0x2268, 0x3df7, 0x0844,

    // slime
    0x7bde,

    // unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    // --- palette 2 --- (zombie and slime - level 3)
    // transparent
    0,

    // zombie
    0x2268, 0x6318, 0x1ce7,

    // slime
    0x7bde,

    // unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    // --- palette 3 --- (zombie and slime - level 4)
    // transparent
    0,

    // zombie
    0x1144, 0x1ce7, 0x0421,

    // slime
    0x4dad,

    // unused
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    // --- palette 4 --- (player)
    // transparent
    0,

    // player
    0x4279, 0x198c, 0x0844,

    // unused
    0,

    // water
    0x6273, 0x4d08, 0x4442,

    // unused
    0, 0, 0, 0, 0, 0, 0, 0,

    // --- palette 5 --- (mob damaged)
    // transparent
    0,

    // zombie/player
    0x7fff, 0x7fff, 0x7fff,

    // slime
    0x7fff,

    // water
    0x6273, 0x4d08, 0x4442,

    // unused
    0, 0, 0, 0, 0, 0, 0, 0,

    // --- palette 6 --- (furniture)
    // transparent
    0,

    // workbench
    0x0844, 0x3255, 0x29b0,

    // furnace
    0x0421, 0x318c, 0x4a52,

    // oven
    0x3e52, 0x46f7,

    // anvil
    0x1ce7,

    // chest
    0x0cc6, 0x539c, 0x3231,

    // lantern
    0x7bde,
};

const u16 item_palette[4 * 16] = {
    // --- palette 0 --- (wood, acorn, powerglove, apple, iron ore, iron ingot,
    //                    dirt, workbench)
    // transparent
    0,

    0x2655, 0x3679, 0x0848,
    0x0844, 0x1d8f,
    0x2655,
    0x0c6c, 0x14b3,
    0x35b0, 0x673b,
    0x4255,
    0x3255, 0x29b0,

    // unused
    0,

    // background
    0x4442,

    // --- palette 1 --- (wheat, bread, gold ore, gold ingot, sand, chest,
    //                    flower, slime, seeds, cactus)
    // transparent
    0,

    0x379c, 0x2231, 0x0cc6,
    0x2ed6, 0x5fbd,
    0x539c, 0x3231,
    0x6318, 0x08a2,
    0x19c6, 0x2709,
    0x2268,

    // unused
    0, 0,

    // background
    0x4442,

    // --- palette 2 --- (anvil, furnace, coal, lantern, glass, oven, stone,
    //                    cloud, gem, cloth)
    // transparent
    0,

    0x0421, 0x318c, 0x1ce7,
    0x7bde,
    0x3e52, 0x46f7,
    0x4a52,
    0x6318,
    0x733c, 0x44b1, 0x1445,
    0x328c, 0x4751, 0x5166,

    // background
    0x4442,

    // --- palette 4 --- (tools)
    // transparent
    0,

    // handle and shadow
    0x29b0, 0x0844,

    // levels
    0x3255, 0x1ce7, 0x7bde, 0x379c, 0x5ea9,

    // unused
    0, 0, 0, 0, 0, 0, 0,

    // background
    0x4442
};
