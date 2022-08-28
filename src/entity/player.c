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
#include "entity.h"
#include "player.h"

#include "mob.h"
#include "input.h"
#include "item.h"
#include "furniture.h"
#include "scene.h"
#include "crafting.h"

#define MAX_HP      (10)
#define MAX_STAMINA (10)

EWRAM_BSS_SECTION
struct Inventory player_inventory;

struct item_Data player_active_item;

u8 player_stamina = MAX_STAMINA;
u8 player_stamina_recharge_delay = 0;

u8 player_invulnerable_time = 0;

static u32 player_tick_time = 0;

void entity_add_player(struct Level *level, u8 xt, u8 yt) {
    struct entity_Data *data  = &level->entities[0];
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    data->type = PLAYER_ENTITY;

    data->x = (xt << 4) + 8;
    data->y = (yt << 4) + 8;

    mob_data->hp = MAX_HP;
    mob_data->dir = 2;

    player_inventory.size = 0;
    player_active_item.type = -1;

    struct item_Data item_to_add;

    item_to_add.type = POWERGLOVE_ITEM;
    inventory_add(&player_inventory, &item_to_add, 0);

    item_to_add.type = WORKBENCH_ITEM;
    inventory_add(&player_inventory, &item_to_add, 0);

    level_add_entity(level, 0);
}

static inline void player_hurt_entities(struct Level *level, struct entity_Data *data) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    const u8 dir = mob_data->dir;
    const u8 range = 20;
    i32 x0 = data->x     - ((dir & 1) == 0) * 8 - (dir == 1) * range + (dir == 3) * 4;
    i32 y0 = data->y - 2 - ((dir & 1) == 1) * 8 - (dir == 0) * range + (dir == 2) * 4;
    i32 x1 = data->x     + ((dir & 1) == 0) * 8 + (dir == 3) * range - (dir == 1) * 4;
    i32 y1 = data->y - 2 + ((dir & 1) == 1) * 8 + (dir == 2) * range - (dir == 0) * 4;

    i32 xt0 = (x0 >> 4) - 1;
    i32 yt0 = (y0 >> 4) - 1;
    i32 xt1 = (x1 >> 4) + 1;
    i32 yt1 = (y1 >> 4) + 1;

    if(xt0 < 0) xt0 = 0;
    if(yt0 < 0) yt0 = 0;
    if(xt1 >= LEVEL_W) xt1 = LEVEL_W - 1;
    if(yt1 >= LEVEL_H) yt1 = LEVEL_H - 1;

    for(u32 yt = yt0; yt <= yt1; yt++) {
        for(u32 xt = xt0; xt <= xt1; xt++) {
            const u32 tile = xt + yt * LEVEL_W;

            for(u32 i = 0; i < SOLID_ENTITIES_IN_TILE; i++) {
                const u8 entity_id = level_solid_entities[tile][i];
                struct entity_Data *e_data = &level->entities[entity_id];

                switch(e_data->type) {
                    case ZOMBIE_ENTITY:
                    case SLIME_ENTITY:
                    case AIR_WIZARD_ENTITY:
                        break;

                    default:
                        continue;
                };

                if(entity_intersects(e_data, x0, y0, x1, y1)) {
                    u8 damage = 1 + rand() % 3;

                    if(player_active_item.type == SWORD_ITEM) {
                        const u8 level = player_active_item.tool_level;
                        damage += (level + 1) * 3 +
                                  rand() % (2 + level * level * 2);
                    } else if(player_active_item.type == AXE_ITEM) {
                        const u8 level = player_active_item.tool_level;
                        damage += (level + 1) * 2 +
                                  rand() % 4;
                    } else if(player_active_item.type < ITEM_TYPES) {
                        damage += 1;
                    }

                    mob_hurt(level, e_data, damage, dir);
                }
            }
        }
    }
}

static inline void player_hurt_tile(struct Level *level, struct entity_Data *data) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    const u8 dir = mob_data->dir;
    const u8 range = 12;
    i32 xt = (data->x     + ((dir == 3) - (dir == 1)) * range) >> 4;
    i32 yt = (data->y - 2 + ((dir == 2) - (dir == 0)) * range) >> 4;

    if(xt < 0 || xt >= LEVEL_W || yt < 0 || yt >= LEVEL_H)
        return;

    const struct Tile *tile = LEVEL_GET_TILE_S(level, xt, yt);
    if(tile->interact)
        tile->interact(level, xt, yt, &player_active_item);
}

static inline void player_take_furniture(struct Level *level, struct entity_Data *data) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    const u8 dir = mob_data->dir;
    const u8 range = 12;
    i32 x0 = data->x     - ((dir & 1) == 0) * 8 - (dir == 1) * range + (dir == 3) * 4;
    i32 y0 = data->y - 2 - ((dir & 1) == 1) * 8 - (dir == 0) * range + (dir == 2) * 4;
    i32 x1 = data->x     + ((dir & 1) == 0) * 8 + (dir == 3) * range - (dir == 1) * 4;
    i32 y1 = data->y - 2 + ((dir & 1) == 1) * 8 + (dir == 2) * range - (dir == 0) * 4;

    for(u32 i = 0; i < ENTITY_LIMIT; i++) {
        struct entity_Data *e_data = &level->entities[i];

        switch(e_data->type) {
            case WORKBENCH_ENTITY:
            case FURNACE_ENTITY:
            case OVEN_ENTITY:
            case ANVIL_ENTITY:
            case CHEST_ENTITY:
            case LANTERN_ENTITY:
                break;

            default:
                continue;
        };

        if(entity_intersects(e_data, x0, y0, x1, y1)) {
            // add the power glove to the inventory
            bool could_add = inventory_add(
                &player_inventory, &player_active_item, 0
            );

            if(could_add) {
                furniture_take(e_data);
                return;
            }
        }
    }
}

static inline void player_eat(struct Level *level, struct entity_Data *data) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;
    const struct Item *item = ITEM_S(&player_active_item);

    if(mob_data->hp < MAX_HP && player_pay_stamina(5)) {
        // FIXED BUG - Mob.java:91
        // eating while hurt, the food is used but hp is not recovered
        mob_data->hp += item->hp_gain;
        if(mob_data->hp > MAX_HP)
            mob_data->hp = MAX_HP;

        entity_add_text_particle(level, data->x, data->y, item->hp_gain, 2);

        player_active_item.count--;
        if(player_active_item.count == 0)
            player_active_item.type = -1;
    }
}

static inline void player_place(struct Level *level, struct entity_Data *data) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;
    const struct Item *item = ITEM_S(&player_active_item);

    const u8 dir = mob_data->dir;
    const u8 range = 12;
    i32 xt = (data->x     + ((dir == 3) - (dir == 1)) * range) >> 4;
    i32 yt = (data->y - 2 + ((dir == 2) - (dir == 0)) * range) >> 4;

    // even if it's out of bounds, it's ok
    // because nothing can be placed on rock
    u8 tile = LEVEL_GET_TILE(level, xt, yt);

    for(u32 i = 0; i < sizeof(item->placeable_on); i++) {
        if(tile == item->placeable_on[i]) {
            LEVEL_SET_TILE(level, xt, yt, item->placed_tile, 0);

            player_active_item.count--;
            if(player_active_item.count == 0)
                player_active_item.type = -1;

            break;
        }
    }
}

static inline void player_place_furniture(struct Level *level, struct entity_Data *data) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    const u8 dir = mob_data->dir;
    const u8 range = 12;
    i32 xt = (data->x     + ((dir == 3) - (dir == 1)) * range) >> 4;
    i32 yt = (data->y - 2 + ((dir == 2) - (dir == 0)) * range) >> 4;

    // even if it's out of bounds, it's ok
    // because rock is solid
    const struct Tile *tile = LEVEL_GET_TILE_S(level, xt, yt);

    if(!tile->is_solid) {
        if(entity_add_furniture(level, xt, yt, &player_active_item))
            player_active_item.type = -1;
    }
}

static inline void player_attack(struct Level *level, struct entity_Data *data) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    mob_data->walk_dist ^= 8;

    const u8 item_class = (player_active_item.type < ITEM_TYPES) ?
                          ITEM_S(&player_active_item)->class : -1;

    // add particles before the action: the item could change
    {
        u8 attack_particle_time;
        if(item_class == (u8) -1 || item_class == ITEMCLASS_TOOL)
            attack_particle_time = 5;
        else
            attack_particle_time = 10;

        entity_add_item_particle(level, attack_particle_time);
        entity_add_attack_particle(level, attack_particle_time);
    }

    switch(item_class) {
        case (u8) -1:
        case ITEMCLASS_TOOL:
            player_hurt_entities(level, data);
            player_hurt_tile(level, data);
            break;

        case ITEMCLASS_POWERGLOVE:
            player_take_furniture(level, data);
            break;

        case ITEMCLASS_FOOD:
            player_eat(level, data);
            break;

        case ITEMCLASS_PLACEABLE:
            player_place(level, data);
            break;

        case ITEMCLASS_FURNITURE:
            player_place_furniture(level, data);
            break;
    };
}

#define OPEN_CRAFTING_MENU(recipe_list)\
    do {\
        crafting_current_recipes = (struct crafting_Recipe *) &recipe_list;\
        crafting_current_recipes_size =\
            sizeof(recipe_list) / sizeof(struct crafting_Recipe);\
\
        set_scene(&scene_crafting, true);\
    } while(0)

static inline bool player_use(struct Level *level, struct entity_Data *data) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    const u8 dir = mob_data->dir;
    const u8 range = 12;
    i32 x0 = data->x     - ((dir & 1) == 0) * 8 - (dir == 1) * range + (dir == 3) * 4;
    i32 y0 = data->y - 2 - ((dir & 1) == 1) * 8 - (dir == 0) * range + (dir == 2) * 4;
    i32 x1 = data->x     + ((dir & 1) == 0) * 8 + (dir == 3) * range - (dir == 1) * 4;
    i32 y1 = data->y - 2 + ((dir & 1) == 1) * 8 + (dir == 2) * range - (dir == 0) * 4;

    for(u32 i = 0; i < ENTITY_LIMIT; i++) {
        struct entity_Data *e_data = &level->entities[i];

        if(entity_intersects(e_data, x0, y0, x1, y1)) {
            bool found = true;
            switch(e_data->type) {
                case WORKBENCH_ENTITY:
                    OPEN_CRAFTING_MENU(workbench_recipes);
                    break;
                case FURNACE_ENTITY:
                    OPEN_CRAFTING_MENU(furnace_recipes);
                    break;
                case OVEN_ENTITY:
                    OPEN_CRAFTING_MENU(oven_recipes);
                    break;
                case ANVIL_ENTITY:
                    OPEN_CRAFTING_MENU(anvil_recipes);
                    break;
                case CHEST_ENTITY:
                    furniture_set_opened_chest(e_data);
                    set_scene(&scene_chest, true);
                    break;

                default:
                    found = false;
            };

            if(found)
                return true;
        }
    }
    return false;
}

ETICK(player_tick) {
    gametime++;

    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    player_tick_time++;

    u8 on_tile = LEVEL_GET_TILE(level, data->x >> 4, data->y >> 4);

    if(on_tile == LIQUID_TILE && current_level == 0)
        mob_hurt(level, data, 4, mob_data->dir ^ 2);

    mob_tick(level, data);

    if(player_invulnerable_time > 0)
        player_invulnerable_time--;

    // check if on stairs
    static u8 on_stairs_delay = 0;
    if(on_tile == STAIRS_DOWN_TILE || on_tile == STAIRS_UP_TILE) {
        if(on_stairs_delay == 0) {
            set_scene(&scene_transition, true);

            if(on_tile == STAIRS_DOWN_TILE)
                current_level--;
            else
                current_level++;

            on_stairs_delay = 10;
            return;
        }
        on_stairs_delay = 10;
    } else if(on_stairs_delay > 0) {
        on_stairs_delay--;
    }

    // stamina
    static u8 stamina_recharge = 0;

    // TODO ugly code
    if(player_stamina_recharge_delay == 0 &&
       player_stamina == 0 &&
       stamina_recharge == 0)
        player_stamina_recharge_delay = 40;

    if(player_stamina_recharge_delay > 0)
        player_stamina_recharge_delay--;

    if(player_stamina_recharge_delay == 0) {
        stamina_recharge++;

        if(on_tile == LIQUID_TILE)
            stamina_recharge = 0;

        // FIXED BUG - Player.java:78-79
        // after using stamina, the first recovered point
        // takes 11 ticks, while the others take 10
        if(stamina_recharge == 10) {
            stamina_recharge = 0;

            if(player_stamina < MAX_STAMINA)
                player_stamina++;
        }
    }

    if(on_tile == LIQUID_TILE && player_tick_time % 60 == 0) {
        if(player_stamina > 0)
            player_stamina--;
        else
            mob_hurt(level, data, 1, mob_data->dir ^ 2);
    }

    // try adding furniture particle (if it already exists, this does nothing)
    if(ITEM_S(&player_active_item)->class == ITEMCLASS_FURNITURE)
        entity_add_furniture_particle(level);

    // movement
    i32 xm = (INPUT_DOWN(KEY_RIGHT) != 0) - (INPUT_DOWN(KEY_LEFT) != 0);
    i32 ym = (INPUT_DOWN(KEY_DOWN)  != 0) - (INPUT_DOWN(KEY_UP)   != 0);

    if((player_stamina_recharge_delay & 1) == 0) {
        static u8 swim_move_flag = 0;
        swim_move_flag ^= (on_tile == LIQUID_TILE);
        swim_move_flag &= (on_tile == LIQUID_TILE);

        if(!swim_move_flag)
            mob_move(level, data, xm, ym);
    }

    if(player_stamina > 0 && INPUT_CLICKED(KEY_A)) {
        player_stamina--;
        stamina_recharge = 0;

        player_attack(level, data);
    }

    if(INPUT_CLICKED(KEY_B)) {
        if(!player_use(level, data))
            set_scene(&scene_inventory, true);
    }

    if(INPUT_CLICKED(KEY_START))
        set_scene(&scene_pause, false);

    // DEBUG
    if(INPUT_CLICKED(KEY_L))
        storage_save();
    if(INPUT_CLICKED(KEY_R))
        storage_load();
}

EDRAW(player_draw) {
    struct mob_Data *mob_data = (struct mob_Data *) &data->data;

    const u8 dir = mob_data->dir;
    const u8 walk_dist = mob_data->walk_dist;
    const u8 hurt_time = mob_data->hurt_time;

    u8 on_tile = LEVEL_GET_TILE(level, data->x >> 4, data->y >> 4);

    u16 sprite = (dir == 0) * 12 +
                 (dir == 2) * 8  +
                 (dir & 1)  * 16;
    sprite += (dir & 1) * (
        ((walk_dist >> 3) & 1) * (4 + ((walk_dist >> 4) & 1) * 4)
    );

    // swimming
    sprite += (on_tile == LIQUID_TILE) *
              (20 + ((player_tick_time / 8) & 1) * 20);

    // carrying furniture
    sprite += (ITEM_S(&player_active_item)->class == ITEMCLASS_FURNITURE) * 60;

    u8 palette = 4 + (hurt_time > 0) * 1;
    u8 flip = ((dir & 1) == 0) * ((walk_dist >> 3) & 1) + (dir == 1);

    SPRITE(
        data->x - 8 - level_x_offset,                                 // x
        data->y - 11 + (on_tile == LIQUID_TILE) * 4 - level_y_offset, // y
        sprite,  // sprite
        palette, // palette
        flip,    // flip
        0,       // shape
        1,       // size
        0        // disable
    );
}

static const struct Entity player_entity = {
    .tick = player_tick,
    .draw = player_draw,

    .xr = 4,
    .yr = 3,

    .is_solid = true
};

#undef MAX_HP
#undef MAX_STAMINA

void mob_player_die(struct Level *level, struct entity_Data *data) {
    // TODO player death
}
