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

// TODO: Remove
#define LCD_DB_PORT         P4
#define LCD_RS_PORT         P2
#define LCD_EN_PORT         P2
#define LCD_RS_MASK         BIT7
#define LCD_EN_MASK         BIT6

#define CTRL_MODE           0
#define DATA_MODE           1
#define LINE1_OFFSET        0x0
#define LINE2_OFFSET        0x40

/* Instruction masks */
#define CLEAR_DISPLAY_MASK  0x01
#define RETURN_HOME_MASK    0x02
#define ENTRY_MODE_MASK     0x04
#define DISPLAY_CTRL_MASK   0x08
#define CURSOR_SHIFT_MASK   0x10
#define FUNCTION_SET_MASK   0x20
#define SET_CGRAM_MASK      0x40
#define SET_CURSOR_MASK     0x80

/* Field masks for instructions:
 * DL   = 1: 8 bits, DL = 0: 4 bits
 * N    = 1: 2 lines, N = 0: 1 line
 * S/C  = 1: Display shift
 * S/C  = 0: Cursor move
 * F    = 1: 5 x 10 dots, F = 0: 5 x 8 dots
 * R/L  = 1: Shift to the right
 * R/L  = 0: Shift to the left
 * D    = 1: Display On, D = 0: Display Off
 * C    = 1: Cursor On, D = 0: Cursor Off
 * I/D  = 1: Increment
 * I/D  = 0: Decrement
 * B    = 1: Cursor blink On, D = 0: Cursor blink Off
 * S    = 1: Accompanies display shift
 * BF   = 1: Internally operating
 * BF   = 0: Instructions acceptable
 */
#define DL_FLAG_MASK        0x10
#define N_FLAG_MASK         0x08
#define SC_FLAG_MASK        0x08
#define F_FLAG_MASK         0x04
#define RL_FLAG_MASK        0x04
#define D_FLAG_MASK         0x04
#define C_FLAG_MASK         0x02
#define ID_FLAG_MASK        0x02
#define B_FLAG_MASK         0x01
#define S_FLAG_MASK         0x01

void configLCD(uint32_t clkFreq, pin_t rs_p, pin_t e_p, pin_t db7_p, pin_t db6_p, pin_t db5_p, pin_t db4_p);
void initLCD(void);
void printChar(char character);
void printString(char *str);
void clearDisplay(void);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* LCD_H_ */
