#include "button.h"
#include "iohelper.h"
#include "msp.h"

#define DEBOUNCE_TIME 10000
void debounce(void) {
    int i;
    for (i = 0; i < DEBOUNCE_TIME; i++);
}

button_t constructButton(pin_t pin, uint8_t exists) {
    button_t button;
    button.pin = pin;
    button.exists = exists;
    button.status = NotPressed;
    button.onPress = NULL;
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

void registerButtonPressEvent(button_t *btn, void (*event)(void)) {
    btn->onPress = event;
}
void updateButton(button_t *btn) {
    buttonState_t status = getButtonState(*btn);
    // Check if button is pressed
    if (status == Pressed && btn->status != Pressed) {
        debounce();
        btn->status = Pressed;
        return;
    }
    // Check if button has been released
    if (status == NotPressed && btn->status == Pressed) {
        debounce();
        btn->status = NotPressed;
        if (btn->onPress != NULL) {
            btn->onPress();
        }
        return;
    }
}
