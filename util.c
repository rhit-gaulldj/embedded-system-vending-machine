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

itemcode_t constructItemCode(letter_t letter, digit_t digit) {
    itemcode_t code;
    code.letter = letter;
    code.digit = digit;
    return code;
}

void moneyToString(money_t price, char *buffer) {
    buffer[0] = '$';
    // Calculate the first digit
    uint8_t wholeNumber = (price & 0b11111100) >> 2;
    switch (wholeNumber) {
        case 0: buffer[1] = '0'; break;
        case 1: buffer[1] = '1'; break;
        case 2: buffer[1] = '2'; break;
        case 3: buffer[1] = '3'; break;
        case 4: buffer[1] = '4'; break;
        case 5: buffer[1] = '5'; break;
        case 6: buffer[1] = '6'; break;
        case 7: buffer[1] = '7'; break;
        case 8: buffer[1] = '8'; break;
        case 9: buffer[1] = '9'; break;
    }
    buffer[2] = '.';
    uint8_t decimal = price & 0b00000011;
    switch (decimal) {
        case 0: buffer[3] = '0'; buffer[4] = '0'; break;
        case 1: buffer[3] = '2'; buffer[4] = '5'; break;
        case 2: buffer[3] = '5'; buffer[4] = '0'; break;
        case 3: buffer[3] = '7'; buffer[4] = '5'; break;
    }
    buffer[5] = '\0';
}

money_t coinsToMoney(uint8_t coins) {
    // Divide by 4 to get the dollars, then use the remainder to get the cents part
    uint8_t coinsPerDollar = 4;
    uint8_t dollars = coins / coinsPerDollar;
    uint8_t remainder = coins % coinsPerDollar;
    money_t money = dollars << 2;
    // Now calculate the "cents"
    switch (remainder) {
        case 0:
            money |= 0b00;
            break;
        case 1:
            money |= 0b01;
            break;
        case 2:
            money |= 0b10;
            break;
        case 3:
            money |= 0b11;
            break;
    }
    return money;
}
