/*
 * iohelper.h
 *
 *  Created on: Jan 28, 2024
 *      Author: gaulldj
 */

#ifndef IOHELPER_H_
#define IOHELPER_H_

#include "typedefs.h"

typedef enum _pinVal {
    High,
    Low
} pinValue_t;

typedef enum _resistorType {
    NoResistor,
    Pullup,
    Pulldown
} resistorType_t;

void initOutputPin(pin_t pin, pinValue_t initValue);
// TODO: Allow pullup/down resistors
void initInputPin(pin_t pin, resistorType_t resType);

void setOutput(pin_t pin);
void clearOutput(pin_t pin);
void toggleOutput(pin_t pin);
uint8_t readPin(pin_t pin);

#endif /* IOHELPER_H_ */
