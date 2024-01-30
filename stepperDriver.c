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

stepperMotor_t constructStepperMotor(uint8_t index, pin_t in1, pin_t in2, pin_t in3, pin_t in4) {
    stepperMotor_t m;
    m.index = index;
    m.in1 = in1;
    m.in2 = in2;
    m.in3 = in3;
    m.in4 = in4;
    return m;
}
