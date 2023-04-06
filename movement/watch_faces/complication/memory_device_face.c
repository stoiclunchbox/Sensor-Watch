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

#include <stdlib.h>
#include <string.h>
#include "memory_device_face.h"



static void _memory_device_face_reset_card(memory_device_state_t *state, uint8_t _card) {
    for (uint8_t i = 0; i < SLOTS; i++) {
        state->card[_card].pos[i] = 0;
    }
    state->card[_card].slot_idx = 0;
}

static void _memory_device_face_draw(memory_device_state_t *state, uint8_t subsecond) {
    char buf[12];

    //sanity vars for our mental convenience
    bool mode = state->card[state->card_idx].mode;
    uint8_t slot = state->card[state->card_idx].slot_idx;

    if (mode) {
        if (state->card[state->card_idx].pos[slot] >= 26)
            state->card[state->card_idx].pos[slot] = 0;

        sprintf(buf, "MD%da%d%c %c%c%c",
                (state->card_idx + 1),
                (slot + 1),
                state->alphas[state->card[state->card_idx].pos[0]],
                state->alphas[state->card[state->card_idx].pos[1]],
                state->alphas[state->card[state->card_idx].pos[2]],
                state->alphas[state->card[state->card_idx].pos[3]]
                );
    } else {
        if (state->card[state->card_idx].pos[slot] >= 10)
            state->card[state->card_idx].pos[slot] = 0;

        sprintf(buf, "MD%dn%c%c%c%c%2d",
                (state->card_idx + 1),
                state->nums[state->card[state->card_idx].pos[0]],
                state->nums[state->card[state->card_idx].pos[1]],
                state->nums[state->card[state->card_idx].pos[2]],
                state->nums[state->card[state->card_idx].pos[3]],
                (slot + 1)
                );
    }


    // WIP temporarily deactivated blinking while not fully implemented
    // blink to indicate selected position
    /* if (subsecond % 2) { */
    /*     uint8_t blink_idx = (selected + 4); */
    /*     buf[blink_idx] = '-'; */
    /* } */

    watch_display_string(buf, 0);
}

void memory_device_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr) {
    (void) settings;
    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(memory_device_state_t));
        memset(*context_ptr, 0, sizeof(memory_device_state_t));
        memory_device_state_t *state = (memory_device_state_t *)*context_ptr;

        // initialise default values
        state->alphas = "abcdefGhijkLmnopqrstuwxHyz";
        state->nums = "0123456789";

        for (uint8_t i = 0; i < CARDS; i++) {
            _memory_device_face_reset_card(state, i);
            state->card[i].mode = START_MODE;
        }
        state->card_idx = 0;
    }
}

void memory_device_face_activate(movement_settings_t *settings, void *context) {
    (void) settings;
    (void) context;
}

bool memory_device_face_loop(movement_event_t event, movement_settings_t *settings, void *context) {
    memory_device_state_t *state = (memory_device_state_t *)context;

    // TODO implement blinking in a reliable and non-blocking way
    /* movement_request_tick_frequency(4); */

    switch (event.event_type) {
        /* case EVENT_TICK:              // for animation */
            // Fall through
        case EVENT_ACTIVATE:
            _memory_device_face_draw(state, event.subsecond);
            break;
        case EVENT_LIGHT_BUTTON_DOWN: // don't illuminate led immediately
            break;
        case EVENT_LIGHT_LONG_PRESS:  // led
            movement_illuminate_led();
            break;
        case EVENT_LIGHT_LONG_UP:     // toggle mode
            state->card[state->card_idx].mode = (!state->card[state->card_idx].mode);
            _memory_device_face_reset_card(state, state->card_idx);  // reset card when changing mode
            _memory_device_face_draw(state, event.subsecond);
            break;
        case EVENT_LIGHT_BUTTON_UP:   // cycle cards
            state->card_idx++;
            if (state->card_idx >= CARDS) state->card_idx = 0;
            _memory_device_face_draw(state, event.subsecond);
            break;
        case EVENT_ALARM_LONG_PRESS:  // cycle position
            state->card[state->card_idx].slot_idx++;
            if (state->card[state->card_idx].slot_idx >= SLOTS) state->card[state->card_idx].slot_idx = 0;
            _memory_device_face_draw(state, event.subsecond);
            break;
        case EVENT_ALARM_BUTTON_UP:   // cycle chars
            state->card[state->card_idx].pos[state->card[state->card_idx].slot_idx]++;
            _memory_device_face_draw(state, event.subsecond);
            break;
        case EVENT_TIMEOUT:
            movement_move_to_face(0);
            break;
        case EVENT_LOW_ENERGY_UPDATE:
            // not used (at the moment) because we timeout
            break;
        default:
            // Movement's default loop handler will step in for any cases you don't handle above:
            // * EVENT_LIGHT_BUTTON_DOWN lights the LED
            // * EVENT_MODE_BUTTON_UP moves to the next watch face in the list
            // * EVENT_MODE_LONG_PRESS returns to the first watch face (or skips to the secondary watch face, if configured)
            // You can override any of these behaviors by adding a case for these events to this switch statement.
            return movement_default_loop_handler(event, settings);
    }

    // return true if the watch can enter standby mode. Generally speaking, you should always return true.
    // Exceptions:
    //  * If you are displaying a color using the low-level watch_set_led_color function, you should return false.
    //  * If you are sounding the buzzer using the low-level watch_set_buzzer_on function, you should return false.
    // Note that if you are driving the LED or buzzer using Movement functions like movement_illuminate_led or
    // movement_play_alarm, you can still return true. This guidance only applies to the low-level watch_ functions.
    return true;
}

void memory_device_face_resign(movement_settings_t *settings, void *context) {
    (void) settings;
    (void) context;

    // handle any cleanup before your watch face goes off-screen.
}

