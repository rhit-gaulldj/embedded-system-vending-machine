#ifndef LCD_H_
#define LCD_H_

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#include <msp.h>
#include "typedefs.h"
#include "iohelper.h"

#define LCDCMD_ClearDisplay     0x01    //clear display: clear, move cursor home
#define LCDCMD_EMS              0x06    //entry mode set: auto increment cursor after each char sent
#define LCDCMD_DisplaySettings  0x0C    //display ON/OFF control: display on, cursor off, blink off
#define LCDCMD_DisplayOff       0x08    //display ON/OFF control: display off, cursor off, blink off
#define LCDCMD_FunctionSet8bit  0x38    //function set: 8-bit mode, 2 lines, 5x7 dots
#define LCDCMD_FunctionSet4bit  0x28
#define FirstLine 0x00
#define SecondLine 0x40
#define CMD_MODE        0           //0 for command mode
#define DATA_MODE       1           //1 for data mode

void configLCD(pin_t rs_p, pin_t e_p, pin_t db7_p, pin_t db6_p, pin_t db5_p, pin_t db4_p);
extern void Display(unsigned char , unsigned int );

/* delay for indicated nr of ms */
extern void DelayMs(unsigned int);

extern void lcd_write(unsigned char mode, unsigned char CmdChar);

/* write a byte to the LCD in 8 bit mode */
extern void lcd8bits_write(unsigned char mode, unsigned char CmdChar);

extern void lcd4bits_write(unsigned char mode, unsigned char CmdChar);

/* Clear and home the LCD */
extern void lcd_clear(void);

/* write a string of characters to the LCD */
extern void lcd_puts(char * s);

/* Go to the specified position */
extern void lcd_SetLineNumber(unsigned char pos);

/* intialize the LCD - call before anything else */
extern void lcd_init(void);

extern void lcd_putch(char);

/*  Set the cursor position */
#define lcd_cursor(x)   lcd_write(((x)&0x7F)|0x80)

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* LCD_H_ */
