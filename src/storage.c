/* Copyright 2022-2026 Vulcalien
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
#include "entity.h"
#include "furniture.h"
#include "inventory.h"
#include "item.h"
#include "player.h"
#include "air-wizard.h"

/*
         Storage Layout

    +----------------------+ 0000
    |        Header        |
    |----------------------| 0040
    |                      |
    |  Player's inventory  |
    |                      |
    | -  -  -  -  -  -  -  |
    |                      |
    |        Chests        |
    |                      |
    | -  -  -  -  -  -  -  |
    |                      |
    |     Entity Data      |
    |                      |
    | -  -  -  -  -  -  -  |
    |                      |
    |      Tile Types      |
    |                      |
    | -  -  -  -  -  -  -  |
    |                      |
    |      Tile Data       |
    |                      |
    +----------------------+ 1 0000

* Header:
      4 B - game code (ZMCE)
      4 B - checksum
      4 B - random seed

      4 B - score
      4 B - gametime

      1 B - stamina
      1 B - stamina recharge delay
      2 B - invulnerable time

      1 B - current level
      1 B - chest count

      1 B - air wizard attack delay
      1 B - air wizard attack time

      4 B - padding

      1 B - keep inventory option

     31 B - padding
*/

#define STORAGE_SIZE (64 * 1024)

#define HEADER_SIZE 64

#define LEVEL_COUNT (sizeof(levels) / sizeof(struct Level))
#define BYTES_PER_ITEM 3
#define BYTES_PER_ENTITY (sizeof(struct entity_Data))

THUMB
bool storage_check(void) {
    backup_identify();

    // check if game code (ZMCE) is present
    u8 code[4];
    backup_read(0x0000, &code, 4);
    return code[0] == 'Z' &&
           code[1] == 'M' &&
           code[2] == 'C' &&
           code[3] == 'E';
}

THUMB
bool storage_verify_checksum(void) {
    u32 val = 0;

    u32 checksum_in_file;
    backup_read(0x0004, &checksum_in_file, 4);

    u8 buffer[8];
    for(u32 i = 0x0008; i < STORAGE_SIZE; i += sizeof(buffer)) {
        backup_read(i, buffer, sizeof(buffer));
        for(u32 b = 0; b < sizeof(buffer); b++)
            val += buffer[b];
    }

    return (val == checksum_in_file);
}

THUMB
void storage_srand(void) {
    u32 seed;
    backup_read(0x0008, &seed, 4);
    random_seed(seed);
}

THUMB
void storage_load_options(void) {
    backup_read(0x0020, &options, sizeof(struct Options));
}

/* ================================================================== */
/*                            storage_load                            */
/* ================================================================== */

static INLINE u32 load_item(u32 offset, struct item_Data *data) {
    backup_read(offset, &data->type, 1);
    backup_read(offset + 1, &data->count, 2);
    return offset + BYTES_PER_ITEM;
}

THUMB
static NO_INLINE u32 load_inventory(u32 offset, struct Inventory *inventory) {
    backup_read(offset++, &inventory->size, 1);
    for(u32 i = 0; i < inventory->size; i++) {
        struct item_Data *item = &inventory->items[i];
        offset = load_item(offset, item);
    }
    return offset;
}

static INLINE u32 load_header(u32 offset) {
    offset += 12; // skip game code, checksum and seed

    backup_read(offset, &score, 4);
    offset += 4;

    backup_read(offset, &gametime, 4);
    offset += 4;

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
    return HEADER_SIZE;
}

static INLINE u32 load_chests(u32 offset) {
    for(u32 i = 0; i < CHEST_LIMIT; i++) {
        struct Inventory *inventory = &chest_inventories[i];
        offset = load_inventory(offset, inventory);
    }
    return offset;
}

static INLINE u32 load_entity(u32 offset, struct entity_Data *data) {
    backup_read(offset, data, BYTES_PER_ENTITY);
    return offset + BYTES_PER_ENTITY;
}

static INLINE u32 load_entities(u32 offset) {
    // load player
    offset = load_entity(offset, &levels[current_level].entities[0]);

    for(u32 i = 0; i < LEVEL_COUNT; i++) {
        struct Level *level = &levels[i];

        // read entity count and load that many entities
        u8 count;
        backup_read(offset++, &count, 1);
        for(u32 e = 1; e < 1 + count; e++) {
            struct entity_Data *data = &level->entities[e];
            offset = load_entity(offset, data);
        }

        // invalidate remaining entity slots
        for(u32 e = 1 + count; e < ENTITY_LIMIT; e++)
            level->entities[e].type = -1;
    }
    return offset;
}

static INLINE u32 read_RLE_tuple(u32 offset, i32 *val, i32 *run_length) {
    u8 buffer[2];
    backup_read(offset, buffer, sizeof(buffer));

    *val = buffer[0];
    *run_length = 1 + buffer[1];
    return offset + sizeof(buffer);
}

THUMB
static NO_INLINE u32 read_array_RLE(u8 *array, u32 size, u32 offset) {
    // if out of memory, do nothing
    if(offset >= STORAGE_SIZE)
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
        if(offset >= STORAGE_SIZE)
            return offset;
    }
    return offset;
}

static INLINE u32 load_tiles(u32 offset) {
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
    return offset;
}

THUMB
void storage_load(void) {
    u32 offset = load_header(0);

    offset = load_item(offset, &player_active_item);
    offset = load_inventory(offset, &player_inventory);

    offset = load_chests(offset);
    offset = load_entities(offset);
    offset = load_tiles(offset);
}

/* ================================================================== */
/*                            storage_save                            */
/* ================================================================== */

static u32 checksum;

static INLINE void write_8(u32 offset, u8 val) {
    checksum += val;
    backup_write(offset, &val, 1);
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

static INLINE u32 padding(u32 offset, u32 dest) {
    checksum += 0xff * (dest - offset);
    return dest;
}

static INLINE u32 store_item(u32 offset, struct item_Data *data) {
    write_8(offset, data->type);
    write_16(offset + 1, data->count);
    return offset + BYTES_PER_ITEM;
}

THUMB
static NO_INLINE u32 store_inventory(u32 offset, struct Inventory *inventory) {
    write_8(offset++, inventory->size);
    for(u32 i = 0; i < inventory->size; i++) {
        struct item_Data *item = &inventory->items[i];
        offset = store_item(offset, item);
    }
    return offset;
}

static INLINE u32 store_header(u32 offset) {
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

    offset = padding(offset, 0x0020);
    write_8(offset, options.keep_inventory);
    offset += 1;

    return padding(offset, HEADER_SIZE);
}

static INLINE u32 store_chests(u32 offset) {
    for(u32 i = 0; i < CHEST_LIMIT; i++) {
        struct Inventory *inventory = &chest_inventories[i];
        offset = store_inventory(offset, inventory);
    }
    return offset;
}

static INLINE u32 store_entity(u32 offset, struct entity_Data *data) {
    // add all bytes to checksum
    for(u32 i = 0; i < BYTES_PER_ENTITY; i++)
        checksum += ((u8 *) data)[i];

    backup_write(offset, data, BYTES_PER_ENTITY);
    return offset + BYTES_PER_ENTITY;
}

static INLINE u32 store_entities(u32 offset) {
    // store player
    offset = store_entity(offset, &levels[current_level].entities[0]);

    for(u32 i = 0; i < LEVEL_COUNT; i++) {
        struct Level *level = &levels[i];

        // reserve one byte for entity count
        offset++;

        u32 count = 0;
        for(u32 e = 1; e < ENTITY_LIMIT; e++) {
            struct entity_Data *data = &level->entities[e];
            if(data->type >= ENTITY_TYPES)
                continue;

            offset = store_entity(offset, data);
            count++;
        }

        // store entity count
        write_8(offset - count * BYTES_PER_ENTITY - 1, count);
    }
    return offset;
}

static INLINE u32 write_RLE_tuple(u32 offset, i32 val, i32 run_length) {
    const u16 tuple = val | (run_length - 1) << 8;
    write_16(offset, tuple);
    return offset + 2;
}

THUMB
static NO_INLINE u32 write_array_RLE(u8 *array, u32 size, u32 offset) {
    // if out of memory, do nothing
    if(offset >= STORAGE_SIZE)
        return offset;

    i32 run_length = 1;
    for(u32 i = 1; i < size; i++) {
        const i32 previous = array[i - 1];
        const i32 current  = array[i];

        // if value changed, or max run-length reached, write a tuple
        if(current != previous || run_length > 255) {
            offset = write_RLE_tuple(offset, previous, run_length);

            // if out of memory, stop writing tuples
            if(offset >= STORAGE_SIZE)
                return offset;

            run_length = 1;
        } else {
            run_length++;
        }
    }

    // write the last tuple and return (offset + bytes-used)
    return write_RLE_tuple(offset, array[size], run_length);
}

static INLINE u32 store_tiles(u32 offset) {
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
    return offset;
}

THUMB
void storage_save(void) {
    backup_erase_chip();
    checksum = 0;

    u32 offset = store_header(0);

    offset = store_item(offset, &player_active_item);
    offset = store_inventory(offset, &player_inventory);

    offset = store_chests(offset);
    offset = store_entities(offset);
    offset = store_tiles(offset);

    padding(offset, STORAGE_SIZE);
    backup_write(0x0004, &checksum, 4);
}
