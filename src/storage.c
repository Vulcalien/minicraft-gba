/* Copyright 2022-2025 Vulcalien
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
         Storage Layout

    +----------------------+ 0 0000
    |        Header  .5 KB |
    |----------------------| 0 0200
    |                      |
    |        Chests        |
    |                12 KB |
    |----------------------| 0 3200
    |                      |
    |     Entity Data      |
    |              17.5 KB |
    |----------------------| 0 7800
    |                      |
    |                      |
    |      Tile Types      |
    |                      |
    |                      |
    | -  -  -  -  -  -  -  | <-- this boundary is not at a fixed address
    |                      |
    |                      |
    |      Tile Data       |
    |                      |
    |                98 KB |
    +----------------------+ 2 0000

* Header:
      4 B - game code (ZMCE)
      4 B - checksum
      4 B - random seed

      4 B - score
      4 B - gametime

    384 B - inventory
      3 B - active item
      1 B - stamina
      1 B - stamina recharge delay
      2 B - invulnerable time

      1 B - current level
      1 B - chest count

      1 B - air wizard attack delay
      1 B - air wizard attack time

      1 B - keep inventory option

     96 B - padding
*/

#define LEVEL_COUNT (sizeof(levels) / sizeof(struct Level))
#define BYTES_PER_ITEM 3

THUMB
bool storage_check(void) {
    backup_set_bank(0);

    // check if game code (ZMCE) is present
    return backup_read_byte(0) == 'Z' &&
           backup_read_byte(1) == 'M' &&
           backup_read_byte(2) == 'C' &&
           backup_read_byte(3) == 'E';
}

THUMB
bool storage_verify_checksum(void) {
    u32 val = 0;
    backup_set_bank(0);

    u32 checksum_in_file;
    backup_read(0x00004, &checksum_in_file, 4);

    for(u32 i = 0x00008; i < 0x10000; i++)
        val += backup_read_byte(i);

    backup_set_bank(1);
    for(u32 i = 0x00000; i < 0x10000; i++)
        val += backup_read_byte(i);

    return (val == checksum_in_file);
}

THUMB
void storage_srand(void) {
    backup_set_bank(0);

    u32 seed;
    backup_read(0x00008, &seed, 4);
    random_seed(seed);
}

THUMB
void storage_load_options(void) {
    backup_set_bank(0);
    options.keep_inventory = backup_read_byte(0x001a0);
}

/* ================================================================== */
/*                            storage_load                            */
/* ================================================================== */

static INLINE void load_item(u32 offset, struct item_Data *data) {
    data->type = backup_read_byte(offset);
    backup_read(offset + 1, &data->count, 2);
}

THUMB
static NO_INLINE void load_inventory(u32 offset, struct Inventory *inventory) {
    inventory->size = 0;

    for(u32 i = 0; i < INVENTORY_SIZE; i++) {
        struct item_Data *item = &inventory->items[i];
        load_item(offset, item);
        if(item->type >= ITEM_TYPES)
            break;

        inventory->size++;
        offset += BYTES_PER_ITEM;
    }
}

static INLINE void load_header(void) {
    u32 offset = 0x0000c; // skip game code, checksum and seed

    backup_read(offset, &score, 4);
    offset += 4;

    backup_read(offset, &gametime, 4);
    offset += 4;

    load_inventory(offset, &player_inventory);
    offset += INVENTORY_SIZE * BYTES_PER_ITEM;

    load_item(offset, &player_active_item);
    offset += BYTES_PER_ITEM;

    backup_read(offset, &player_stamina, 1);
    offset += 1;

    backup_read(offset, &player_stamina_recharge_delay, 1);
    offset += 1;

    backup_read(offset, &player_invulnerable_time, 2);
    offset += 2;

    backup_read(offset, &current_level, 1);
    offset += 1;

    backup_read(offset, &chest_count, 1);
    offset += 1;

    backup_read(offset, &air_wizard_attack_delay, 1);
    offset += 1;

    backup_read(offset, &air_wizard_attack_time, 1);
    offset += 1;

    // stop here: do not load options again
}

static INLINE void load_chests(void) {
    u32 offset = 0x00200;
    for(u32 i = 0; i < CHEST_LIMIT; i++) {
        load_inventory(offset, &chest_inventories[i]);
        offset += INVENTORY_SIZE * BYTES_PER_ITEM;
    }
}

static INLINE void load_entities(void) {
    u32 offset = 0x03200;
    for(u32 i = 0; i < LEVEL_COUNT; i++) {
        struct Level *level = &levels[i];

        backup_read(offset, level->entities, sizeof(level->entities));
        offset += sizeof(level->entities);
    }
}

static INLINE u32 read_RLE_tuple(u32 offset, i32 *val, i32 *run_length) {
    *val = backup_read_byte(offset++);
    *run_length = 1 + backup_read_byte(offset++);

    // if offset is 64KB, switch to memory bank 1
    if(offset == 0x10000)
        backup_set_bank(1);

    return offset;
}

THUMB
static NO_INLINE u32 read_array_RLE(u8 *array, u32 size, u32 offset) {
    // if out of memory, do nothing
    if(offset >= 0x20000)
        return offset;

    u32 index = 0;
    while(index < size) {
        i32 val, run_length;
        offset = read_RLE_tuple(offset, &val, &run_length);

        while(run_length > 0 && index < size) {
            array[index++] = val;
            run_length--;
        }

        // if out of memory, stop reading tuples
        if(offset >= 0x20000)
            return offset;
    }
    return offset;
}

static INLINE void load_tiles(void) {
    u32 offset = 0x07800;

    // load tile types
    for(u32 i = 0; i < LEVEL_COUNT; i++) {
        struct Level *level = &levels[i];
        offset = read_array_RLE(level->tiles, LEVEL_SIZE, offset);
    }

    // load tile data
    for(u32 i = 0; i < LEVEL_COUNT; i++) {
        struct Level *level = &levels[i];
        offset = read_array_RLE(level->data, LEVEL_SIZE, offset);
    }
}

THUMB
void storage_load(void) {
    backup_set_bank(0);

    load_header();
    load_chests();
    load_entities();
    load_tiles();
}

/* ================================================================== */
/*                            storage_save                            */
/* ================================================================== */

static u32 checksum;

static INLINE void write_8(u32 offset, u8 val) {
    checksum += val;
    backup_write_byte(offset, val);
}

static INLINE void write_16(u32 offset, u16 val) {
    checksum += val & 0xff;
    checksum += (val >> 8) & 0xff;
    backup_write(offset, &val, 2);
}

static INLINE void write_32(u32 offset, u32 val) {
    checksum += val & 0xff;
    checksum += (val >> 8)  & 0xff;
    checksum += (val >> 16) & 0xff;
    checksum += (val >> 24) & 0xff;
    backup_write(offset, &val, 4);
}

static INLINE void store_item(u32 offset, struct item_Data *data) {
    write_8(offset, data->type);
    write_16(offset + 1, data->count);
}

THUMB
static NO_INLINE void store_inventory(u32 offset, struct Inventory *inventory) {
    for(u32 i = 0; i < INVENTORY_SIZE; i++) {
        if(i < inventory->size) {
            store_item(offset, &inventory->items[i]);
        } else {
            write_8(offset, -1);
            write_16(offset + 1, 0);
        }
        offset += BYTES_PER_ITEM;
    }
}

static INLINE void store_header(void) {
    u32 offset = 0x00000;

    // write game code (without updating checksum)
    backup_write(offset, "ZMCE", 4);
    offset += 4;

    // skip checksum
    offset += 4;

    u32 seed = random(RANDOM_MAX + 1) | random(RANDOM_MAX + 1) << 16;
    write_32(offset, seed);
    offset += 4;

    write_32(offset, score);
    offset += 4;

    write_32(offset, gametime);
    offset += 4;

    store_inventory(offset, &player_inventory);
    offset += INVENTORY_SIZE * BYTES_PER_ITEM;

    store_item(offset, &player_active_item);
    offset += BYTES_PER_ITEM;

    write_8(offset, player_stamina);
    offset += 1;

    write_8(offset, player_stamina_recharge_delay);
    offset += 1;

    write_16(offset, player_invulnerable_time);
    offset += 2;

    write_8(offset, current_level);
    offset += 1;

    write_8(offset, chest_count);
    offset += 1;

    write_8(offset, air_wizard_attack_delay);
    offset += 1;

    write_8(offset, air_wizard_attack_time);
    offset += 1;

    write_8(offset, options.keep_inventory);
    offset += 1;

    // adjust checksum to consider padding
    checksum += 0xff * (0x00200 - offset);
}

static INLINE void store_chests(void) {
    u32 offset = 0x00200;
    for(u32 i = 0; i < CHEST_LIMIT; i++) {
        store_inventory(offset, &chest_inventories[i]);
        offset += INVENTORY_SIZE * BYTES_PER_ITEM;
    }
}

static INLINE void store_entities(void) {
    u32 offset = 0x03200;
    for(u32 i = 0; i < LEVEL_COUNT; i++) {
        struct Level *level = &levels[i];
        u8 *data = (u8 *) level->entities;

        // add all bytes to checksum
        for(u32 b = 0; b < sizeof(level->entities); b++)
            checksum += data[b];

        backup_write(offset, data, sizeof(level->entities));
        offset += sizeof(level->entities);
    }

    // adjust checksum to consider padding
    checksum += 0xff * (0x07800 - offset);
}

static INLINE u32 write_RLE_tuple(u32 offset, i32 val, i32 run_length) {
    const u16 tuple = val | (run_length - 1) << 8;
    write_16(offset, tuple);
    offset += 2;

    // if offset is 64KB, switch to memory bank 1
    if(offset == 0x10000)
        backup_set_bank(1);

    return offset;
}

THUMB
static NO_INLINE u32 write_array_RLE(u8 *array, u32 size, u32 offset) {
    // if out of memory, do nothing
    if(offset >= 0x20000)
        return offset;

    i32 run_length = 1;
    for(u32 i = 1; i < size; i++) {
        const i32 previous = array[i - 1];
        const i32 current  = array[i];

        // if value changed, or max run-length reached, write a tuple
        if(current != previous || run_length > 255) {
            offset = write_RLE_tuple(offset, previous, run_length);

            // if out of memory, stop writing tuples
            if(offset >= 0x20000)
                return offset;

            run_length = 1;
        } else {
            run_length++;
        }
    }

    // write the last tuple and return (offset + bytes-used)
    return write_RLE_tuple(offset, array[size], run_length);
}

static INLINE void store_tiles(void) {
    u32 offset = 0x07800;

    // store tile types
    for(u32 i = 0; i < LEVEL_COUNT; i++) {
        struct Level *level = &levels[i];
        offset = write_array_RLE(level->tiles, LEVEL_SIZE, offset);
    }

    // store tile data
    for(u32 i = 0; i < LEVEL_COUNT; i++) {
        struct Level *level = &levels[i];
        offset = write_array_RLE(level->data, LEVEL_SIZE, offset);
    }

    // adjust checksum to consider padding
    checksum += 0xff * (0x20000 - offset);
}

THUMB
void storage_save(void) {
    backup_set_bank(0);
    backup_erase_chip();
    checksum = 0;

    store_header();
    store_chests();
    store_entities();
    store_tiles();

    backup_set_bank(0);
    backup_write(0x00004, &checksum, 4);
}
