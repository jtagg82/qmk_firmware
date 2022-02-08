#include "matrix.h"
#include "gpio.h"

#define SR_INPUT D7
#define SR_LATCH C6
#define SR_SHIFT E6

#define ROW1 F4
#define ROW2 F5
#define ROW3 F6
#define ROW4 F7
#define ROW5 B1
#define ROW6 B3

void matrix_init_custom(void) {
    setPinOutput(SR_INPUT);
    setPinOutput(SR_LATCH);
    setPinOutput(SR_SHIFT);

    setPinInputHigh(ROW1);
    setPinInputHigh(ROW2);
    setPinInputHigh(ROW3);
    setPinInputHigh(ROW4);
    setPinInputHigh(ROW5);
    setPinInputHigh(ROW6);
}

void reset_col(void) {
    writePinLow(SR_INPUT);
    writePinHigh(SR_SHIFT); 
    writePinLow(SR_SHIFT);
    writePinHigh(SR_LATCH);
    writePinLow(SR_LATCH); 
}

void next_col(void) {
    writePinHigh(SR_INPUT);
    writePinHigh(SR_SHIFT); 
    writePinLow(SR_SHIFT);
    writePinHigh(SR_LATCH);
    writePinLow(SR_LATCH); 
}


static uint8_t read_rows(void) {
    return ((readPin(ROW1) ? 0 : 1)  |
	    (readPin(ROW2) ? 0 : 2)  |
	    (readPin(ROW3) ? 0 : 4)  |
	    (readPin(ROW4) ? 0 : 8)  |
	    (readPin(ROW5) ? 0 : 16) |
	    (readPin(ROW6) ? 0 : 32));
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {

    bool changed = false;

    reset_col();
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        matrix_io_delay();
        uint8_t rows = read_rows();

        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            bool prev_bit = current_matrix[row] & ((matrix_row_t)1 << col);
            bool curr_bit = rows & (1 << row);

            if (prev_bit != curr_bit) {
                current_matrix[row] ^= ((matrix_row_t)1 << col);
                changed = true;
            }
        }
        next_col();
    }

    return changed;
}
