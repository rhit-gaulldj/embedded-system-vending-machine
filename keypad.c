#include "keypad.h"
#include "iohelper.h"
#include <stddef.h>

struct keypad_pins {
    pin_t pin0;
    pin_t pin1;
    pin_t pin2;
    pin_t pin3;
    pin_t pin4;
    pin_t pin5;
    pin_t pin6;
    pin_t pin7;
};
struct keypad_pins pins;

const keyType_t allKeys[NUM_KEYS] = {
                               KeyPound,KeyStar,
                               Key0,Key9,Key8,Key7,Key6,Key5,Key4,Key3,Key2,Key1,
                               KeyD,KeyC,KeyB,KeyA
};

void initKeypad(pin_t pin0, pin_t pin1, pin_t pin2, pin_t pin3,
                pin_t pin4, pin_t pin5, pin_t pin6, pin_t pin7) {
    // Columns are output
    initOutputPin(pin0, Low);
    initOutputPin(pin1, Low);
    initOutputPin(pin2, Low);
    initOutputPin(pin3, Low);
    // Rows are polled to get the button, so they're inputs
    initInputPin(pin4, Pullup);
    initInputPin(pin5, Pullup);
    initInputPin(pin6, Pullup);
    initInputPin(pin7, Pullup);

    pins.pin0 = pin0;
    pins.pin1 = pin1;
    pins.pin2 = pin2;
    pins.pin3 = pin3;
    pins.pin4 = pin4;
    pins.pin5 = pin5;
    pins.pin6 = pin6;
    pins.pin7 = pin7;
}

keys_t getPressedKeys() {
    keys_t result = 0;

    // Pull the columns (pin 0-3) low one by one, and read the rows
    // Check the first column
    // Pins default to being low so we need to pull them high right now
    setOutput(pins.pin1);
    setOutput(pins.pin2);
    setOutput(pins.pin3);
    clearOutput(pins.pin0);
    if (!readPin(pins.pin7)) {
        result |= KEYA_MASK;
    }
    if (!readPin(pins.pin6)) {
        result |= KEYB_MASK;
    }
    if (!readPin(pins.pin5)) {
        result |= KEYC_MASK;
    }
    if (!readPin(pins.pin4)) {
        result |= KEYD_MASK;
    }
    // Check the second column
    setOutput(pins.pin0);
    clearOutput(pins.pin1);
    if (!readPin(pins.pin7)) {
        result |= KEY3_MASK;
    }
    if (!readPin(pins.pin6)) {
        result |= KEY6_MASK;
    }
    if (!readPin(pins.pin5)) {
        result |= KEY9_MASK;
    }
    if (!readPin(pins.pin4)) {
        result |= KEYPOUND_MASK;
    }
    // Check the third column
    setOutput(pins.pin1);
    clearOutput(pins.pin2);
    if (!readPin(pins.pin7)) {
        result |= KEY2_MASK;
    }
    if (!readPin(pins.pin6)) {
        result |= KEY5_MASK;
    }
    if (!readPin(pins.pin5)) {
        result |= KEY8_MASK;
    }
    if (!readPin(pins.pin4)) {
        result |= KEY0_MASK;
    }
    // Finally, check the fourth column
    setOutput(pins.pin2);
    clearOutput(pins.pin3);
    if (!readPin(pins.pin7)) {
        result |= KEY1_MASK;
    }
    if (!readPin(pins.pin6)) {
        result |= KEY4_MASK;
    }
    if (!readPin(pins.pin5)) {
        result |= KEY7_MASK;
    }
    if (!readPin(pins.pin4)) {
        result |= KEYSTAR_MASK;
    }
    // Clear output of other pins to save power
    clearOutput(pins.pin0);
    clearOutput(pins.pin1);
    clearOutput(pins.pin2);

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
