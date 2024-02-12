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

char intDigitToChar(uint8_t digit) {
    switch (digit) {
        case 0: return '0';
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
    }
    return ' ';
}

void moneyToString(money_t price, char *buffer) {
    // Buffer must have length of at least 7
    buffer[0] = '$';
    // Calculate the first digit
    uint8_t wholeNumber = (price & 0b11111100) >> 2;
    if (wholeNumber >= 10) {
        // Need to use two digits
        uint8_t upper = wholeNumber / 10;
        if (upper >= 10) {
            // This is a 3-digit number, so we just set everything to $99.99
            buffer[1] = '9';
            buffer[2] = '9';
            buffer[3] = '.';
            buffer[4] = '9';
            buffer[5] = '9';
            buffer[6] = '\0';
            return;
        }
        uint8_t lower = wholeNumber % 10;
        // Set each digit
        buffer[1] = intDigitToChar(upper);
        buffer[2] = intDigitToChar(lower);
    } else {
        buffer[1] = ' ';
        buffer[2] = intDigitToChar(wholeNumber);
    }

    buffer[3] = '.';
    uint8_t decimal = price & 0b00000011;
    switch (decimal) {
        case 0: buffer[4] = '0'; buffer[5] = '0'; break;
        case 1: buffer[4] = '2'; buffer[5] = '5'; break;
        case 2: buffer[4] = '5'; buffer[5] = '0'; break;
        case 3: buffer[4] = '7'; buffer[5] = '5'; break;
    }
    buffer[6] = '\0';
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
