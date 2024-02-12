#include "iohelper.h"
#include "msp.h"

port_t Port1;
port_t Port2;
port_t Port3;
port_t Port4;
port_t Port5;
port_t Port6;

pin_t P1P1;
pin_t P1P4;
pin_t P1P5;
pin_t P1P6;
pin_t P1P7;

pin_t P2P3;
pin_t P2P4;
pin_t P2P5;
pin_t P2P6;
pin_t P2P7;

pin_t P3P0;
pin_t P3P1;
pin_t P3P2;
pin_t P3P3;
pin_t P3P4;
pin_t P3P5;
pin_t P3P6;
pin_t P3P7;

pin_t P4P0;
pin_t P4P1;
pin_t P4P2;
pin_t P4P3;
pin_t P4P4;
pin_t P4P5;
pin_t P4P6;
pin_t P4P7;

pin_t P5P0;
pin_t P5P1;
pin_t P5P2;
pin_t P5P3;
pin_t P5P4;
pin_t P5P5;
pin_t P5P6;
pin_t P5P7;

pin_t P6P0;
pin_t P6P1;
pin_t P6P2;
pin_t P6P3;
pin_t P6P4;
pin_t P6P5;
pin_t P6P6;
pin_t P6P7;

// Can only have this many pin interrupt handlers registered
// This number should be plenty for our program
#define MAX_PIN_INTERRUPTS 16
pinInterruptEntry_t interruptPins[MAX_PIN_INTERRUPTS];
uint8_t interruptPinsTail; // Index of the first empty entry in the interruptPins array
uint8_t interruptNvicBits[6];

port_t initEvenPort(DIO_PORT_Even_Interruptable_Type *p, uint8_t num) {
    port_t port;
    port.DIR = &(p->DIR);
    port.IN = &(p->IN);
    port.OUT = &(p->OUT);
    port.REN = &(p->REN);
    port.SEL0 = &(p->SEL0);
    port.SEL1 = &(p->SEL1);
    port.IE = &(p->IE);
    port.IES = &(p->IES);
    port.portNum = num;
    return port;
}
port_t initOddPort(DIO_PORT_Odd_Interruptable_Type *p, uint8_t num) {
    port_t port;
    port.DIR = &(p->DIR);
    port.IN = &(p->IN);
    port.OUT = &(p->OUT);
    port.REN = &(p->REN);
    port.SEL0 = &(p->SEL0);
    port.SEL1 = &(p->SEL1);
    port.IE = &(p->IE);
    port.IES = &(p->IES);
    port.portNum = num;
    return port;
}

void initConstants(void) {
    interruptNvicBits[0] = PORT1_IRQn;
    interruptNvicBits[1] = PORT2_IRQn;
    interruptNvicBits[2] = PORT3_IRQn;
    interruptNvicBits[3] = PORT4_IRQn;
    interruptNvicBits[4] = PORT5_IRQn;
    interruptNvicBits[5] = PORT6_IRQn;

    Port1 = initOddPort(P1, 1);
    Port2 = initEvenPort(P2, 2);
    Port3 = initOddPort(P3, 3);
    Port4 = initEvenPort(P4, 4);
    Port5 = initOddPort(P5, 5);
    Port6 = initEvenPort(P6, 6);

    P1P1.port = Port1;
    P1P1.pin = 1;
    P1P4.port = Port1;
    P1P4.pin = 4;
    P1P5.port = Port1;
    P1P5.pin = 5;
    P1P6.port = Port1;
    P1P6.pin = 6;
    P1P7.port = Port1;
    P1P7.pin = 7;

    P2P3.port = Port2;
    P2P3.pin = 3;
    P2P4.port = Port2;
    P2P4.pin = 4;
    P2P5.port = Port2;
    P2P5.pin = 5;
    P2P6.port = Port2;
    P2P6.pin = 6;
    P2P7.port = Port2;
    P2P7.pin = 7;

    P3P0.port = Port3;
    P3P0.pin = 0;
    P3P1.port = Port3;
    P3P1.pin = 1;
    P3P2.port = Port3;
    P3P2.pin = 2;
    P3P3.port = Port3;
    P3P3.pin = 3;
    P3P4.port = Port3;
    P3P4.pin = 4;
    P3P5.port = Port3;
    P3P5.pin = 5;
    P3P6.port = Port3;
    P3P6.pin = 6;
    P3P7.port = Port3;
    P3P7.pin = 7;

    P4P0.port = Port4;
    P4P0.pin = 0;
    P4P1.port = Port4;
    P4P1.pin = 1;
    P4P2.port = Port4;
    P4P2.pin = 2;
    P4P3.port = Port4;
    P4P3.pin = 3;
    P4P4.port = Port4;
    P4P4.pin = 4;
    P4P5.port = Port4;
    P4P5.pin = 5;
    P4P6.port = Port4;
    P4P6.pin = 6;
    P4P7.port = Port4;
    P4P7.pin = 7;

    P5P0.port = Port5;
    P5P0.pin = 0;
    P5P1.port = Port5;
    P5P1.pin = 1;
    P5P2.port = Port5;
    P5P2.pin = 2;
    P5P3.port = Port5;
    P5P3.pin = 3;
    P5P4.port = Port5;
    P5P4.pin = 4;
    P5P5.port = Port5;
    P5P5.pin = 5;
    P5P6.port = Port5;
    P5P6.pin = 6;
    P5P7.port = Port5;
    P5P7.pin = 7;

    P6P0.port = Port6;
    P6P0.pin = 0;
    P6P1.port = Port6;
    P6P1.pin = 1;
    P6P2.port = Port6;
    P6P2.pin = 2;
    P6P3.port = Port6;
    P6P3.pin = 3;
    P6P4.port = Port6;
    P6P4.pin = 4;
    P6P5.port = Port6;
    P6P5.pin = 5;
    P6P6.port = Port6;
    P6P6.pin = 6;
    P6P7.port = Port6;
    P6P7.pin = 7;

    interruptPinsTail = 0;
}

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

    // Configure internal resistor
    if (resType == Pullup) {
        *(port.REN) |= mask;
        *(port.OUT) |= mask;
    } else if (resType == Pulldown) {
        *(port.REN) |= mask;
        *(port.OUT) &= ~mask;
    } else if (resType == NoResistor) {
        *(port.REN) &= ~mask;
        *(port.OUT) &= ~mask; // Set output to low to save on power
    }
}
void setupInterrupts(pin_t pin, interruptEdge_t edge) {
    port_t port = pin.port;
    uint8_t mask = 1 << pin.pin;

    // Disable interrupts while we modify the pin
    *(port.IE) &= ~mask;
    // Low to high => IES = 0
    // High to low => IES = 1
    if (edge == LowToHigh) {
        *(port.IES) &= ~mask;
    } else {
        *(port.IES) |= mask;
    }
    // Enable interrupts
    *(port.IE) |= mask;
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

char valToChar(uint8_t pinValue) {
    if (pinValue) {
        return '1';
    }
    return '0';
}

void registerPortInterruptHandler(pin_t *pin, interruptEdge_t edge, void (*handler)(uint8_t, uint8_t)) {
    setupInterrupts(*pin, edge);
    pinInterruptEntry_t newEntry;
    newEntry.pin = pin;
    newEntry.handler = handler;
    if (interruptPinsTail >= MAX_PIN_INTERRUPTS) {
        // Cannot add more than max number, so just return early and do nothing
        return;
    }
    interruptPins[interruptPinsTail] = newEntry;
    interruptPinsTail++;
    // Turn on interrupt for this pin in NVIC
    uint8_t nvicBit = interruptNvicBits[pin->port.portNum - 1];
    NVIC->ISER[1] |= (1)<<(nvicBit-32);
}

void intHandler(uint8_t port, uint8_t pin) {
    // Iterate over all the interrupt handlers, until we find one for this port
    int i = 0;
    while (i < interruptPinsTail && i < MAX_PIN_INTERRUPTS) {
        pinInterruptEntry_t entry = interruptPins[i];
        if ((entry.pin->port.portNum == port) && (entry.pin->pin == pin)) {
            entry.handler(port, pin);
        }
        i++;
    }
}

// Priority goes from low to high values
uint8_t getInterruptPin(uint8_t flags) {
    uint8_t p = 0;
    while (p < 8) {
        if (flags & 0x01) {
            return p;
        }
        flags >>= 1;
        p++;
    }
    return p; // Should never get here if flags > 0
}

void PORT1_IRQHandler(void) {
    uint8_t pin = getInterruptPin(P1->IFG);
    char status = P1->IFG;
    intHandler(1, pin);
    // Clear interrupt flag
    P1->IFG &= ~(1 << pin);
    status = P1->IFG;
}
void PORT2_IRQHandler(void) {
    uint8_t pin = getInterruptPin(P2->IFG);
    intHandler(2, pin);
    // Clear interrupt flag
    P2->IFG &= ~(1 << pin);
}
void PORT3_IRQHandler(void) {
    uint8_t pin = getInterruptPin(P3->IFG);
    intHandler(3, pin);
    // Clear interrupt flag
    P3->IFG &= ~(1 << pin);
}
void PORT4_IRQHandler(void) {
    uint8_t pin = getInterruptPin(P4->IFG);
    intHandler(4, pin);
    // Clear interrupt flag
    P4->IFG &= ~(1 << pin);
}
void PORT5_IRQHandler(void) {
    uint8_t pin = getInterruptPin(P5->IFG);
    intHandler(5, pin);
    // Clear interrupt flag
    P5->IFG &= ~(1 << pin);
}
void PORT6_IRQHandler(void) {
    uint8_t pin = getInterruptPin(P6->IFG);
    intHandler(6, pin);
    // Clear interrupt flag
    P6->IFG &= ~(1 << pin);
}
