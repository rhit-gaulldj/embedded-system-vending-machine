#include "iohelper.h"
#include "stepperDriver.h"

// Uses timer A1
#define STEPPER_TIMER           TIMER_A1
#define TIMER_INTERRUPT_BIT     TA1_0_IRQn

#define STEPPER_PERIOD          2000
#define STEP_SEQ_CNT            8

const uint8_t stepperSequence[STEP_SEQ_CNT] = {0b0001, 0b0011, 0b0010, 0b0110,
                                               0b0100, 0b1100, 0b1000, 0b1001};
uint8_t currentStep = 0;
stepperMotor_t *currentlyRotating = NULL;
uint16_t stepsRemaining = 0;
void (*onRotationFinish)(void);

void initStepperMotor(stepperMotor_t motor) {
    initOutputPin(motor.in1, Low);
    initOutputPin(motor.in2, Low);
    initOutputPin(motor.in3, Low);
    initOutputPin(motor.in4, Low);
}

void initStepperMotorTimer(void) {
    STEPPER_TIMER->CCR[0] = STEPPER_PERIOD;
    // 0000 0000 0001 0000 => 0x0010
    STEPPER_TIMER->CCTL[0] = 0x0010;
    // XXXX XX10 1001 0000 => 0x0290 (prescaler of 4) - to enable interrupt here, set bit 1
    STEPPER_TIMER->CTL = 0x0290;
    // Prescaler of 6 - Total is 24
    STEPPER_TIMER->EX0 = 0x0005;

    NVIC->ISER[0] |= 1 << TIMER_INTERRUPT_BIT;
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

void rotate(stepperMotor_t *motor, uint8_t revolutions) {
    currentlyRotating = motor;
    stepsRemaining = revolutions * 64 * 64;
}

void setStepperOutput(stepperMotor_t *motor, uint8_t sequence) {
    if (sequence & 0b1000) {
        setOutput(motor->in4);
    } else {
        clearOutput(motor->in4);
    }
    if (sequence & 0b0100) {
        setOutput(motor->in3);
    } else {
        clearOutput(motor->in3);
    }
    if (sequence & 0b0010) {
        setOutput(motor->in2);
    } else {
        clearOutput(motor->in2);
    }
    if (sequence & 0b0001) {
        setOutput(motor->in1);
    } else {
        clearOutput(motor->in1);
    }
}

void step() {
    currentStep = (currentStep + 1) % STEP_SEQ_CNT;
    uint8_t sequence = stepperSequence[currentStep];
    setStepperOutput(currentlyRotating, sequence);
    stepsRemaining--;
    if (stepsRemaining <= 0) {
        onRotationFinish();
        // Clear the outputs for the current motor to save power
        clearOutput(currentlyRotating->in1);
        clearOutput(currentlyRotating->in2);
        clearOutput(currentlyRotating->in3);
        clearOutput(currentlyRotating->in4);
    }
}

void registerRotationFinishHandler(void (*event)(void)) {
    onRotationFinish = event;
}

void TA1_0_IRQHandler(void) {
    if (stepsRemaining > 0) {
        step();
    }

    // Clear interrupt flag
    STEPPER_TIMER->CCTL[0] &= ~0x0001;
}
