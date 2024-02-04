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

#endif /* UTIL_H_ */
