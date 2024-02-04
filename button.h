/*
 * button.h
 *
 *  Created on: Jan 29, 2024
 *      Author: gaulldj
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "iohelper.h"
#include "typedefs.h"

typedef enum _buttonState {
    Pressed,
    NotPressed
} buttonState_t;

typedef struct _button {
    pin_t pin;
    uint8_t exists;
    buttonState_t status;
    void (*onPress)(void);

} button_t;

// Sets up the IO for the provided button
button_t constructButton(pin_t pin, uint8_t exists);
void initButton(button_t btn);
buttonState_t getButtonState(button_t btn);
//void debounce(void); // Debounce only used internally
void registerButtonPressEvent(button_t *btn, void (*event)(void));
void updateButton(button_t *btn);

#endif /* BUTTON_H_ */
