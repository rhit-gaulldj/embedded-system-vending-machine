#include <stddef.h>
#include "msp.h"

#include "keypad.h"
#include "stepperDriver.h"
#include "lcd.h"
#include "button.h"
#include "typedefs.h"
#include "util.h"


#define NUM_ITEMS           4
itemcode_t itemCodes[NUM_ITEMS];
price_t itemPrices[NUM_ITEMS];

#define NUM_STEPPERS        NUM_ITEMS
#define MCLK_FREQUENCY      48000000

#define CANCEL_CLK          32000
#define CANCEL_TIMER        TIMER_A2
#define CANCEL_SECONDS      3 // Show "Canceled" for this many seconds
#define CANCEL_PRESCALER    8
#define CANCEL_PERIOD       CANCEL_CLK/CANCEL_PRESCALER*CANCEL_SECONDS

stepperMotor_t steppers[NUM_STEPPERS];
button_t submitButton;
button_t clearButton;
button_t coinButton;

mode_t currentMode = EnteringCode;
itemcode_t itemCode;
uint8_t coinsInserted = 0;

// Functions defined further below
void loop(void);
void submitButtonHandler(void);
void clearButtonHandler(void);
void coinButtonHandler(void);
void handleKey(keyType_t key);
void updateLcd();
price_t getPrice(itemcode_t code);

void init() {
    initConstants();

    configHFXT();
    configLFXT();

    // Initialize the keypad
    initKeypad(P6P6, P6P5, P6P4, P1P7,
               P1P6, P1P5, P3P7, P3P6);

    // Initialize the stepper motor stuff
    steppers[0] = constructStepperMotor(0, P2P3, P2P4, P2P5, P2P6);
    steppers[1] = constructStepperMotor(1, P3P0, P3P2, P3P3, P3P5);
    steppers[2] = constructStepperMotor(2, P5P0, P5P1, P5P2, P5P4);
    steppers[3] = constructStepperMotor(3, P5P5, P5P6, P5P7, P6P7);
    initStepperMotor(steppers[0]);
    initStepperMotor(steppers[1]);
    initStepperMotor(steppers[2]);
    initStepperMotor(steppers[3]);

    initStepperMotorTimer();

    // Initialize the various outside push buttons
    submitButton = constructButton(P1P4, 1);
    clearButton = constructButton(P4P5, 1);
    coinButton = constructButton(P1P1, 1);
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

    // Configure and initialize the LCD
    configLCD(P4P3, P4P2, P6P0, P6P1, P4P0, P4P1);
    lcd_init();
    lcd_clear();
    updateLcd();

    // Set up internal representation
    itemCode.letter = NoLetter;
    itemCode.digit = NoDigit;
    currentMode = EnteringCode;
    coinsInserted = 0;

    itemCodes[0] = constructItemCode(A, _1);
    itemCodes[1] = constructItemCode(A, _2);
    itemCodes[2] = constructItemCode(B, _1);
    itemCodes[3] = constructItemCode(B, _2);
    itemPrices[0] = 0b00000100; // 000001 00 = $1.00
    itemPrices[1] = 0b00000101; // 000001 01 = $1.25
    itemPrices[2] = 0b00000101; // 000010 11 = $2.75
    itemPrices[3] = 0b00000110; // 000001 10 = $1.50

    // Initialize the timer for the canceled mode
    // Set it to use ACLK
    // XXXX XX01 1100 0000 => ACLK, prescaler of 8, stop mode (for now)
    CANCEL_TIMER->CTL = 0x01C0;
    CANCEL_TIMER->EX0 = 0;
    CANCEL_TIMER->CCR[0] = CANCEL_PERIOD;
    // 0000 0000 0001 0000 => Just enable interrupt, compare mode
    CANCEL_TIMER->CCTL[0] = 0x0010;

    NVIC->ISER[0] |= 1 << TA2_0_IRQn;

    __enable_irq();
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
        updateButton(&submitButton);
    }
    if (clearButton.exists) {
        updateButton(&clearButton);
    }
    if (coinButton.exists) {
        updateButton(&coinButton);
    }

    static keys_t pressedKeys;
    static keys_t lastKeyState;
    if (currentMode == EnteringCode) {
        pressedKeys = getPressedKeys();
        if (pressedKeys != lastKeyState) {
            int i;
            for (i = 0; i < NUM_KEYS; i++) {
                uint16_t currentlyDown = pressedKeys & (1 << i);
                uint16_t prevDown = lastKeyState & (1 << i);
                if (currentlyDown && !prevDown) {
                    // Newly-pressed key
                    keyType_t pressedKey = allKeys[i];
                    handleKey(pressedKey);
                }
            }
        }
        lastKeyState = pressedKeys;
    }
}

void handleKey(keyType_t pressedKey) {
    keyCategory_t cat = getKeyCategory(pressedKey);
    if (cat == KCLetter && itemCode.letter == NoLetter) {
        itemCode.letter = getLetterForKey(pressedKey);
        updateLcd();
    } else if (cat == KCNumber && itemCode.digit == NoDigit && itemCode.letter != NoLetter) {
        itemCode.digit = getDigitForKey(pressedKey);
        updateLcd();
    }
}

void updateLcd(void) {
    int i;
    switch (currentMode) {
        case EnteringCode:
            lcd_SetLineNumber(FirstLine);
            lcd_puts("Enter Code: ");
            char letter = '_', digit = '_';
            if (itemCode.letter != NoLetter) {
                letter = getCharForLetter(itemCode.letter);
                if (itemCode.digit != NoDigit) {
                    digit = getCharForDigit(itemCode.digit);
                }
            }
            lcd_putch(letter);
            lcd_putch(digit);
            lcd_puts("  "); // Double space to fully clear the line
            // Clear the second line
            lcd_SetLineNumber(SecondLine);
            for (i = 0; i < 16; i++) {
                lcd_putch(' ');
            }
            break;
        case EnteringCoins:
            lcd_SetLineNumber(FirstLine);
            lcd_puts("Item: "); // Item: __00000000 (8 spaces)
            lcd_putch(getCharForLetter(itemCode.letter));
            lcd_putch(getCharForDigit(itemCode.digit));
            lcd_puts("        ");

            lcd_SetLineNumber(SecondLine);
            lcd_puts("Insert: "); // Insert: $_.__000
            char buffer[6];
            price_t price = getPrice(itemCode);
            priceToString(price, buffer);
            lcd_puts(buffer);
            lcd_puts("   "); // Fill out the line
            break;
        case ModeCanceled:
            lcd_SetLineNumber(FirstLine);
            lcd_puts("   *Canceled*   ");
            // Clear the second line
            lcd_SetLineNumber(SecondLine);
            for (i = 0; i < 16; i++) {
                lcd_putch(' ');
            }
            break;
    }
    // TODO

    // State 3:
    // Item: __
    // Insert: $_.__

    // (able to cancel at any time until item dispensed)
}

void enterCancelMode(void) {
    itemCode.letter = NoLetter;
    itemCode.digit = NoDigit;
    currentMode = ModeCanceled;
    CANCEL_TIMER->R = 0;
    CANCEL_TIMER->CTL |= 0b0000000000010000; // 0000000000 01 0000 => Up mode (count up to CCR0)
    updateLcd();
}

void submitButtonHandler(void) {
    switch (currentMode) {
        case EnteringCode: {
            // Check if the code is a valid item
            price_t price = getPrice(itemCode);
            // If invalid item, need to tell user and not accept
            if (price == INVALID_PRICE) {
                lcd_SetLineNumber(SecondLine);
                lcd_puts(" *Invalid Code* ");
            } else {
                currentMode = EnteringCoins;
                updateLcd();
            }
            break;
        }
        case ModeCanceled:
            // Entering while in canceled mode will reset
            currentMode = EnteringCode;
            updateLcd();
            break;
    }
}
void clearButtonHandler(void) {
    switch (currentMode) {
        case EnteringCode:
            // Reset the internal item, and update the display
            itemCode.letter = NoLetter;
            itemCode.digit = NoDigit;
            updateLcd();
            break;
        case EnteringCoins:
            enterCancelMode();
            break;
        case ModeCanceled:
            // Canceling while in canceled mode will reset
            currentMode = EnteringCode;
            updateLcd();
            break;
    }
}
void coinButtonHandler(void) {
}

price_t getPrice(itemcode_t code) {
    int i;
    for (i = 0; i < NUM_ITEMS; i++) {
        itemcode_t code = itemCodes[i];
        if (code.letter == itemCode.letter && code.digit == itemCode.digit) {
            return itemPrices[i];
        }
    }
    return INVALID_PRICE;
}

void TA2_0_IRQHandler(void) {
    if (currentMode != EnteringCode) {
        currentMode = EnteringCode;
        updateLcd();
    }

    // Stop the timer
    CANCEL_TIMER->CTL &= 0b1111111111001111; // 1111 1111 11__ 1111
    // Clear interrupt flag
    CANCEL_TIMER->CCTL[0] &= ~0x0001;
}
