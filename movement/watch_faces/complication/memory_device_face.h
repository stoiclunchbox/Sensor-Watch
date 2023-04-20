/*
 * MIT License
 *
 * Copyright (c) <#year#> <#author_name#>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef memory_device_FACE_H_
#define memory_device_FACE_H_

#include "movement.h"

/*
 * A DESCRIPTION OF YOUR WATCH FACE
 *
 * TODO a description of how use it
 *
 */

#define TICK_FREQ   8    // blink speed
#define CARDS       9    // no of available pages (max. 16)

typedef struct {
    uint8_t pos[6];
    uint8_t slot_idx : 3;
    // REVIEW bitfield efficiency
    bool modified;
} card_data_t;

typedef struct {
    card_data_t card[CARDS];
    uint8_t card_idx : 4;
    bool quick_cycle;
    bool edit_mode;
    const char *alphanums;
    const char *nums;
} memory_device_state_t;

void memory_device_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr);
void memory_device_face_activate(movement_settings_t *settings, void *context);
bool memory_device_face_loop(movement_event_t event, movement_settings_t *settings, void *context);
void memory_device_face_resign(movement_settings_t *settings, void *context);

#define memory_device_face ((const watch_face_t){ \
    memory_device_face_setup, \
    memory_device_face_activate, \
    memory_device_face_loop, \
    memory_device_face_resign, \
    NULL, \
})

#endif // memory_device_FACE_H_

