/*
 * MIT License
 *
 * Copyright (c) 2022 Mikhail Svarichevsky
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
 *
 * Displays some pre-defined data that you might want to remember.
 * Math constants, birthdays, phone numbers...
 */

#include <stdlib.h>
#include <string.h>
#include "databank_face.h"
#include "watch.h"
#include "watch_private_display.h"

const char *pi_data[] = {
    "DB", " dAtA",
    "BA", "0431  091687",
    "HE", "HEgH",

    "DB", "ntroPy",
    "2 ", "4",
    "3 ", "8",
    "4 ", "16",
    "6 ", "64",
    "8 ", "255",
    "Nt", "16    65536",
    "Nt", "32    4-29H9  4294967296",
    "Nt", "64    1-8H19    18446744073709551616",

    "DB", "ConSts",
    "PI", "3.1415926535897932",
    "TU", "6.2831853071795864",
    "e ", "2.7182818284590452",
    "GR", "1.6180339887498948",
    "c ", "299792458    M/s",
    "g ", "9.81   M/s2",
    "MR", "12.35  us/nM rEtrn",
};
//we show 6 characters per screen

const int databank_num_pages = (sizeof(pi_data) / sizeof(char*) / 2);

struct {
    uint8_t current_word;
    uint8_t databank_page;
    bool animating;
} databank_state;

void databank_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr) {
    // These next two lines just silence the compiler warnings associated with unused parameters.
    // We have no use for the settings or the watch_face_index, so we make that explicit here.
    (void) settings;
    (void) context_ptr;
    (void) watch_face_index;
    // At boot, context_ptr will be NULL indicating that we don't have anyplace to store our context.
    // on probation reset to first page when waking from sleep
    databank_state.databank_page = 0;
}

void databank_face_activate(movement_settings_t *settings, void *context) {
    // same as above: silence the warning, we don't need to check the settings.
    (void) settings;
    (void) context;
    // we do however need to set some things in our context. Here we cast it to the correct type...
    databank_state.current_word = 0;
    databank_state.animating = true;
}

static void display() {
    char buf[14];
    int max_words = (strlen(pi_data[databank_state.databank_page * 2 + 1]) - 1) / 6 + 1;
    int word = databank_state.current_word;
    watch_clear_indicator(WATCH_INDICATOR_LAP);
    if (max_words < 2) {
        sprintf(buf, "%s  ", pi_data[databank_state.databank_page * 2 + 0]);
    } else {
        if (word == 0) {
            watch_set_indicator(WATCH_INDICATOR_LAP);
            sprintf(buf, "%s%2d", pi_data[databank_state.databank_page * 2 + 0], max_words);
        } else {
            sprintf(buf, "%s%2d", pi_data[databank_state.databank_page * 2 + 0], (word + 1));
        }
    }
    watch_display_string(buf, 0);
    bool data_ended = false;
    for (int i = 0; i < 6; i++) {
        if (pi_data[databank_state.databank_page * 2 + 1][word * 6 + i] == 0) {
            data_ended = true;
        }

        if (!data_ended) {
            watch_display_character(pi_data[databank_state.databank_page * 2 + 1][word * 6 + i], 4 + i);
        } else {
            // only 6 digits per page
            watch_display_character(' ', 4 + i);
        }
    }
}

bool databank_face_loop(movement_event_t event, movement_settings_t *settings, void *context) {
    (void) settings;
    (void) context;
    int max_words = (strlen(pi_data[databank_state.databank_page * 2 + 1]) - 1) / 6 + 1;

    //TODO add long light press to reset:
    //      a) first word
    //      b) if (first word) first page
    //  or
    //  long light press for light, and
    //  light up for reset
    //  (align to memory_device)

    switch (event.event_type) {
        case EVENT_ACTIVATE:
             display();
        case EVENT_TICK:
            // on activate and tick, if we are animating,
            break;
        case EVENT_LIGHT_BUTTON_UP:
            movement_illuminate_led();  // so we can see at night
            /* // when the user presses 'light', we illuminate the LED. We could override this if */
            /* // our UI needed an additional button for input, consuming the light button press */
            /* // but not illuminating the LED. */
            /* databank_state.current_word = (databank_state.current_word + max_words - 1) % max_words; */
            /* display(); */
            break;
        case EVENT_LIGHT_LONG_PRESS:
            databank_state.databank_page = (databank_state.databank_page + databank_num_pages - 1) % databank_num_pages;
            databank_state.current_word = 0;
            display();
            break;
        case EVENT_ALARM_LONG_PRESS:
            databank_state.current_word = (databank_state.current_word + 1) % max_words;
            display();
            break;
        case EVENT_ALARM_BUTTON_UP:
            databank_state.databank_page = (databank_state.databank_page + 1) % databank_num_pages;
            databank_state.current_word = 0;
            display();
            break;
        case EVENT_LOW_ENERGY_UPDATE:
            // This low energy mode update occurs once a minute, if the watch face is in the
            // foreground when Movement enters low energy mode. We have the option of supporting
            // this mode, but since our watch face animates once a second, the "Hello there" face
            // isn't very useful in this mode. So we choose not to support it. (continued below)
            break;
        case EVENT_TIMEOUT:
            // ... Instead, we respond to the timeout event. This event happens after a configurable
            // interval on screen (1-30 minutes). The watch will give us this event as a chance to
            // resign control if we want to, and in this case, we do.
            // This function will return the watch to the first screen (usually a simple clock),
            // and it will do it long before the watch enters low energy mode. This ensures we
            // won't be on screen, and thus opts us out of getting the EVENT_LOW_ENERGY_UPDATE above.
            movement_move_to_face(0);
            break;
        case EVENT_LIGHT_BUTTON_DOWN:
            // don't light up every time light is hit
            break;
        default:
            movement_default_loop_handler(event, settings);
            break;
    }

    return true;
}

void databank_face_resign(movement_settings_t *settings, void *context) {
    // our watch face, like most watch faces, has nothing special to do when resigning.
    // watch faces that enable a peripheral or interact with a sensor may want to turn it off here.
    (void) settings;
    (void) context;
}
