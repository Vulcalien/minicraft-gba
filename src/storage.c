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
#include "storage.h"

#include "options.h"
#include "level.h"
#include "tile.h"
#include "furniture.h"
#include "inventory.h"
#include "item.h"
#include "player.h"
#include "air-wizard.h"

/*
Storage Layout (128 KB)

* 1 KB - header and options:
      4 B - game code (ZMCE)
      4 B - random seed

      1 B - keep inventory

   1015 B - padding

* 114 KB - level data:
    5 times:
       7056 B - tiles
      12544 B - data
       3570 B - entities
    886 B - padding

* 12 KB - chest inventories:
    32 times:
        384 B - chest inventory

* 1 KB - player and other data:
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

    617 B - padding

      4 B - checksum
*/

#define FLASH_ROM ((vu8 *) 0x0e000000)

#define LEVEL_COUNT (sizeof(levels) / sizeof(struct Level))
#define BYTES_PER_ITEM (3)

static u32 checksum;

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
    CHEST_LIMIT * INVENTORY_SIZE * BYTES_PER_ITEM == 12 * 1024,
    "chest storage size should be 12 KB"
);

static inline void switch_bank(u32 bank) {
    FLASH_ROM[0x5555] = 0xaa;
    FLASH_ROM[0x2aaa] = 0x55;
    FLASH_ROM[0x5555] = 0xb0;

    FLASH_ROM[0x0000] = bank;
}

IWRAM_SECTION NOCLONE NOINLINE
static u8 read_byte(u16 addr) {
    return FLASH_ROM[addr];
}

static inline u16 read_2_bytes(u16 addr) {
    return read_byte(addr) |
           read_byte(addr + 1) << 8;
}

static inline u32 read_4_bytes(u16 addr) {
    return read_byte(addr) |
           read_byte(addr + 1) << 8 |
           read_byte(addr + 2) << 16 |
           read_byte(addr + 3) << 24;
}

bool storage_check(void) {
    switch_bank(0);

    // check if game code (ZMCE) is present
    bool valid = read_byte(0) == 'Z' &&
                 read_byte(1) == 'M' &&
                 read_byte(2) == 'C' &&
                 read_byte(3) == 'E';

    return valid;
}

bool storage_verify_checksum(void) {
    u32 val = 0;

    switch_bank(0);
    for(u32 i = 0; i < 64 * 1024; i++)
        val += read_byte(i);

    switch_bank(1);
    for(u32 i = 0; i < 64 * 1024 - 4; i++)
        val += read_byte(i);

    u32 checksum_in_file = read_4_bytes(64 * 1024 - 4);
    return (val == checksum_in_file);
}

void storage_srand(void) {
    switch_bank(0);

    srand(read_4_bytes(4), true);
}

void storage_load_options(void) {
    options.keep_inventory = read_byte(8);
}

static inline void load_item(u16 addr, struct item_Data *data) {
    data->type = read_byte(addr);
    if(data->type >= ITEM_TYPES)
        return;

    u16 val = read_2_bytes(addr + 1);
    if(item_is_resource(data->type))
        data->count = val;
    else if(ITEM_S(data)->class == ITEMCLASS_TOOL)
        data->tool_level = val;
    else if(data->type == CHEST_ITEM)
        data->chest_id = val;
}

static inline void load_inventory(u16 addr, struct Inventory *inventory) {
    inventory->size = 0;

    for(u32 i = 0; i < INVENTORY_SIZE; i++) {
        struct item_Data *item = &inventory->items[i];
        load_item(addr, item);
        if(item->type >= ITEM_TYPES)
            break;

        addr += BYTES_PER_ITEM;
        inventory->size++;
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
    for(u32 i = 0; i < CHEST_LIMIT; i++) {
        load_inventory(addr, &chest_inventories[i]);
        addr += INVENTORY_SIZE * BYTES_PER_ITEM;
    }

    // read player and other data
    addr = (127 % 64) * 1024;
    {
        load_inventory(addr, &player_inventory);
        addr += INVENTORY_SIZE * BYTES_PER_ITEM;

        load_item(addr, &player_active_item);
        addr += BYTES_PER_ITEM;

        player_stamina                = read_byte(addr++);
        player_stamina_recharge_delay = read_byte(addr++);

        player_invulnerable_time = read_2_bytes(addr);
        addr += 2;

        score = read_4_bytes(addr);
        addr += 4;

        gametime = read_4_bytes(addr);
        addr += 4;

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

    while(read_byte(0x0000) != 0xff);
}

IWRAM_SECTION NOCLONE NOINLINE
static void write_byte(u16 addr, u8 byte) {
    checksum += byte;

    FLASH_ROM[0x5555] = 0xaa;
    FLASH_ROM[0x2aaa] = 0x55;
    FLASH_ROM[0x5555] = 0xa0;

    FLASH_ROM[addr] = byte;

    while(read_byte(addr) != byte);
}

static inline void write_2_bytes(u16 addr, u16 bytes) {
    write_byte(addr, bytes);
    write_byte(addr + 1, bytes >> 8);
}

static inline void write_4_bytes(u16 addr, u32 bytes) {
    write_byte(addr, bytes);
    write_byte(addr + 1, bytes >> 8);
    write_byte(addr + 2, bytes >> 16);
    write_byte(addr + 3, bytes >> 24);
}

static inline void write_padding(u16 *addr, u16 dest_address) {
    while(*addr != dest_address)
        write_byte((*addr)++, 0);
}

static inline void store_item(u16 addr, struct item_Data *data) {
    write_byte(addr, data->type);

    u16 val = 0;
    if(data->type < ITEM_TYPES) {
        if(item_is_resource(data->type))
            val = data->count;
        else if(ITEM_S(data)->class == ITEMCLASS_TOOL)
            val = data->tool_level;
        else if(data->type == CHEST_ITEM)
            val = data->chest_id;
    }

    write_2_bytes(addr + 1, val);

}

static inline void store_inventory(u16 addr, struct Inventory *inventory) {
    for(u32 i = 0; i < INVENTORY_SIZE; i++) {
        if(i < inventory->size) {
            store_item(addr, &inventory->items[i]);
        } else {
            write_byte(addr, -1);
            write_2_bytes(addr + 1, 0);
        }
        addr += BYTES_PER_ITEM;
    }
}

void storage_save(void) {
    checksum = 0;
    erase_chip();

    u16 addr = 0;
    switch_bank(0);

    // write header and options
    {
        // game code - ZMCE
        write_byte(addr++, 'Z');
        write_byte(addr++, 'M');
        write_byte(addr++, 'C');
        write_byte(addr++, 'E');

        // random seed
        u32 seed = rand() << 16 | rand();
        write_4_bytes(addr, seed);
        addr += 4;

        // options
        write_byte(addr++, options.keep_inventory);

        write_padding(&addr, 1 * 1024);
    }

    // write levels
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

    write_padding(&addr, (115 % 64) * 1024);

    // write chest inventories
    for(u32 i = 0; i < CHEST_LIMIT; i++) {
        store_inventory(addr, &chest_inventories[i]);
        addr += INVENTORY_SIZE * BYTES_PER_ITEM;
    }

    // write player and other data
    {
        store_inventory(addr, &player_inventory);
        addr += INVENTORY_SIZE * BYTES_PER_ITEM;

        store_item(addr, &player_active_item);
        addr += BYTES_PER_ITEM;

        write_byte(addr++, player_stamina);
        write_byte(addr++, player_stamina_recharge_delay);

        write_2_bytes(addr, player_invulnerable_time);
        addr += 2;

        write_4_bytes(addr, score);
        addr += 4;

        write_4_bytes(addr, gametime);
        addr += 4;

        write_byte(addr++, current_level);
        write_byte(addr++, chest_count);

        write_byte(addr++, air_wizard_attack_delay);
        write_byte(addr++, air_wizard_attack_time);

        write_padding(&addr, (128 % 64) * 1024 - 4);

        write_4_bytes(addr, checksum);
    }
}
