#include <msp.h>

#include "lcd.h"
#include "sysTickDelays.h"

#define NONHOME_MASK        0xFC

#define LONG_INSTR_DELAY    2000
#define SHORT_INSTR_DELAY   50

pin_t rs, e, db7, db6, db5, db4;
pin_t outputPins[4];

void configLCD(pin_t rs_p, pin_t e_p, pin_t db7_p, pin_t db6_p, pin_t db5_p, pin_t db4_p) {
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
}

void setCommandMode() {
    clearOutput(rs);
}
void setDataMode() {
    setOutput(rs);
}
void setEnableHigh() {
    setOutput(e);
}
void setEnableLow() {
    clearOutput(e);
}

void lcd_init(void)
{
    setCommandMode();

    setEnableLow();

    // https://www.sparkfun.com/datasheets/LCD/HD44780.pdf page 42
    DelayMs(20); // wait 15mSec after power applied,
    lcd_write(CMD_MODE, LCDCMD_FunctionSet4bit); // function set
    DelayMs(20);
    lcd_write(CMD_MODE, LCDCMD_FunctionSet4bit); // function set again
    DelayMs(4);
    // turn display off, then on
    lcd_write(CMD_MODE, LCDCMD_DisplayOff);
    DelayMs(4);
    lcd_write(CMD_MODE, LCDCMD_DisplaySettings); // display ON/OFF control: display on, cursor off, blink off

    DelayMs(4);
    lcd_clear(); // Clear screen
    DelayMs(4);
    lcd_write(CMD_MODE, LCDCMD_EMS); // Set entry Mode
}

//making E line rise and then fall.  This falling edge
//writes data on LCD Panel pin DB7-0 into LCD Panel.
void LCD_STROBE(void) {
    setEnableHigh();
    DelayMs(10); //10
    setEnableLow();
    DelayMs(1);
}

void lcd_write(unsigned char mode, unsigned char CmdChar) {
    lcd4bits_write(mode, CmdChar);
//    lcd8bits_write(mode, CmdChar);
}

/*
 * lcd_write function ---writes a byte to the LCD in 8-bit mode
 * Note that the "mode" argument is set to either CMD_MODE (=0) or DTA_MODE (=1), so that the
 * LCD panel knows whether an instruction byte is being written to it or an ASCII code is being written to it
 * that is to be displayed on the panel.
 */
void lcd8bits_write(unsigned char mode, unsigned char CmdChar) {

    if(mode==CMD_MODE) {
        setCommandMode();
    }
    else {
        setDataMode();
    }
    DelayMs(10);
    // If we pretend we have 8 bits, we only write the upper 4 bits
    char data = CmdChar & 0b11110000;
    int i;
    for (i = 0; i < 4; i++) {
        if (data & (1 << (i+4))) {
            setOutput(outputPins[i]);
        } else {
            clearOutput(outputPins[i]);
        }
    }
//    data = CmdChar & 0b00001111;
//    for (i = 0; i < 4; i++) {
//        if (data & (1 << i)) {
//            setOutput(outputPins[i]);
//        } else {
//            clearOutput(outputPins[i]);
//        }
//    }
    LCD_STROBE(); // Write 8 bits of data on D7-0
}

void lcd4bits_write(unsigned char mode, unsigned char CmdChar) {

    if(mode==CMD_MODE) {
        setCommandMode();
    }
    else {
        setDataMode();
    }
    DelayMs(10);
    char data = CmdChar & 0b11110000;
    int i;
    for (i = 0; i < 4; i++) {
        if (data & (1 << (i+4))) {
            setOutput(outputPins[i]);
        } else {
            clearOutput(outputPins[i]);
        }
    }
    LCD_STROBE();
    data = CmdChar & 0b00001111;
    for (i = 0; i < 4; i++) {
        if (data & (1 << i)) {
            setOutput(outputPins[i]);
        } else {
            clearOutput(outputPins[i]);
        }
    }
    LCD_STROBE();
    DelayMs(40);
}

/* write a string of chars to the LCD */
void lcd_puts(char *string) {
    while (*string != 0) // Last character in a C-language string is alway "0" (ASCII NULL character)
        lcd_write(DATA_MODE, *string++);
}

/*
 *  Clear and home the LCD
 */
void lcd_clear(void) {
    lcd_write(CMD_MODE, LCDCMD_ClearDisplay);
    DelayMs(2);
}

/* write one character to the LCD */
void lcd_putch(char character) {
    lcd_write(DATA_MODE, character);
}

/*
 * Moves cursor to desired position.
 * For 16 x 2 LCD display panel,
 *     the columns of Row 1 are 0x00....0x10
 *     the columns of Row 2 are 0x40....0x50
 */
void lcd_SetLineNumber(unsigned char position) {
    lcd_write(CMD_MODE, 0x80 | position); // The "cursor move" command is indicated by MSB=1 (0x80)
    // followed by the panel position address (0x00- 0x7F)
}

void DelayMs(unsigned int nrms) {
    unsigned int i, j;
    for (j = 0; j < nrms; j++)
        for (i = 0; i < 20; i++);
}
