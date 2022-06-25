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
#ifndef MINICRAFT_MENU
#define MINICRAFT_MENU

#include "minicraft.h"

struct Menu {
    void (*init)(void);
    void (*tick)(void);
    void (*draw)(void);
};

extern const struct Menu *menu;

extern const struct Menu menu_start;
extern const struct Menu menu_instructions;
extern const struct Menu menu_about;

inline void set_menu(const struct Menu *new_menu, bool should_init) {
    menu = new_menu;

    if(should_init && menu && menu->init)
        menu->init();
}

#endif // MINICRAFT_MENU
