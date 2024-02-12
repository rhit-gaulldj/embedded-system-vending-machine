#include "keypad.h"
#include "iohelper.h"
#include <stddef.h>

#define KEY_PRESSED_VALUE   0

struct keypad_pins {
    pin_t row1;
    pin_t row2;
    pin_t row3;
    pin_t row4;
    pin_t col1;
    pin_t col2;
    pin_t col3;
    pin_t col4;
};
struct keypad_pins pins;

const keyType_t allKeys[NUM_KEYS] = {
                               KeyPound,KeyStar,
                               Key0,Key9,Key8,Key7,Key6,Key5,Key4,Key3,Key2,Key1,
                               KeyD,KeyC,KeyB,KeyA
};

void initKeypad(pin_t pin1, pin_t pin2, pin_t pin3, pin_t pin4,
                pin_t pin5, pin_t pin6, pin_t pin7, pin_t pin8) {
    // https://www.futurlec.com/Keypad4x4.shtml
    pins.row1 = pin8;
    pins.row2 = pin1;
    pins.row3 = pin2;
    pins.row4 = pin4;
    pins.col1 = pin3;
    pins.col2 = pin5;
    pins.col3 = pin6;
    pins.col4 = pin7;

    // Columns are output
    initOutputPin(pins.col1, Low);
    initOutputPin(pins.col2, Low);
    initOutputPin(pins.col3, Low);
    initOutputPin(pins.col4, Low);
    // Rows are polled to get the button, so they're inputs
    initInputPin(pins.row1, Pullup);
    initInputPin(pins.row2, Pullup);
    initInputPin(pins.row3, Pullup);
    initInputPin(pins.row4, Pullup);
}

keys_t getPressedKeys() {
    keys_t result = 0;

    // Pull the columns low one by one, and read the rows
    // Check the first column
    // Pins default to being low so we need to pull them high right now
    setOutput(pins.col2);
    setOutput(pins.col3);
    setOutput(pins.col4);
    clearOutput(pins.col1);
    if (readPin(pins.row1) == KEY_PRESSED_VALUE) {
        result |= KEY1_MASK;
    }
    if (readPin(pins.row2) == KEY_PRESSED_VALUE) {
        result |= KEY4_MASK;
    }
    if (readPin(pins.row3) == KEY_PRESSED_VALUE) {
        result |= KEY7_MASK;
    }
    if (readPin(pins.row4) == KEY_PRESSED_VALUE) {
        result |= KEYSTAR_MASK;
    }
    // Check the second column
    setOutput(pins.col1);
    clearOutput(pins.col2);
    if (readPin(pins.row1) == KEY_PRESSED_VALUE) {
        result |= KEY2_MASK;
    }
    if (readPin(pins.row2) == KEY_PRESSED_VALUE) {
        result |= KEY5_MASK;
    }
    if (readPin(pins.row3) == KEY_PRESSED_VALUE) {
        result |= KEY8_MASK;
    }
    if (readPin(pins.row4) == KEY_PRESSED_VALUE) {
        result |= KEY0_MASK;
    }
    // Check the third column
    setOutput(pins.col2);
    clearOutput(pins.col3);
    char test = P4->IN;
    if (readPin(pins.row1) == KEY_PRESSED_VALUE) {
        result |= KEY3_MASK;
    }
    if (readPin(pins.row2) == KEY_PRESSED_VALUE) {
        result |= KEY6_MASK;
    }
    if (readPin(pins.row3) == KEY_PRESSED_VALUE) {
        result |= KEY9_MASK;
    }
    if (readPin(pins.row4) == KEY_PRESSED_VALUE) {
        result |= KEYPOUND_MASK;
    }
    // Finally, check the fourth column
    setOutput(pins.col3);
    clearOutput(pins.col4);
    if (readPin(pins.row1) == KEY_PRESSED_VALUE) {
        result |= KEYA_MASK;
    }
    if (readPin(pins.row2) == KEY_PRESSED_VALUE) {
        result |= KEYB_MASK;
    }
    if (readPin(pins.row3) == KEY_PRESSED_VALUE) {
        result |= KEYC_MASK;
    }
    if (readPin(pins.row4) == KEY_PRESSED_VALUE) {
        result |= KEYD_MASK;
    }
    // Clear output of other pins to save power
    clearOutput(pins.col1);
    clearOutput(pins.col2);
    clearOutput(pins.col3);

    return result;
}

char getCharForKey(keyType_t key) {
    switch (key) {
        case KeyA:
            return 'A';
        case KeyB:
            return 'B';
        case KeyC:
            return 'C';
        case KeyD:
            return 'D';
        case Key0:
            return '0';
        case Key1:
            return '1';
        case Key2:
            return '2';
        case Key3:
            return '3';
        case Key4:
            return '4';
        case Key5:
            return '5';
        case Key6:
            return '6';
        case Key7:
            return '7';
        case Key8:
            return '8';
        case Key9:
            return '9';
        case KeyPound:
            return '#';
        case KeyStar:
            return '*';
    }
    return ' ';
}

keyCategory_t getKeyCategory(keyType_t key) {
    switch (key) {
        case Key0:
        case Key1:
        case Key2:
        case Key3:
        case Key4:
        case Key5:
        case Key6:
        case Key7:
        case Key8:
        case Key9:
            return KCNumber;
        case KeyA:
        case KeyB:
        case KeyC:
        case KeyD:
            return KCLetter;
        case KeyStar:
        case KeyPound:
            return KCOther;
    }
    return KCOther;
}
