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
#ifndef MINICRAFT_ENTITIES
#define MINICRAFT_ENTITIES

#include "minicraft.h"

#include "level.h"

#define ZOMBIE_ENTITY         (0)
#define SLIME_ENTITY          (1)
#define AIR_WIZARD_ENTITY     (2)
#define PLAYER_ENTITY         (3)
#define TEXT_PARTICLE_ENTITY  (4)
#define SMASH_PARTICLE_ENTITY (5)
#define SPARK_ENTITY          (6)
#define FURNITURE_ENTITY      (7) // TODO maybe split these?
#define LANTERN_ENTITY        (8)
#define ITEM_ENTITY           (9)

#define ETICK(name)\
    IWRAM_SECTION\
    static void name(struct Level *level, struct entity_Data *data)

#define EDRAW(name)\
    IWRAM_SECTION\
    static void name(struct Level *level, struct entity_Data *data,\
                     vu16 *sprite_attribs)

struct Entity {
    void (*tick)(struct Level *level, struct entity_Data *data);

    void (*draw)(struct Level *level, struct entity_Data *data,
                 vu16 *sprite_attribs);
};

#define ENTITY_S(data)\
    (&entity_list[data->type])

#define ENTITY_TYPES (10)
extern const struct Entity entity_list[ENTITY_TYPES];

#endif // MINICRAFT_ENTITIES
