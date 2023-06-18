/*
 * MIT License
 *
 * Copyright (c) 2023 <#author_name#>
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
#include "water_tracker_face.h"

static const uint8_t _cup_sizes[] = {0, 380, 500, 2000}; // default cup sizes in mL (mililitres)

typedef enum {
    water_tracker_mode_norm
    water_tracker_mode_edit
    water_tracker_mode_data
} water_tracker_mode_t

static void _water_tracker_face_draw(memory_device_state_t *state, uint8_t subsecond) {
    char buf[17];

    // WIP
    if (state->mode == water_tracker_mode_norm) {
        sprintf(buf, "WA%2d%d",
                (state->cup_idx + 1),
                state->amount;
                state->nums[state->card[state->card_idx].pos[0]]
                );
    } else if (state->mode == water_tracker_mode_edit) {
        sprintf(buf, "WA%2d%d",
                (state->cup_idx + 1),
                _cup_sizes[state->cup_idx]
                );
    }

    // WIP
    // blink to indicate selected cup
    /* if (state->mode == water_tracker_mode_edit) { */
    /*     if (subsecond % 4 == 0) { */
    /*         buf[4] = ' '; */
    /*     } */
    /* } */

    watch_display_string(buf, 0);
}

void water_tracker_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr) {
    (void) settings;
    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(water_tracker_state_t));
        memset(*context_ptr, 0, sizeof(water_tracker_state_t));
        water_tracker_state_t *state = (water_tracker_state_t *)*context_ptr;

        for (uint8_t i=0;i<WATER_TRACKER_MAX_DAYS_TO_TRACK;i++) {
            state->amount[i] = 0;
        }
    }
    state->cup_idx = 1;  // default, 500ml
}

void water_tracker_face_activate(movement_settings_t *settings, void *context) {
    (void) settings;
    water_tracker_state_t *state = (water_tracker_state_t *)context;

    // Handle any tasks related to your watch face coming on screen.
}

bool water_tracker_face_loop(movement_event_t event, movement_settings_t *settings, void *context) {
    water_tracker_state_t *state = (water_tracker_state_t *)context;

    switch (event.event_type) {
        case EVENT_ACTIVATE:
            break;
        case EVENT_TICK:
            _water_tracker_face_draw(state, event.subsecond);  // TESTING
            break;
        case EVENT_LIGHT_BUTTON_UP:
            // You can use the Light button for your own purposes. Note that by default, Movement will also
            // illuminate the LED in response to EVENT_LIGHT_BUTTON_DOWN; to suppress that behavior, add an
            // empty case for EVENT_LIGHT_BUTTON_DOWN.
            break;
        case EVENT_ALARM_BUTTON_UP:
            if (state->mode == water_tracker_mode_norm) {
                state->cup_idx = (state->cup_idx + 1) % WATER_TRACKER_NUM_CUP_SIZES;
                state->mode = water_tracker_mode_edit;
            } else if (state->mode == water_tracker_mode_edit) {
                state->cup_idx = (state->cup_idx + 1) % WATER_TRACKER_NUM_CUP_SIZES;
            }
            _water_tracker_face_draw(state, event.subsecond);
            break;
        case EVENT_ALARM_LONG_PRESS:
            if (state->mode == water_tracker_mode_norm) {
                state->amount += _cup_sizes[state->cup_idx];
            } else if (state->mode == water_tracker_mode_edit) {
                state->mode = water_tracker_mode_norm;
            }
            _water_tracker_face_draw(state, event.subsecond);
            break;
        case EVENT_TIMEOUT:
            // Your watch face will receive this event after a period of inactivity. If it makes sense to resign,
            // you may uncomment this line to move back to the first watch face in the list:
            // movement_move_to_face(0);
            break;
        case EVENT_LOW_ENERGY_UPDATE:
            // If you did not resign in EVENT_TIMEOUT, you can use this event to update the display once a minute.
            // Avoid displaying fast-updating values like seconds, since the display won't update again for 60 seconds.
            // You should also consider starting the tick animation, to show the wearer that this is sleep mode:
            // watch_start_tick_animation(500);
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

void water_tracker_face_resign(movement_settings_t *settings, void *context) {
    (void) settings;
    (void) context;

    // handle any cleanup before your watch face goes off-screen.
}

