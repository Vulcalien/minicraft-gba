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
#ifndef MINICRAFT_SOUND
#define MINICRAFT_SOUND

#include "minicraft.h"

#include <gba/sound.h>

extern bool sound_channel_flag;

#define SOUND_PLAY(sound) sound_play(sound, sizeof(sound))
INLINE void sound_play(const u8 *sound, u16 length) {
    sound_dma_play(
        sound, length, false,
        sound_channel_flag ? SOUND_DMA_A  : SOUND_DMA_B
    );
    sound_channel_flag = !sound_channel_flag;
}

// Sound effects
extern const u8 sound_start[1804];

extern const u8 sound_pickup[512];
extern const u8 sound_craft[1764];

extern const u8 sound_monster_hurt[876];
extern const u8 sound_player_hurt[888];

extern const u8 sound_player_death[13608];
extern const u8 sound_boss_death[17292];

#endif // MINICRAFT_SOUND
