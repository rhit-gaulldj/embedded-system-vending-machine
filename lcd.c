#include <msp.h>

#include "lcd.h"
#include "sysTickDelays.h"

#define NONHOME_MASK        0xFC

#define LONG_INSTR_DELAY    2000
#define SHORT_INSTR_DELAY   50

pin_t rs, e, db7, db6, db5, db4;
pin_t outputPins[4];

void configLCD(uint32_t clkFreq, pin_t rs_p, pin_t e_p, pin_t db7_p, pin_t db6_p, pin_t db5_p, pin_t db4_p) {
    rs = rs_p;
    e = e_p;
    db7 = db7_p;
    db6 = db6_p;
    db5 = db5_p;
    db4 = db4_p;
    outputPins[0] = db4;
    outputPins[1] = db5;
    outputPins[2] = db6;
    outputPins[3] = db7;

    initOutputPin(rs, Low);
    initOutputPin(e, Low);
    initOutputPin(db7, Low);
    initOutputPin(db6, Low);
    initOutputPin(db5, Low);
    initOutputPin(db4, Low);

    initDelayTimer(clkFreq);
}

void instructionDelay(uint8_t mode, uint8_t instruction) {
    // if instruction is Return Home or Clear Display, use long delay for
    //  instruction execution; otherwise, use short delay
    if ((mode == DATA_MODE) || (instruction & NONHOME_MASK)) {
        delayMicroSec(SHORT_INSTR_DELAY);
    }
    else {
        delayMicroSec(LONG_INSTR_DELAY);
    }
}

void flashData() {
    setOutput(e);
    delayMicroSec(100);
    clearOutput(e);
}
void writeInstruction(uint8_t mode, uint8_t instruction) {
    // set RS for data or control instruction mode
    if (mode == DATA_MODE) {
        setOutput(rs);
    } else {
        clearOutput(rs);
    }

    // Send data to the output pins
    int i;
    for (i = 0; i < 4; i++) {
        if (instruction & (1 << (i+4))) {
            setOutput(outputPins[i]);
        } else {
            clearOutput(outputPins[i]);
        }
    }
    flashData();
    // Set the lower 4 bits
    for (i = 0; i < 4; i++) {
        if (instruction & (1 << i)) {
            setOutput(outputPins[i]);
        } else {
            clearOutput(outputPins[i]);
        }
    }
    flashData();

    // delay to allow instruction execution to complete
    instructionDelay(mode, instruction);
}

void commandInstruction(uint8_t command) {
    writeInstruction(CTRL_MODE, command);
}
void dataInstruction(uint8_t data) {
    writeInstruction(DATA_MODE, data);
}

void initLCD(void) {
    // follows initialization sequence described for 8-bit data mode in
    //  Figure 23 of HD447780 data sheet
//    delayMilliSec(40);
//    commandInstruction(FUNCTION_SET_MASK | N_FLAG_MASK);
//    delayMilliSec(20);
//    commandInstruction(FUNCTION_SET_MASK | N_FLAG_MASK);
//    delayMicroSec(150);
//    commandInstruction(FUNCTION_SET_MASK | N_FLAG_MASK);
//    delayMicroSec(SHORT_INSTR_DELAY);
//    commandInstruction(FUNCTION_SET_MASK | N_FLAG_MASK);
//    delayMicroSec(SHORT_INSTR_DELAY);
//    commandInstruction(DISPLAY_CTRL_MASK);
//    delayMicroSec(SHORT_INSTR_DELAY);
//    commandInstruction(CLEAR_DISPLAY_MASK);
//    delayMicroSec(SHORT_INSTR_DELAY);
//    commandInstruction(ENTRY_MODE_MASK | ID_FLAG_MASK);
//    delayMicroSec(LONG_INSTR_DELAY);
//
//    // after initialization and configuration, turn display ON
//    commandInstruction(DISPLAY_CTRL_MASK | D_FLAG_MASK);

    // Function set twice
    delayMilliSec(40);
    commandInstruction(FUNCTION_SET_MASK | N_FLAG_MASK);
    delayMilliSec(20);
    commandInstruction(FUNCTION_SET_MASK | N_FLAG_MASK);
    delayMilliSec(4);
    // Display off/on
    commandInstruction(DISPLAY_CTRL_MASK);
    delayMilliSec(4);
    commandInstruction(DISPLAY_CTRL_MASK | D_FLAG_MASK);
    delayMilliSec(4);
    clearDisplay();
    delayMilliSec(4);
    commandInstruction(ENTRY_MODE_MASK | ID_FLAG_MASK);
}

void printChar(char character) {
    dataInstruction(character);
}

void printString(char *str) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        printChar(str[i]);
    }
}

void clearDisplay() {
    commandInstruction(CLEAR_DISPLAY_MASK);
}
