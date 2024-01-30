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
               P1P6, P1P5, P3P7, P3P6);//,
               //NULL, &P2P7);

    steppers[0].index = 0;
    steppers[0].in1 = P2P3;
    steppers[0].in2 = P2P4;
    steppers[0].in3 = P2P5;
    steppers[0].in4 = P2P6;

    steppers[1].index = 1;
    steppers[1].in1 = P3P5;
    steppers[1].in2 = P3P3;
    steppers[1].in3 = P3P2;
    steppers[1].in4 = P3P0;

    steppers[2].index = 2;
    steppers[2].in1 = P5P0;
    steppers[2].in2 = P5P1;
    steppers[2].in3 = P5P2;
    steppers[2].in4 = P5P4;

    steppers[3].index = 3;
    steppers[3].in1 = P5P5;
    steppers[3].in2 = P5P6;
    steppers[3].in3 = P5P7;
    steppers[3].in4 = P6P7;

    button_t submitButton = constructButton(P2P7, 1);
    button_t cancelButton = constructButton(P2P7, 0);
    if (submitButton.exists) {
        initButton(submitButton);
    }
    if (cancelButton.exists) {
        initButton(cancelButton);
    }

    initStepperMotor(steppers[0]);
    initStepperMotor(steppers[1]);
    initStepperMotor(steppers[2]);
    initStepperMotor(steppers[3]);

    initStepperMotorTimer(TIMER_A1, TA1_0_IRQn);

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

	keys_t pressedKeys;
	while (1) {
	    // TEST: Prints out the currently-held buttons
	    pressedKeys = getPressedKeys();
	    printf("0x%04x\n", pressedKeys);
	}
}

void loop(void) {

}
