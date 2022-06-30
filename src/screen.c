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
#include "screen.h"

#include "images.h"

#define DISPLAY_CONTROL *((vu16 *) 0x04000000)
#define VCOUNT          *((vu16 *) 0x04000006)

#define CHAR_BLOCK_0 ((vu16 *) 0x06000000)
#define CHAR_BLOCK_1 ((vu16 *) 0x06004000)
#define CHAR_BLOCK_2 ((vu16 *) 0x06008000)
#define CHAR_BLOCK_3 ((vu16 *) 0x0600c000)

#define SPR_TILESET ((vu16 *) 0x06010000)

#define BG0_CONTROL *((vu16 *) 0x04000008)
#define BG1_CONTROL *((vu16 *) 0x0400000a)
#define BG2_CONTROL *((vu16 *) 0x0400000c)
#define BG3_CONTROL *((vu16 *) 0x0400000e)

#define BG_PALETTE  ((vu16 *) 0x05000000)
#define SPR_PALETTE ((vu16 *) 0x05000200)

static void load_tileset(vu16 *dest, const u8 *tileset, u32 size,
                         u8 zero_conversion) {
    for(u32 i = 0; i < size; i++) {
        u8 b0 = tileset[i * 2];
        u8 b1 = tileset[i * 2 + 1];

        if((b0 & 0x0f) == 0)
            b0 |= zero_conversion;
        if((b0 & 0xf0) == 0)
            b0 |= (zero_conversion << 4);

        if((b1 & 0x0f) == 0)
            b1 |= zero_conversion;
        if((b1 & 0xf0) == 0)
            b1 |= (zero_conversion << 4);

        // in a byte, lower 4 bits are for left, upper 4 bits for right
        b0 = (b0 << 4) | (b0 >> 4);
        b1 = (b1 << 4) | (b1 >> 4);

        dest[i] = (b1 << 8) | b0;
    }
}

#define LOAD_TILESET_CONVERT(dest, tileset, zero_conversion)\
    load_tileset(dest, tileset, sizeof(tileset) / sizeof(u16), zero_conversion)

#define LOAD_TILESET(dest, tileset)\
    LOAD_TILESET_CONVERT(dest, tileset, 0)

#define LOAD_PALETTE(dest, palette)\
    memcpy16(dest, palette, sizeof(palette) / sizeof(u16))

// TODO fix visual glitches at startup
void screen_init(void) {
    // TODO enable BG2
    DISPLAY_CONTROL = (0)       | // Video mode
                      (1 << 6)  | // OBJ Character mapping (1 is linear)
                      (1 << 8)  | // Enable BG 0
                      (1 << 9)  | // Enable BG 1
                      (0 << 10) | // Enable BG 2
                      (1 << 11) | // Enable BG 3
                      (1 << 12);  // Enable OBJ

    // Level tiles - 1st layer
    BG0_CONTROL = (3)       | // BG Priority (0 is highest, 3 is lowest)
                  (0 << 2)  | // Tileset character block
                  (0 << 7)  | // Color mode (0 is 4bpp with 16/16 palettes)
                  (16 << 8) | // Tilemap screen block
                  (0 << 14);  // BG size (0 is 256x256)

    // Level tiles - 2nd layer
    BG1_CONTROL = (2)       | // BG Priority (0 is highest, 3 is lowest)
                  (0 << 2)  | // Tileset character block
                  (0 << 7)  | // Color mode (0 is 4bpp with 16/16 palettes)
                  (17 << 8) | // Tilemap screen block
                  (0 << 14);  // BG size (0 is 256x256)

    // Light system ???
    // BG2_CONTROL = ...

    // Text and GUI
    BG3_CONTROL = (0)       | // BG Priority (0 is highest, 3 is lowest)
                  (1 << 2)  | // Tileset character block
                  (0 << 7)  | // Color mode (0 is 4bpp with 16/16 palettes)
                  (19 << 8) | // Tilemap screen block
                  (0 << 14);  // BG size (0 is 256x256)

    // load palettes
    LOAD_PALETTE(BG_PALETTE,  bg_palette);
    LOAD_PALETTE(SPR_PALETTE, sprite_palette);

    LOAD_PALETTE(BG_PALETTE  + 13 * 16, item_palette);
    LOAD_PALETTE(SPR_PALETTE + 13 * 16, item_palette);

    // load tilesets
    LOAD_TILESET(CHAR_BLOCK_0, level_tileset);
    LOAD_TILESET(CHAR_BLOCK_1, gui_tileset);

    LOAD_TILESET(SPR_TILESET, sprite_tileset);

    LOAD_TILESET_CONVERT(CHAR_BLOCK_1 + (32 * 128) / 2, item_tileset, 0xf);
}

void vsync(void) {
    while(VCOUNT >= SCREEN_H);
    while(VCOUNT < SCREEN_H);
}

void screen_write(const char *text, u8 palette, u8 x, u8 y) {
    const u8 x0 = x;

    for(u32 i = 0; text[i] != '\0'; i++) {
        const char c = text[i];

        if(c == '\n') {
            x = x0;
            y++;

            if(y >= 20)
                break;
        } else {
            if(x >= 30)
                continue;

            u16 tile = 29 + (c - 32);
            BG3_TILEMAP[x + y * 32] = tile | palette << 12;

            x++;
        }
    }
}

void screen_set_bg_palette_color(u8 palette, u8 index, u16 color) {
    BG_PALETTE[palette * 16 + index] = color;
}
