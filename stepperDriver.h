/*! \file */
/*!
 * stepperMotor.h
 *
 * Description: Stepper motor ULN2003 driver for MSP432P4111 Launchpad.
 *              Assumes SMCLK configured with 48MHz HFXT as source.
 *              Uses Timer_A3 and P2.7, P2.6, P2.5, P2.4
 *
 *  Created on: 1/22/2024
 *      Author: Daniel Gaull
 */

#ifndef STEPPERMOTOR_H_
#define STEPPERMOTOR_H_

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

#include "msp.h"
#include "typedefs.h"

typedef struct _stepper {
    uint8_t index;
    pin_t in1;
    pin_t in2;
    pin_t in3;
    pin_t in4;

} stepperMotor_t;

// Initializes the GPIO pins for the stepper
void initStepperMotor(stepperMotor_t motor);

// Initializes the specified timer for the stepper
// Configures the control and prescaler values, and enables interrupts
// It is up to the caller to set up their interrupt handler
void initStepperMotorTimer(Timer_A_Type *timer, uint16_t interruptBit);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* STEPPERMOTOR_H_ */
