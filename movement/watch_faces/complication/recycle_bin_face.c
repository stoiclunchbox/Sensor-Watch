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
#include "watch_utility.h"
#include "recycle_bin_face.h"

void recycle_bin_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr) {
    (void) settings;
    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(recycle_bin_state_t));
        memset(*context_ptr, 0, sizeof(recycle_bin_state_t));
        recycle_bin_state_t *state = (recycle_bin_state_t *)*context_ptr;

        // initialise to zero
        state->datetime_start.reg = 0;

        // TODO refine these instructions
        // Set to the day after a green bin day
        //  (ie the first day of a seven day period where you SHOULD
        //  put out the recycling bin)
        //  ..which includes the bin day itself, incase you check it that
        //  morning
        state->datetime_start.unit.year  = 23;
        state->datetime_start.unit.month = 3;
        state->datetime_start.unit.day   = 17;
    }
}

void recycle_bin_face_activate(movement_settings_t *settings, void *context) {
    (void) settings;
    recycle_bin_state_t *state = (recycle_bin_state_t *)context;

    state->datetime_now = watch_rtc_get_date_time();
        // NOTE This works, but..
        // TODO find out what we should be doing.
        // REVIEW correctly apply WATCH_RTC_REFERENCE_YEAR offset
        /* state->datetime_now.unit.year += WATCH_RTC_REFERENCE_YEAR; */
        state->datetime_now.unit.year += 20;
    state->datetime_now.unit.hour   = 0;
    state->datetime_now.unit.minute = 0;
    state->datetime_now.unit.second = 0;
    state->int_start = watch_utility_date_time_to_unix_time(state->datetime_start, 0);
    state->int_now = watch_utility_date_time_to_unix_time(state->datetime_now, 0);
}

bool recycle_bin_face_loop(movement_event_t event, movement_settings_t *settings, void *context) {
    recycle_bin_state_t *state = (recycle_bin_state_t *)context;
    char buf[12];

    switch (event.event_type) {
        case EVENT_ACTIVATE:
            state->int_difference = ((state->int_now - state->int_start) / 86400);
            state->bin_week = (!((state->int_difference / 7) % 2));
            sprintf(buf, "RC   thu %c", state->bin_week ? 'y' : 'n');
            watch_display_string(buf, 0);
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

