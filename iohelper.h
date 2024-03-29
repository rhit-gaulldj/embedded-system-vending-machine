/*
 * iohelper.h
 *
 *  Created on: Jan 28, 2024
 *      Author: gaulldj
 */

#ifndef IOHELPER_H_
#define IOHELPER_H_

#include "msp.h"

typedef struct _port {
    volatile const uint8_t *IN;
    volatile uint8_t *OUT;
    volatile uint8_t *REN;
    volatile uint8_t *SEL0;
    volatile uint8_t *SEL1;
    volatile uint8_t *DIR;
    volatile uint8_t *IE;
    volatile uint8_t *IES;
    uint8_t portNum;

} port_t;

typedef struct _pin {
    port_t port;
    uint8_t pin; // Limited to 0-7

} pin_t;

void initConstants(void);

//#define Port1 P1
//#define Port2 P2
//#define Port3 P3
//#define Port4 P4
//#define Port5 P5
//#define Port6 P6

extern port_t Port1;
extern port_t Port2;
extern port_t Port3;
extern port_t Port4;
extern port_t Port5;
extern port_t Port6;

extern pin_t P1P1;
extern pin_t P1P4;
extern pin_t P1P5;
extern pin_t P1P6;
extern pin_t P1P7;

extern pin_t P2P3;
extern pin_t P2P4;
extern pin_t P2P5;
extern pin_t P2P6;
extern pin_t P2P7;

extern pin_t P3P0;
extern pin_t P3P1;
extern pin_t P3P2;
extern pin_t P3P3;
extern pin_t P3P4;
extern pin_t P3P5;
extern pin_t P3P6;
extern pin_t P3P7;

extern pin_t P4P0;
extern pin_t P4P1;
extern pin_t P4P2;
extern pin_t P4P3;
extern pin_t P4P4;
extern pin_t P4P5;
extern pin_t P4P6;
extern pin_t P4P7;

extern pin_t P5P0;
extern pin_t P5P1;
extern pin_t P5P2;
extern pin_t P5P3;
extern pin_t P5P4;
extern pin_t P5P5;
extern pin_t P5P6;
extern pin_t P5P7;

extern pin_t P6P0;
extern pin_t P6P1;
extern pin_t P6P2;
extern pin_t P6P3;
extern pin_t P6P4;
extern pin_t P6P5;
extern pin_t P6P6;
extern pin_t P6P7;

typedef enum _pinVal {
    High,
    Low
} pinValue_t;

typedef enum _resistorType {
    NoResistor,
    Pullup,
    Pulldown
} resistorType_t;

typedef struct _intPin {
    pin_t *pin;
    void (*handler)(uint8_t, uint8_t);
} pinInterruptEntry_t;

typedef enum _intEdge {
    LowToHigh,
    HighToLow
} interruptEdge_t;

void initOutputPin(pin_t pin, pinValue_t initValue);
void initInputPin(pin_t pin, resistorType_t resType);

void setOutput(pin_t pin);
void clearOutput(pin_t pin);
void toggleOutput(pin_t pin);
uint8_t readPin(pin_t pin);
char valToChar(uint8_t pinValue);

void registerPortInterruptHandler(pin_t *pin, interruptEdge_t edge, void (*handler)(uint8_t, uint8_t));

#endif /* IOHELPER_H_ */
