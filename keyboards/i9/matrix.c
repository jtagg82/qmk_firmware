#include "matrix.h"
#include "gpio.h"

#define SR_INPUT B4
#define SR_LATCH E6
#define SR_SHIFT B5


void matrix_init_custom(void) {
    setPinOutput(SR_INPUT);
    setPinOutput(SR_LATCH);
    setPinOutput(SR_SHIFT);

    setPinInputHigh(F4);
    setPinInputHigh(F5);
    setPinInputHigh(F6);
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
    return (readPin(F4) ? 0 : 1 //|
	    //readPin(F5) ? 0 : 2 |
	    //readPin(F6) ? 0 : 4
	   );
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
