/*
 * button.h
 *
 *  Created on: Jan 29, 2024
 *      Author: gaulldj
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "iohelper.h"

typedef struct _button {
    pin_t pin;
    uint8_t exists;

} button_t;

typedef enum _buttonState {
    Pressed,
    NotPressed
} buttonState_t;

// Sets up the IO for the provided button
button_t constructButton(pin_t pin, uint8_t exists);
void initButton(button_t btn);
buttonState_t getButtonState(button_t btn);


#endif /* BUTTON_H_ */
