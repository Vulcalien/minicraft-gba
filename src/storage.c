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
#include "storage.h"

#include "level.h"
#include "tile.h"
#include "furniture.h"
#include "inventory.h"
#include "item.h"
#include "player.h"
#include "air-wizard.h"

#define NO_INLINE __attribute__((noinline))

#define FLASH_ROM ((vu8 *) 0x0e000000)

/*
Storage Layout (128 KB)

1 KB - header:
    4 B - game code (ZMCE)

114 KB - 5 * level:
     7056 B - tiles
    12544 B - data
     3570 B - entities

12 KB - chest inventories
1 KB - player and other data:
    384 B - inventory
      3 B - active item

      1 B - stamina
      1 B - stamina recharge delay
      2 B - invulnerable time

      4 B - score
      4 B - gametime

      1 B - current level
      1 B - chest count

      1 B - air wizard attack delay
      1 B - air wizard attack time
 */

#define LEVEL_COUNT (sizeof(levels) / sizeof(struct Level))

static_assert(LEVEL_COUNT == 5, "there should be 5 levels");

static_assert(
    (LEVEL_W * LEVEL_H) % 16 == 0,
    "level size should be a multiple of 16"
);

static_assert(
    TILE_TYPES * TILE_TYPES < (1 << 9), "two tiles should fit in 9 bits"
);

// assert level
static_assert(
    LEVEL_W * LEVEL_H * 9 / 16 == 7056,
    "tile storage size per level should be 7056 B"
);

static_assert(
    LEVEL_W * LEVEL_H == 12544,
    "data storage size per level should be 12544 B"
);

static_assert(
    sizeof(struct entity_Data) * ENTITY_LIMIT == 3570,
    "entity storage size per level should be 3570 B"
);

// assert chests
static_assert(
    CHEST_LIMIT * INVENTORY_SIZE * 3 == 12 * 1024,
    "chest storage size should be 12 KB"
);

static inline void switch_bank(u32 bank) {
    FLASH_ROM[0x5555] = 0xaa;
    FLASH_ROM[0x2aaa] = 0x55;
    FLASH_ROM[0x5555] = 0xb0;

    FLASH_ROM[0x0000] = bank;
}

IWRAM_SECTION NO_INLINE
static u8 read_byte(u16 addr) {
    // TODO make sure only LDRB is used
    return FLASH_ROM[addr];
}

bool storage_check(void) {
    switch_bank(0);

    // check if game code (ZMCE) is present
    bool valid = read_byte(0) == 'Z' &&
                 read_byte(1) == 'M' &&
                 read_byte(2) == 'C' &&
                 read_byte(3) == 'E';

    // TODO validate a checksum

    return valid;
}

static inline void load_item(u16 *addr, struct item_Data *data) {
    data->type = FLASH_ROM[(*addr)++];

    if(item_is_resource(data->type)) {
        data->count  = read_byte((*addr)++);
        data->count |= read_byte((*addr)++) >> 8;
    } else if(ITEM_S(data)->class == ITEMCLASS_TOOL) {
        data->tool_level = read_byte((*addr)++);
        (*addr)++;
    } else if(data->type == CHEST_ITEM) {
        data->chest_id = read_byte((*addr)++);
        (*addr)++;
    } else {
        (*addr)++;
        (*addr)++;
    }
}

static inline void load_inventory(u16 *addr, struct Inventory *inventory) {
    for(u32 i = 0; i < INVENTORY_SIZE; i++) {
        struct item_Data *item = &inventory->items[i];
        load_item(addr, item);

        if(item->type >= ITEM_TYPES) {
            inventory->size = i;
            (*addr) += (INVENTORY_SIZE - i - 1) * 3;
            break;
        }
    }
}

void storage_load(void) {
    u16 addr;

    switch_bank(0);

    // read levels
    addr = 1 * 1024;
    for(u32 l = 0; l < LEVEL_COUNT; l++) {
        struct Level *level = &levels[l];

        // read tiles
        for(u32 t = 0; t < LEVEL_W * LEVEL_H; t += 16) {
            u8 bytes[9];

            for(u32 i = 0; i < sizeof(bytes); i++)
                bytes[i] = read_byte(addr++);

            for(u32 i = 0; i < 8; i++) {
                u16 two_tiles = bytes[i]     << (i + 1) |
                                bytes[i + 1] >> (7 - i);

                two_tiles &= 0x1ff;

                level->tiles[t + i * 2]     = two_tiles / TILE_TYPES;
                level->tiles[t + i * 2 + 1] = two_tiles % TILE_TYPES;
            }
        }

        // read data
        for(u32 t = 0; t < LEVEL_W * LEVEL_H; t++) {
            level->data[t] = read_byte(addr++);

            // 64 KB is hit when writing tile data for the third level
            // causing an overflow: switch bank
            if(addr == 0)
                switch_bank(1);
        }

        // read entities
        for(u32 i = 0; i < ENTITY_LIMIT; i++) {
            u8 *data = (u8 *) &level->entities[i];

            for(u32 b = 0; b < sizeof(struct entity_Data); b++)
                data[b] = read_byte(addr++);
        }
    }

    // read chest inventories
    addr = (115 % 64) * 1024;
    for(u32 i = 0; i < CHEST_LIMIT; i++)
        load_inventory(&addr, &chest_inventories[i]);

    // read player and other data
    addr = (127 % 64) * 1024;
    {
        load_inventory(&addr, &player_inventory);
        load_item(&addr, &player_active_item);

        player_stamina                = read_byte(addr++);
        player_stamina_recharge_delay = read_byte(addr++);

        player_invulnerable_time  = read_byte(addr++);
        player_invulnerable_time |= read_byte(addr++) << 8;

        score =  read_byte(addr++);
        score |= read_byte(addr++) << 8;
        score |= read_byte(addr++) << 16;
        score |= read_byte(addr++) << 24;

        gametime =  read_byte(addr++);
        gametime |= read_byte(addr++) << 8;
        gametime |= read_byte(addr++) << 16;
        gametime |= read_byte(addr++) << 24;

        current_level = read_byte(addr++);
        chest_count = read_byte(addr++);

        air_wizard_attack_delay = read_byte(addr++);
        air_wizard_attack_time = read_byte(addr++);
    }
}

static inline void erase_chip(void) {
    FLASH_ROM[0x5555] = 0xaa;
    FLASH_ROM[0x2aaa] = 0x55;
    FLASH_ROM[0x5555] = 0x80;

    FLASH_ROM[0x5555] = 0xaa;
    FLASH_ROM[0x2aaa] = 0x55;
    FLASH_ROM[0x5555] = 0x10;

    // TODO setup a timeout
    while(read_byte(0x0000) != 0xff);
}

IWRAM_SECTION NO_INLINE
static void write_byte(u16 addr, u8 byte) {
    FLASH_ROM[0x5555] = 0xaa;
    FLASH_ROM[0x2aaa] = 0x55;
    FLASH_ROM[0x5555] = 0xa0;

    // TODO make sure only STRB is used
    FLASH_ROM[addr] = byte;

    // TODO setup a timeout
    while(read_byte(addr) != byte);
}

static inline void store_item(u16 *addr, struct item_Data *data) {
    write_byte((*addr)++, data->type);

    if(item_is_resource(data->type)) {
        write_byte((*addr)++, data->count);
        write_byte((*addr)++, data->count >> 8);
    } else if(ITEM_S(data)->class == ITEMCLASS_TOOL) {
        write_byte((*addr)++, data->tool_level);
        (*addr)++;
    } else if(data->type == CHEST_ITEM) {
        write_byte((*addr)++, data->chest_id);
        (*addr)++;
    } else {
        write_byte((*addr)++, data->chest_id);
        (*addr)++;
    }
}

static inline void store_inventory(u16 *addr, struct Inventory *inventory) {
    for(u32 i = 0; i < INVENTORY_SIZE; i++) {
        if(i >= inventory->size) {
            (*addr) += (INVENTORY_SIZE - i) * 3;
            break;
        }

        store_item(addr, &inventory->items[i]);
    }
}

void storage_save(void) {
    u16 addr;

    erase_chip();
    switch_bank(0);

    // write header
    addr = 0;
    {
        // game code - ZMCE
        write_byte(addr++, 'Z');
        write_byte(addr++, 'M');
        write_byte(addr++, 'C');
        write_byte(addr++, 'E');
    }

    // write levels
    addr = 1 * 1024;
    for(u32 l = 0; l < LEVEL_COUNT; l++) {
        struct Level *level = &levels[l];

        // write tiles
        for(u32 t = 0; t < LEVEL_W * LEVEL_H; t += 16) {
            u8 bytes[9] = { 0 };

            for(u32 i = 0; i < 8; i++) {
                u16 two_tiles = level->tiles[t + i * 2] * TILE_TYPES +
                                level->tiles[t + i * 2 + 1];

                bytes[i]     |= two_tiles >> (i + 1);
                bytes[i + 1] |= two_tiles << (7 - i);
            }

            for(u32 i = 0; i < sizeof(bytes); i++)
                write_byte(addr++, bytes[i]);
        }

        // write data
        for(u32 t = 0; t < LEVEL_W * LEVEL_H; t++) {
            write_byte(addr++, level->data[t]);

            // 64 KB is hit when writing tile data for the third level
            // causing an overflow: switch bank
            if(addr == 0)
                switch_bank(1);
        }

        // write entities
        for(u32 i = 0; i < ENTITY_LIMIT; i++) {
            u8 *data = (u8 *) &level->entities[i];

            for(u32 b = 0; b < sizeof(struct entity_Data); b++)
                write_byte(addr++, data[b]);
        }
    }

    // write chest inventories
    addr = (115 % 64) * 1024;
    for(u32 i = 0; i < CHEST_LIMIT; i++)
        store_inventory(&addr, &chest_inventories[i]);

    // write player and other data
    addr = (127 % 64) * 1024;
    {
        store_inventory(&addr, &player_inventory);
        store_item(&addr, &player_active_item);

        write_byte(addr++, player_stamina);
        write_byte(addr++, player_stamina_recharge_delay);

        write_byte(addr++, player_invulnerable_time);
        write_byte(addr++, player_invulnerable_time >> 8);

        write_byte(addr++, score);
        write_byte(addr++, score >> 8);
        write_byte(addr++, score >> 16);
        write_byte(addr++, score >> 24);

        write_byte(addr++, gametime);
        write_byte(addr++, gametime >> 8);
        write_byte(addr++, gametime >> 16);
        write_byte(addr++, gametime >> 24);

        write_byte(addr++, current_level);
        write_byte(addr++, chest_count);

        write_byte(addr++, air_wizard_attack_delay);
        write_byte(addr++, air_wizard_attack_time);
    }
}
