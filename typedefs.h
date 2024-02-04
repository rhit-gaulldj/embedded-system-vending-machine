#define NULL 0

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

typedef enum _bool {
    True, False
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
    ShowingPrice,
    EnteringCoins,
    DispensingItem,
    ModeCanceled

} mode_t;

#endif
