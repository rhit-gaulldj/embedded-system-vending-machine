#include "iohelper.h"

void initOutputPin(pin_t pin, pinValue_t initValue) {
    port_t port = pin.port;
    uint8_t mask = 1 << pin.pin;

    // SEL = 00
    *(port.SEL0) &= ~mask;
    *(port.SEL1) &= ~mask;
    // DIR = 1 for output, 0 for input
    *(port.DIR) |= mask;
    // Set OUT to initial value
    if (initValue == High) {
        *(port.OUT) |= mask;
    } else if (initValue == Low) {
        *(port.OUT) &= ~mask;
    }
}
void initInputPin(pin_t pin, resistorType_t resType) {
    port_t port = pin.port;
    uint8_t mask = 1 << pin.pin;

    // SEL = 00
    *(port.SEL0) &= ~mask;
    *(port.SEL1) &= ~mask;
    // DIR = 1 for output, 0 for input
    *(port.DIR) &= ~mask;
    // Disable resistors for now
    *(port.REN) &= ~mask;
}

void setOutput(pin_t pin) {
    port_t port = pin.port;
    uint8_t mask = 1 << pin.pin;
    *(port.OUT) |= mask;
}
void clearOutput(pin_t pin) {
    port_t port = pin.port;
    uint8_t mask = 1 << pin.pin;
    *(port.OUT) &= ~mask;
}
void toggleOutput(pin_t pin) {
    port_t port = pin.port;
    uint8_t mask = 1 << pin.pin;
    *(port.OUT) ^= mask;
}
uint8_t readPin(pin_t pin) {
    port_t port = pin.port;
    uint8_t mask = 1 << pin.pin;
    uint8_t value;
    value = *(port.IN) & mask;
    if (value > 0) {
        return 1;
    }
    return 0;
}
