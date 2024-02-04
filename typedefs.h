#define NULL 0

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include "msp.h"

typedef enum _bool {
    False, True
} bool_t;

typedef enum _letter {
    NoLetter, A, B, C, D
} letter_t;
typedef enum _digit {
    NoDigit, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9
} digit_t;
typedef struct _itemcode {
    letter_t letter;
    digit_t digit;
} itemcode_t;

typedef enum _mode {
    EnteringCode,
    EnteringCoins,
    DispensingItem,
    ModeCanceled

} mode_t;

// Lets be smart with our prices
// 8 bits means we can have:
// Upper 6 bits for the whole number
// Lower 2 bits for the fraction (.00, .25, .50, .75)
typedef uint8_t price_t;
#define INVALID_PRICE 0b11111111

#endif
