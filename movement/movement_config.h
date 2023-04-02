/*
 * MIT License
 *
 * Copyright (c) 2022 Joey Castillo
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

#ifndef MOVEMENT_CONFIG_H_
#define MOVEMENT_CONFIG_H_

#include "movement_faces.h"

const watch_face_t watch_faces[] = {
    simple_clock_face,
    //INTERNAL
    stock_stopwatch_face,
    timer_face,
    alarm_face,
    counter_face,
    //water
    thermistor_readout_face,
    ratemeter_face,
    pulsometer_face,
    tachymeter_face,
    //telemeter
        //TIME PASSAGE
        day_one_face,
        time_left_face,
        //actual time left?
    tomato_face,
    probability_face,
    databank_face,
        rpn_calculator_face,
        rpn_calculator_alt_face,
        habit_face, //work out if I want to keep / use
        interval_face,
        ships_bell_face,

    memory_device_face,
    //EXTERNAL
    world_clock2_face,
    sunrise_sunset_face,
    moon_phase_face,
    //tide clock
    //due (rubbish bin)
    recycle_bin_face,
    preferences_face,
    set_time_face,
    voltage_face,
    finetune_face,
    nanosec_face,
        thermistor_logging_face,
    totp_face,
    totp_face_lfs,
        character_set_face,
        chirpy_demo_face,
};

#define MOVEMENT_NUM_FACES (sizeof(watch_faces) / sizeof(watch_face_t))
#define MOVEMENT_SECONDARY_FACE_INDEX (MOVEMENT_NUM_FACES - 15)

#endif // MOVEMENT_CONFIG_H_
