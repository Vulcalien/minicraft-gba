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
#include "sound.h"

#define SOUND_CONTROL_H *((vu16 *) 0x04000082)
#define SOUND_CONTROL_X *((vu16 *) 0x04000084)

void sound_init(void) {
    SOUND_CONTROL_X = (1 << 7); // Enable Sound

    SOUND_CONTROL_H = (1 << 2) | // Channel A Volume (1 is 100%)
                      (1 << 8) | // Enable Channel A RIGHT
                      (1 << 9) | // Enable Channel A LEFT
                      (0 << 10); // Channel A Timer (0 is Timer 0)
}
