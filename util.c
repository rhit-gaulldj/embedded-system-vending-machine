#include "util.h"

char getCharForLetter(letter_t letter) {
    switch (letter) {
        case A:
            return 'A';
        case B:
            return 'B';
        case C:
            return 'C';
        case D:
            return 'D';
    }
    return ' ';
}
char getCharForDigit(digit_t digit) {
    switch (digit) {
        case _0:
            return '0';
        case _1:
            return '1';
        case _2:
            return '2';
        case _3:
            return '3';
        case _4:
            return '4';
        case _5:
            return '5';
        case _6:
            return '6';
        case _7:
            return '7';
        case _8:
            return '8';
        case _9:
            return '9';
    }
    return ' ';
}

letter_t getLetterForKey(keyType_t key) {
    switch (key) {
        case KeyA:
            return A;
        case KeyB:
            return B;
        case KeyC:
            return C;
        case KeyD:
            return D;
    }
    return NoLetter;
}

digit_t getDigitForKey(keyType_t key) {
    switch (key) {
        case Key0:
            return _0;
        case Key1:
            return _1;
        case Key2:
            return _2;
        case Key3:
            return _3;
        case Key4:
            return _4;
        case Key5:
            return _5;
        case Key6:
            return _6;
        case Key7:
            return _7;
        case Key8:
            return _8;
        case Key9:
            return _9;
    }
    return NoDigit;
}
