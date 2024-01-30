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
//pin_t *submitPin;
//pin_t *clearPin;

void initKeypad(pin_t pin0, pin_t pin1, pin_t pin2, pin_t pin3,
                pin_t pin4, pin_t pin5, pin_t pin6, pin_t pin7){//,
                //pin_t *clearBtnPin, pin_t *submitBtnPin) {
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
    // Buttons may not be external, so they are pointers. Only initialize if the pointer isn't null
//    if (clearBtnPin != NULL) {
//        initInputPin(*clearBtnPin, Pullup);
//    }
//    if (submitBtnPin != NULL) {
//        initInputPin(*submitBtnPin, Pullup);
//    }

//    submitPin = submitBtnPin;
//    clearPin = clearBtnPin;
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

    // Pull the columns (pin 0-3) high/low one by one, and read the rows
    // Check the first column
    clearOutput(pins.pin1);
    clearOutput(pins.pin2);
    clearOutput(pins.pin3);
    setOutput(pins.pin0);
    if (readPin(pins.pin7)) {
        result |= KEYA_MASK;
    }
    if (readPin(pins.pin6)) {
        result |= KEYB_MASK;
    }
    if (readPin(pins.pin5)) {
        result |= KEYC_MASK;
    }
    if (readPin(pins.pin4)) {
        result |= KEYD_MASK;
    }
    // Check the second column
    clearOutput(pins.pin0);
    setOutput(pins.pin1);
    if (readPin(pins.pin7)) {
        result |= KEY3_MASK;
    }
    if (readPin(pins.pin6)) {
        result |= KEY6_MASK;
    }
    if (readPin(pins.pin5)) {
        result |= KEY9_MASK;
    }
    if (readPin(pins.pin4)) {
        result |= KEYPOUND_MASK;
    }
    // Check the third column
    clearOutput(pins.pin1);
    setOutput(pins.pin2);
    if (readPin(pins.pin7)) {
        result |= KEY2_MASK;
    }
    if (readPin(pins.pin6)) {
        result |= KEY5_MASK;
    }
    if (readPin(pins.pin5)) {
        result |= KEY8_MASK;
    }
    if (readPin(pins.pin4)) {
        result |= KEY0_MASK;
    }
    // Finally, check the fourth column
    clearOutput(pins.pin2);
    setOutput(pins.pin3);
    if (readPin(pins.pin7)) {
        result |= KEY1_MASK;
    }
    if (readPin(pins.pin6)) {
        result |= KEY4_MASK;
    }
    if (readPin(pins.pin5)) {
        result |= KEY7_MASK;
    }
    if (readPin(pins.pin4)) {
        result |= KEYSTAR_MASK;
    }
    clearOutput(pins.pin3);

    return result;
}
