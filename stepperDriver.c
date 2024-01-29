#include "iohelper.h"
#include "stepperDriver.h"

void initStepperMotor(stepperMotor_t motor) {
    initOutputPin(motor.in1, Low);
    initOutputPin(motor.in2, Low);
    initOutputPin(motor.in3, Low);
    initOutputPin(motor.in4, Low);
}

void initStepperMotorTimer(Timer_A_Type *timer, uint16_t interruptBit) {
    // TODO timer control & prescaler

    NVIC->ISER[0] |= 1 << interruptBit;

    __enable_irq();
}
