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
#include "screen.h"

#include <gba/display.h>
#include <gba/background.h>
#include <gba/sprite.h>
#include <gba/window.h>
#include <memory.h>

#include "res/palettes/background.c"
#include "res/palettes/sprites.c"
#include "res/palettes/items.c"

#include "res/images/level.c"
#include "res/images/logo.c"
#include "res/images/font.c"
#include "res/images/gui.c"
#include "res/images/items_bg.c"
#include "res/images/level-light.c"

#include "res/images/entities.c"
#include "res/images/sparks.c"
#include "res/images/items_spr.c"
#include "res/images/player-light.c"
#include "res/images/player-lantern-light.c"
#include "res/images/text-particle.c"

#define DISPLAY_CONTROL *((vu16 *) 0x04000000)

#define BG_PALETTE  ((vu16 *) 0x05000000)
#define SPR_PALETTE ((vu16 *) 0x05000200)

#define LOAD_TILESET(charblock, offset, tileset)      \
    memcpy32(                                         \
        display_charblock(charblock) + (offset) * 16, \
        tileset,                                      \
        sizeof(tileset)                               \
    )

#define LOAD_PALETTE(dest, palette)          \
    memcpy32(dest, palette, sizeof(palette))

void screen_init(void) {
    DISPLAY_CONTROL = 0       | // Video mode
                      1 << 6  | // OBJ Character mapping (1 is linear)
                      1 << 7  | // Forced Blank
                      0 << 8  | // Enable BG 0
                      1 << 9  | // Enable BG 1
                      0 << 10 | // Enable BG 2
                      1 << 11 | // Enable BG 3
                      1 << 12;  // Enable OBJ

    window_config(WINDOW_OUT, NULL);

    // Filter out the light layer when inside OBJ Window
    window_enable(WINDOW_SPR);
    window_config(WINDOW_SPR, &(struct Window) {
        .bg0 = true,
        .bg1 = true,
        .bg2 = false,
        .bg3 = true,

        .sprites = true,

        .effects = true
    });

    // Sky Background
    background_config(BG0, &(struct Background) {
        .priority = 3,
        .tileset  = 0,
        .tilemap  = 16
    });

    // Level Tiles
    background_config(BG1, &(struct Background) {
        .priority = 2,
        .tileset  = 0,
        .tilemap  = 17
    });

    // Light system
    background_config(BG2, &(struct Background) {
        .priority = 1,
        .tileset  = 1,
        .tilemap  = 18
    });

    // Text and GUI
    background_config(BG3, &(struct Background) {
        .priority = 0,
        .tileset  = 1,
        .tilemap  = 19
    });

    // load palettes
    LOAD_PALETTE(BG_PALETTE, background_palette);
    LOAD_PALETTE(SPR_PALETTE, sprites_palette);

    LOAD_PALETTE(BG_PALETTE  + 12 * 16, items_palette);
    LOAD_PALETTE(SPR_PALETTE + 12 * 16, items_palette);

    // make the first tile of charblock 1 fully transparent
    memset32(display_charblock(1), 0x00, 32);

    // load tilesets
    LOAD_TILESET(0, 0, level_tileset);
    LOAD_TILESET(1, 1, logo_tileset);
    LOAD_TILESET(1, 32, font_tileset);
    LOAD_TILESET(1, 96, gui_tileset);
    LOAD_TILESET(1, 128, items_bg_tileset);
    LOAD_TILESET(1, 192, level_light_tileset);

    LOAD_TILESET(4, 0, entities_tileset);
    LOAD_TILESET(4, 192, sparks_tileset);
    LOAD_TILESET(4, 256, items_spr_tileset);
    LOAD_TILESET(4, 320, player_light_tileset);
    LOAD_TILESET(4, 336, player_lantern_light_tileset);

    // load font sprites
    for(u32 i = 0; i <= 51; i++) {
        vu16 *dest = display_charblock(4) + (640 + i * 2) * 16;
        memcpy32(
            dest,
            (u8 *) (text_particle_tileset) + (i / 10) * 32,
            32
        );
        memcpy32(
            dest + 16,
            (u8 *) (text_particle_tileset) + (i % 10) * 32,
            32
        );
    }

    // set sky background
    for(u32 y = 0; y <= 18; y++)
        for(u32 x = 0; x <= 30; x++)
            BG0_TILEMAP[x + y * 32] = 0 | 9 << 12;

    // prepare prestart screen
    display_brighten(NULL, 16);
    for(u32 y = 0; y < 20; y++)
        for(u32 x = 0; x < 30; x++)
            BG3_TILEMAP[x + y * 32] = 32;

    sprite_hide_all();

    // disable forced blank
    DISPLAY_CONTROL &= ~(1 << 7);
}

IWRAM_SECTION
void vsync(void) {
    __asm__ volatile ("swi 0x05 << 16");
}

IWRAM_SECTION
void screen_write(const char *text, u8 palette, u32 x, u32 y) {
    const u32 x0 = x;

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

            u16 tile = c; // font tiles match ASCII values
            BG3_TILEMAP[x + y * 32] = tile | palette << 12;

            x++;
        }
    }
}

THUMB
void screen_draw_frame(const char *title, u32 x, u32 y, u32 w, u32 h) {
    w--;
    h--;

    // draw corners
    BG3_TILEMAP[(x)     + (y)     * 32] = 96 | 0 << 10 | 6 << 12;
    BG3_TILEMAP[(x + w) + (y)     * 32] = 96 | 1 << 10 | 6 << 12;
    BG3_TILEMAP[(x)     + (y + h) * 32] = 96 | 2 << 10 | 6 << 12;
    BG3_TILEMAP[(x + w) + (y + h) * 32] = 96 | 3 << 10 | 6 << 12;

    // draw vertical borders
    for(u32 yi = y + 1; yi <= y + h - 1; yi++) {
        BG3_TILEMAP[(x)     + yi * 32] = 98 | 0 << 10 | 6 << 12;
        BG3_TILEMAP[(x + w) + yi * 32] = 98 | 1 << 10 | 6 << 12;

        // draw background
        for(u32 xi = x + 1; xi <= x + w - 1; xi++)
            BG3_TILEMAP[xi + yi * 32] = 99 | 6 << 12;
    }

    // draw horizontal borders
    for(u32 xi = x + 1; xi <= x + w - 1; xi++) {
        BG3_TILEMAP[xi + (y)     * 32] = 97 | 0 << 10 | 6 << 12;
        BG3_TILEMAP[xi + (y + h) * 32] = 97 | 2 << 10 | 6 << 12;
    }

    screen_write(title, 10, x + 1, y);
}

static inline u32 ticks_to_seconds(u32 ticks) {
    // refresh time:    280_896    cycles = 4389   * 64 cycles
    // clock frequency: 16_777_216 Hz     = 262144 * 64 Hz
    //
    // framerate = (clock frequency) / (refresh time)
    // time = ticks / framerate
    //      = (ticks * 4389) / 262144
    //      = (ticks * 4389) >> 18

    return (((u64) ticks) * 4389) >> 18;
}

void screen_write_time(u32 ticks, u8 palette, u32 x, u32 y) {
    // NOTE this is different from the original game: here, seconds are
    // displayed even if hours is not 0 and there is a space between the
    // hours and minutes values.

    u32 seconds = ticks_to_seconds(ticks);
    u32 minutes = seconds / 60;
    u32 hours   = minutes / 60;

    seconds %= 60;
    minutes %= 60;

    char text[15] = { 0 };

    u8 offset = 0;
    if(hours > 0) {
        itoa(hours, 10, text, 5, false);
        offset += 1 + (hours > 9)   + (hours > 99) +
                      (hours > 999) + (hours > 9999);
        text[offset++] = 'H';
        text[offset++] = ' ';
    }

    itoa(minutes, 10, text + offset, 2, true);
    offset += 2;
    text[offset++] = 'M';
    text[offset++] = ' ';

    itoa(seconds, 10, text + offset, 2, true);
    offset += 2;
    text[offset++] = 'S';

    screen_write(text, palette, x, y);
}

void screen_set_bg_palette_color(u8 palette, u8 index, u16 color) {
    BG_PALETTE[palette * 16 + index] = color;
}

void screen_load_active_item_palette(u8 palette) {
    memcpy32(BG_PALETTE + 11 * 16, items_palette + 16 * palette, 32);
    screen_set_bg_palette_color(11, 15, 0x0421);
}

void screen_update_level_specific(void) {
    // sky background
    if(current_level == 4)
        DISPLAY_CONTROL |= 1 << 8;
    else
        DISPLAY_CONTROL &= ~(1 << 8);

    // light layer
    if(current_level < 3)
        DISPLAY_CONTROL |= 1 << 10;
    else
        DISPLAY_CONTROL &= ~(1 << 10);

    // dirt color
    const u16 dirt_colors[5][2] = {
        { 0x1ce7, 0x318c },
        { 0x1ce7, 0x318c },
        { 0x1ce7, 0x318c },
        { 0x1cea, 0x35b0 },
        { -1,     0x6318 }
    };
    for(u32 i = 0; i <= 3; i++) {
        screen_set_bg_palette_color(i, 12, dirt_colors[current_level][0]);
        screen_set_bg_palette_color(i, 13, dirt_colors[current_level][1]);
    }
    screen_set_bg_palette_color(0, 0, dirt_colors[current_level][1]);

    screen_set_bg_palette_color(8, 2, dirt_colors[current_level][0]);
    screen_set_bg_palette_color(8, 1, dirt_colors[current_level][1]);

    // farmland
    const u16 farmland_colors[5][2] = {
        { 0x1cea, 0x1445 },
        { 0x1cea, 0x1445 },
        { 0x1cea, 0x1445 },
        { 0x210e, 0x1869 },
        { -1, -1 }
    };
    screen_set_bg_palette_color(4, 4, farmland_colors[current_level][0]);
    screen_set_bg_palette_color(4, 5, farmland_colors[current_level][1]);

    // ore/cloud cactus
    const u16 ore_colors[5][3] = {
        { 0x733c, 0x44b1, 0x1445 }, // gem
        { 0x5fbd, 0x2ed6, 0x0cc6 }, // gold
        { 0x673b, 0x35b0, 0x0844 }, // iron
        { -1, -1, -1 },
        { 0x7bde, 0x4a52, 0x1ce7 }  // cloud cactus
    };
    screen_set_bg_palette_color(3, 6, ore_colors[current_level][0]);
    screen_set_bg_palette_color(3, 5, ore_colors[current_level][1]);
    screen_set_bg_palette_color(3, 4, ore_colors[current_level][2]);

    // liquid
    const u16 liquid_colors[2][2] = {
        { 0x14b3, 0x21d7 },
        { 0x4442, 0x4d08 }
    };
    for(u32 i = 3; i <= 4; i++) {
        screen_set_bg_palette_color(i, 1, liquid_colors[current_level > 0][0]);
        screen_set_bg_palette_color(i, 2, liquid_colors[current_level > 0][1]);
        screen_set_bg_palette_color(i, 3, liquid_colors[current_level > 0][0]);
    }
}
