#include "iohelper.h"
#include "stepperDriver.h"

// Uses timer A1
#define STEPPER_TIMER           TIMER_A1
#define TIMER_INTERRUPT_BIT     TA1_0_IRQn

void initStepperMotor(stepperMotor_t motor) {
    initOutputPin(motor.in1, Low);
    initOutputPin(motor.in2, Low);
    initOutputPin(motor.in3, Low);
    initOutputPin(motor.in4, Low);
}

void initStepperMotorTimer(void) {
    // TODO timer control & prescaler

    NVIC->ISER[0] |= 1 << TIMER_INTERRUPT_BIT;

    __enable_irq();
}

stepperMotor_t constructStepperMotor(uint8_t index, pin_t in1, pin_t in2, pin_t in3, pin_t in4) {
    stepperMotor_t m;
    m.index = index;
    m.in1 = in1;
    m.in2 = in2;
    m.in3 = in3;
    m.in4 = in4;
    return m;
}
