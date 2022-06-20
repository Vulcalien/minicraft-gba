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
#include "input.h"

#define KEY_INPUT *((vu16 *) 0x04000130)

u16 input_keys_down    = 0;
u16 input_keys_clicked = 0;

void input_tick(void) {
    input_keys_down = KEY_INPUT;

    // Set clicked
    //
    // Repeat delay:    500ms (30 ticks)
    // Repeat interval: ~33ms (2 ticks)
    for(u32 i = 0; i < 10; i++) {
        static u8 press_time[10] = { 0 };

        u16 key_state = input_keys_down & (1 << i);
        if(!key_state) {
            if(press_time[i] < 2 || press_time[i] >= 30) {
                // invert i-th bit
                input_keys_clicked ^= (1 << i);
            }

            if(press_time[i] < 30)
                press_time[i]++;
        } else {
            press_time[i] = 0;

            // set i-th bit to 1
            /*input_keys_clicked &= ~(1 << i);*/
            input_keys_clicked |= (1 << i);
        }
    }
}
