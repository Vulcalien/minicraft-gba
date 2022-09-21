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
#include "scene.h"

const struct Scene *scene = NULL;

#include "scene/prestart.c"

#include "scene/start.c"
#include "scene/instructions.c"
#include "scene/about.c"

#include "scene/game.c"
#include "scene/transition.c"

#include "scene/inventory.c"
#include "scene/chest.c"
#include "scene/crafting.c"

#include "scene/pause.c"

#include "scene/death.c"
#include "scene/win.c"

u16 scene_death_timer = 0;
u16 scene_win_timer = 0;
