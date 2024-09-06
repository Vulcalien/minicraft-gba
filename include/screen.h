/* Copyright 2022-2023 Vulcalien
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
#ifndef MINICRAFT_SCREEN
#define MINICRAFT_SCREEN

#include "minicraft.h"

#define SCREEN_W (240)
#define SCREEN_H (160)

#define BG0_TILEMAP ((vu16 *) 0x06008000)
#define BG1_TILEMAP ((vu16 *) 0x06008800)
#define BG2_TILEMAP ((vu16 *) 0x06009000)
#define BG3_TILEMAP ((vu16 *) 0x06009800)

#define OAM ((vu16 *) 0x07000000)

extern void screen_init(void);

extern void vsync(void);

extern void screen_write(const char *text, u8 palette, u32 x, u32 y);
extern void screen_draw_frame(const char *title, u32 x, u32 y, u32 w, u32 h);
extern void screen_write_time(u32 ticks, u8 palette, u32 x, u32 y);

#define SCREEN_WRITE_NUMBER(number, radix, digits, zero_fill, palette, x, y)\
    do {\
        char text[(digits) + 1] = { 0 };\
        itoa((number), (radix), text, (digits), (zero_fill));\
        screen_write(text, (palette), (x), (y));\
    } while (0)

extern void screen_set_bg_palette_color(u8 palette, u8 index, u16 color);
extern void screen_load_active_item_palette(u8 palette);

extern void screen_update_level_specific(void);

#endif // MINICRAFT_SCREEN
