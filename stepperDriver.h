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
#include "iohelper.h"
#include "typedefs.h"

// Uses Timer A1

typedef struct _stepper {
    uint8_t index;
    pin_t in1;
    pin_t in2;
    pin_t in3;
    pin_t in4;

} stepperMotor_t;

stepperMotor_t constructStepperMotor(uint8_t index, pin_t in1, pin_t in2, pin_t in3, pin_t in4);
// Initializes the GPIO pins for the stepper
void initStepperMotor(stepperMotor_t motor);

// Initializes the specified timer for the stepper
// Configures the control and prescaler values, and enables interrupts
// It is up to the caller to set up their interrupt handler
void initStepperMotorTimer(void);

void rotate(stepperMotor_t *motor, uint8_t revolutions);

#ifdef __cplusplus
}
#endif

#endif /* STEPPERMOTOR_H_ */
