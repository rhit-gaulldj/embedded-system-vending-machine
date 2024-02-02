#include <stddef.h>
#include "msp.h"

#include "keypad.h"
#include "stepperDriver.h"
#include "lcd.h"
#include "button.h"

#define NUM_STEPPERS        4

stepperMotor_t steppers[NUM_STEPPERS];
button_t submitButton;
button_t clearButton;
button_t coinButton;

itemcode_t itemCode;
uint8_t coinsInserted;

// Functions defined further below
void loop(void);
void submitButtonHandler(void);
void clearButtonHandler(void);
void coinButtonHandler(void);

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

    submitButton = constructButton(P2P7, 1);
    clearButton = constructButton(P4P0, 1);
    coinButton = constructButton(P4P1, 1);
    if (submitButton.exists) {
        initButton(submitButton);
        registerButtonPressEvent(&submitButton, submitButtonHandler);
    }
    if (clearButton.exists) {
        initButton(clearButton);
        registerButtonPressEvent(&clearButton, clearButtonHandler);
    }
    if (coinButton.exists) {
        initButton(coinButton);
        registerButtonPressEvent(&coinButton, coinButtonHandler);
    }

    initStepperMotorTimer();

    itemCode.letter = NoLetter;
    itemCode.digit = NoNumber;
}

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	init();

	while (1) {
	    loop();
	}
}

void loop(void) {
    if (submitButton.exists) {
        updateButton(submitButton);
    }
    if (clearButton.exists) {
        updateButton(clearButton);
    }
    if (coinButton.exists) {
        updateButton(coinButton);
    }

    // TEST: Prints out the currently-held buttons on the keypad
    static keys_t pressedKeys;
    static keys_t lastKeyState;
    pressedKeys = getPressedKeys();
    if (pressedKeys != lastKeyState) {
        printf("0x%04x\n", pressedKeys);
        lastKeyState = pressedKeys;
    }
}

void updateLcd(void) {
    // TODO
    // State 1:
    // Code: __
    // (blank line 2)

    // State 2:
    // Code: __
    // Cost: $_
    .__
    // State 3:
    // Remaining:
    // Cost: $_.__
}

void resetInput(void) {
    itemCode.letter = NoLetter;
    itemCode.number = NoNumber;
    // TODO: LCD clear
}

void submitButtonHandler(void) {
    resetInput();
}
void clearButtonHandler(void) {
    resetInput();
}
void coinButtonHandler(void) {
    coinsInserted++;
}
