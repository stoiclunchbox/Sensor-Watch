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
#include "recycle_bin_face.h"

void recycle_bin_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr) {
    (void) settings;
    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(recycle_bin_state_t));
        memset(*context_ptr, 0, sizeof(recycle_bin_state_t));
        recycle_bin_state_t *state = (recycle_bin_state_t *)*context_ptr;

        // initialise to zero
        state->datetime_start.reg = 0;

        // Set to the day after a green bin day
        //  (ie the first day of a seven day period where you SHOULD
        //  put out the recycling bin)
        // BUG off by a day, says yes from 13-19 apr 23 (inclusive)
        state->datetime_start.unit.year  = 23;
        state->datetime_start.unit.month = 3;
        state->datetime_start.unit.day   = 31;
    }
}

void recycle_bin_face_activate(movement_settings_t *settings, void *context) {
    (void) settings;
    (void) context;
}

bool recycle_bin_face_loop(movement_event_t event, movement_settings_t *settings, void *context) {
    recycle_bin_state_t *state = (recycle_bin_state_t *)context;
    char buf[14];

    watch_date_time datetime_now = watch_rtc_get_date_time();
    datetime_now.unit.year += WATCH_RTC_REFERENCE_YEAR;
    watch_date_time difference;
    bool bin_week = 0;

    switch (event.event_type) {
        case EVENT_ACTIVATE:
            difference.reg = (datetime_now.reg - state->datetime_start.reg);
            bin_week = (!((difference.unit.day / 7) % 2));
            sprintf(buf, "RC   thu %c", bin_week ? 'y' : 'n');
            watch_display_string(buf, 0);
            break;
        case EVENT_LIGHT_BUTTON_UP:
            break;
        case EVENT_ALARM_BUTTON_UP:
            break;
        case EVENT_TIMEOUT:
            movement_move_to_face(0);
            break;
        default:
            return movement_default_loop_handler(event, settings);
    }

    return true;
}

void recycle_bin_face_resign(movement_settings_t *settings, void *context) {
    (void) settings;
    (void) context;
}

