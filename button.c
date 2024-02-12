#include "button.h"
#include "iohelper.h"
#include "msp.h"

#define MAX_BUTTONS     16
// Store buttons for use with interrupt handlers
button_t *interruptButtons[MAX_BUTTONS];
uint8_t buttonListTail; // Index of next open slot

void handleInterrupt(uint8_t port, uint8_t pin);

void initButtonModule() {
    buttonListTail = 0;
}

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
    // Set up our interrupt handler
    if (buttonListTail >= MAX_BUTTONS) {
        // List is full
        return;
    }
    interruptButtons[buttonListTail] = btn;
    buttonListTail++;
    registerPortInterruptHandler(&(btn->pin), HighToLow, handleInterrupt);
}
void handleInterrupt(uint8_t port, uint8_t pin) {
    // Check every button, update status and call handlers if needed
    int i;
    for (i = 0; i < buttonListTail; i++) {
        button_t *btn = interruptButtons[i];
        if (btn->pin.pin == pin && btn->pin.port.portNum == port) {
            buttonState_t status = getButtonState(*btn); // We expect this to always be 1
            // Check if button is pressed
            if (status == Pressed && btn->status != Pressed) {
                debounce();
                btn->status = Pressed;
            }
            // Check if button has been released
            else if (status == NotPressed && btn->status == Pressed) {
                debounce();
                btn->status = NotPressed;
                if (btn->onPress != NULL) {
                    btn->onPress();
                }
            }
        }
    }
}

void updateButton(button_t *btn) {
    buttonState_t status = getButtonState(*btn);
    // Check if button is pressed (this is done in the interrupt handler now)
//    if (status == Pressed && btn->status != Pressed) {
//        debounce();
//        btn->status = Pressed;
//        return;
//    }
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
