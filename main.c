#include <stddef.h>
#include "msp.h"

#include "keypad.h"
#include "stepperDriver.h"
#include "lcd.h"
#include "button.h"

#define NUM_STEPPERS        4

stepperMotor_t steppers[NUM_STEPPERS];
button_t submitButton;
button_t cancelButton;
itemcode_t itemCode;

void init() {
    initConstants();

    // Initialize the keypad
    initKeypad(P6P6, P6P5, P6P4, P1P7,
               P1P6, P1P5, P3P7, P3P6);

    steppers[0] = constructStepperMotor(0, P2P3, P2P4, P2P5, P2P6);
    steppers[1] = constructStepperMotor(1, P3P5, P3P3, P3P2, P3P0);
    steppers[2] = constructStepperMotor(2, P5P0, P5P1, P5P2, P5P4);
    steppers[3] = constructStepperMotor(3, P5P5, P5P6, P5P7, P6P7);
    initStepperMotor(steppers[0]);
    initStepperMotor(steppers[1]);
    initStepperMotor(steppers[2]);
    initStepperMotor(steppers[3]);

    button_t submitButton = constructButton(P2P7, 1);
    button_t cancelButton = constructButton(P2P7, 0);
    if (submitButton.exists) {
        initButton(submitButton);
    }
    if (cancelButton.exists) {
        initButton(cancelButton);
    }

    initStepperMotorTimer();

    itemCode.letter = NoLetter;
    itemCode.digit = NoNumber;
}

// Functions defined further below
void loop(void);

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	init();

	while (1) {
	    loop();
	}
}

void loop(void) {
    // TEST: Prints out the currently-held buttons
    static keys_t pressedKeys;
    static keys_t lastKeyState;
    pressedKeys = getPressedKeys();
    if (pressedKeys != lastKeyState) {
        printf("0x%04x\n", pressedKeys);
        lastKeyState = pressedKeys;
    }
}
