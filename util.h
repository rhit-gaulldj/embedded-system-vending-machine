/*
 * util.h
 *
 *  Created on: Feb 4, 2024
 *      Author: gaulldj
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "typedefs.h"
#include "keypad.h"

char getCharForLetter(letter_t letter);
char getCharForDigit(digit_t digit);
letter_t getLetterForKey(keyType_t key);
digit_t getDigitForKey(keyType_t key);

itemcode_t constructItemCode(letter_t letter, digit_t digit);

money_t coinsToMoney(uint8_t coins);

// BUFFER MUST BE OF LENGTH AT LEAST 7 ($__.__ + \0)
void moneyToString(money_t money, char *buffer);

#endif /* UTIL_H_ */
