#include "button.h"
#include "iohelper.h"
#include "msp.h"

button_t constructButton(pin_t pin, uint8_t exists) {
    button_t button;
    button.pin = pin;
    button.exists = exists;
    return button;
}
void initButton(button_t btn) {
    initInputPin(btn.pin, Pullup);
}

buttonState_t getButtonState(button_t btn) {
    uint8_t v = readPin(btn.pin);
    if (v) {
        return NotPressed;
    }
    return Pressed;
}
