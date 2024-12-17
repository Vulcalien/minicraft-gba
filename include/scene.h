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
#ifndef MINICRAFT_SCENE
#define MINICRAFT_SCENE

#include "minicraft.h"

struct Scene {
    void (*init)(u8 flags);
    void (*tick)(void);
    void (*draw)(void);
};

extern const struct Scene *scene;

extern const struct Scene scene_prestart;

extern const struct Scene scene_start;
extern const struct Scene scene_instructions;
extern const struct Scene scene_about;
extern const struct Scene scene_options;

extern const struct Scene scene_game;
extern const struct Scene scene_transition;

extern const struct Scene scene_inventory;
extern const struct Scene scene_chest;
extern const struct Scene scene_crafting;

extern const struct Scene scene_pause;

extern const struct Scene scene_death;
extern const struct Scene scene_win;

extern u16 scene_death_timer;
extern u16 scene_win_timer;

INLINE void set_scene(const struct Scene *new_scene, u8 init_flags) {
    scene = new_scene;

    if((init_flags & 1) && scene->init)
        scene->init(init_flags);
}

#endif // MINICRAFT_SCENE
