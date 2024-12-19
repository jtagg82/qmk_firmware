// Copyright 2021 jtagg82 (@jtagg82)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "matrix.h"
<<<<<<< HEAD
=======
#include "gpio.h"
>>>>>>> 6230f86d736c141d830ab429f3cddc222d2bb8de

// The 3 pins needed to control the shift registers
#define SR_INPUT D7
#define SR_LATCH C6
#define SR_SHIFT E6

// The actual pins used on the Pro Micro to read the rows
#define ROW1 F4
#define ROW2 F5
#define ROW3 F6
#define ROW4 F7
#define ROW5 B1
#define ROW6 B3

/*
The Pro Micro doesn't have enough pins for all columns and rows for a full keyboard (6 rows and 21 columns).
To reduce the number of pins needed for the columns, 3 8-bit shift registers are used in series. This works well
because only one column should ever be active at a time. This reduces the number of pins used for columns
from 21 to 3.

The idea of this matrix scan is to activate the first bit of the shift register for the first column. Subsequent
columns are activated by shifting the bit. Since the shift registers used have separate shift and latch functions,
we must always shift and latch to validate the new output.

74HC595 Shift register
      ____ ____
     |    U    |
 Qb [| 1 o  16 |] Vcc
 Qc [| 2    15 |] Qa
 Qd [| 3    14 |] Input
 Qe [| 4    13 |] Output Enable
 Qf [| 5    12 |] Latch Clock
 Qg [| 6    11 |] Shift Clock
 Qh [| 7    10 |] Reset
GND [| 8     9 |] SQh
     |_________|

*/


void matrix_init_custom(void) {
    // Set the pins that control the shift registers as outputs
<<<<<<< HEAD
    gpio_set_pin_output(SR_INPUT);
    gpio_set_pin_output(SR_LATCH);
    gpio_set_pin_output(SR_SHIFT);

    // Set the row pins as inputs that are pulled HIGH when unconnected
    gpio_set_pin_input_high(ROW1);
    gpio_set_pin_input_high(ROW2);
    gpio_set_pin_input_high(ROW3);
    gpio_set_pin_input_high(ROW4);
    gpio_set_pin_input_high(ROW5);
    gpio_set_pin_input_high(ROW6);
=======
    setPinOutput(SR_INPUT);
    setPinOutput(SR_LATCH);
    setPinOutput(SR_SHIFT);

    // Set the row pins as inputs that are pulled HIGH when unconnected
    setPinInputHigh(ROW1);
    setPinInputHigh(ROW2);
    setPinInputHigh(ROW3);
    setPinInputHigh(ROW4);
    setPinInputHigh(ROW5);
    setPinInputHigh(ROW6);
>>>>>>> 6230f86d736c141d830ab429f3cddc222d2bb8de
}

inline void shift(void) {
    // toggles both shifting and latching of the shift register
<<<<<<< HEAD
    gpio_write_pin_high(SR_SHIFT);
    gpio_write_pin_low(SR_SHIFT);
    gpio_write_pin_high(SR_LATCH);
    gpio_write_pin_low(SR_LATCH);
=======
    writePinHigh(SR_SHIFT);
    writePinLow(SR_SHIFT);
    writePinHigh(SR_LATCH);
    writePinLow(SR_LATCH);
>>>>>>> 6230f86d736c141d830ab429f3cddc222d2bb8de
}

void reset_col(void) {
    // Sets the input of the shift register LOW, which will start a new scan by activating the first column
<<<<<<< HEAD
    gpio_write_pin_low(SR_INPUT);
=======
    writePinLow(SR_INPUT);
>>>>>>> 6230f86d736c141d830ab429f3cddc222d2bb8de
    shift();
}

void next_col(void) {
    // When shifting to the next output, we input a HIGH, which means that we are not activating any further columns
<<<<<<< HEAD
    gpio_write_pin_high(SR_INPUT);
=======
    writePinHigh(SR_INPUT);
>>>>>>> 6230f86d736c141d830ab429f3cddc222d2bb8de
    shift();
}


static uint8_t read_rows(void) {
    // Read all rows and return them as a single byte
<<<<<<< HEAD
    return ((gpio_read_pin(ROW1) ? 0 : 1)  |
	        (gpio_read_pin(ROW2) ? 0 : 2)  |
	        (gpio_read_pin(ROW3) ? 0 : 4)  |
            (gpio_read_pin(ROW4) ? 0 : 8)  |
            (gpio_read_pin(ROW5) ? 0 : 16) |
            (gpio_read_pin(ROW6) ? 0 : 32));
=======
    return ((readPin(ROW1) ? 0 : 1)  |
	        (readPin(ROW2) ? 0 : 2)  |
	        (readPin(ROW3) ? 0 : 4)  |
            (readPin(ROW4) ? 0 : 8)  |
            (readPin(ROW5) ? 0 : 16) |
            (readPin(ROW6) ? 0 : 32));
>>>>>>> 6230f86d736c141d830ab429f3cddc222d2bb8de
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {

    bool changed = false;

    reset_col(); // Activates first column
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
<<<<<<< HEAD
        //matrix_io_delay();
=======
        matrix_io_delay();
>>>>>>> 6230f86d736c141d830ab429f3cddc222d2bb8de
        uint8_t rows = read_rows();

        // The following for block checks whether anything changed since the last scan
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            bool prev_bit = current_matrix[row] & ((matrix_row_t)1 << col);
            bool curr_bit = rows & (1 << row);

            if (prev_bit != curr_bit) {
                current_matrix[row] ^= ((matrix_row_t)1 << col);
                changed = true;
            }
        }
        next_col(); // Shifts activated column to the next one
    }

    return changed;
}
