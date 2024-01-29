/*
 * keypad.h
 *
 *  Created on: Jan 28, 2024
 *      Author: gaulldj
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "iohelper.h"

typedef enum _buttonState {
    Pressed,
    NotPressed
} buttonState_t;

// A, B, C, D, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, *, #
typedef uint16_t keys_t;

#define KEYA_MASK       1<<15;
#define KEYB_MASK       1<<14;
#define KEYC_MASK       1<<13;
#define KEYD_MASK       1<<12;
#define KEY1_MASK       1<<11;
#define KEY2_MASK       1<<10;
#define KEY3_MASK       1<<9;
#define KEY4_MASK       1<<8;
#define KEY5_MASK       1<<7;
#define KEY6_MASK       1<<6;
#define KEY7_MASK       1<<5;
#define KEY8_MASK       1<<4;
#define KEY9_MASK       1<<3;
#define KEY0_MASK       1<<2;
#define KEYSTAR_MASK    1<<1;
#define KEYPOUND_MASK   1<<0;

// Initializes the GPIO pins
void initKeypad(pin_t pin0, pin_t pin1, pin_t pin2, pin_t pin3,
                pin_t pin4, pin_t pin5, pin_t pin6, pin_t pin7,
                pin_t *clearBtnPin, pin_t *submitBtnPin);

// Polls and returns the currently held-down keys
// The bit for the key is high if the key is pressed, low if not
keys_t getPressedKeys(void);

// Returns the status of various buttons
buttonState_t getSubmitButtonStatus(void);
buttonState_t getClearButtonStatus(void);

#endif /* KEYPAD_H_ */
