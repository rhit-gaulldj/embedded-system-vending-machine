/*! \file */
/*!
 * stepperMotor.c
 *
 * Description: Stepper motor ULN2003 driver for MSP432P4111 Launchpad.
 *              Assumes SMCLK configured with 48MHz HFXT as source.
 *              Uses Timer_A3 and P2.7, P2.6, P2.5, P2.4
 *
 *  Created on: 1/22/2024
 *      Author: Daniel Gaull
 */

#include "stepperDriver.h"
#include "msp.h"

/* Global Variables  */
// fill in array with 4-bit binary sequence for wave drive (1-phase full step)
const uint8_t stepperSequence[STEP_SEQ_CNT] = {0b0001, 0b0011, 0b0010, 0b0110,
                                               0b0100, 0b1100, 0b1000, 0b1001};
uint16_t stepPeriod = INIT_PERIOD;
uint8_t currentStep = 0;

uint8_t stepperDir = 0;

void initStepperMotor(void) {
    // set stepper port pins as GPIO outputs
    STEPPER_PORT->SEL0 = (STEPPER_PORT->SEL0) & ~STEPPER_MASK;
    STEPPER_PORT->SEL1 = (STEPPER_PORT->SEL1) & ~STEPPER_MASK;
    STEPPER_PORT->DIR = (STEPPER_PORT->DIR) | STEPPER_MASK;

    // initialize stepper outputs to LOW
    STEPPER_PORT->OUT = (STEPPER_PORT->OUT) & ~STEPPER_MASK;

    /* Configure Timer_A3 and CCR0 */
    // Set period of Timer_A3 in CCR0 register for Up Mode
    TIMER_A3->CCR[0] = stepPeriod;
    // configure CCR0 for Compare mode with interrupt enabled (no output mode - 0)
    // 0000 0000 0001 0000
    TIMER_A3->CCTL[0] = 0x0010;

    // Configure Timer_A3 in Stop Mode, with source SMCLK, prescale 24:1, and
    //  interrupt disabled  -  tick rate will be 4MHz (for SMCLK = 48MHz)
    // configure Timer_A3 (requires setting control AND expansion register)
    // 0000 0010 1000 0000
    TIMER_A3->CTL = 0x0280;
    TIMER_A3->EX0 = 0x0005;

    /* Configure global interrupts and NVIC */
    // Enable TA3CCR0 compare interrupt by setting IRQ bit in NVIC ISER0 register
    // enable interrupt by setting IRQ bit in NVIC ISER0 register
    NVIC->ISER[0] |= 1 << TA3_0_IRQn;

    __enable_irq();                             // Enable global interrupt
}

void enableStepperMotor(void) {
    // configure Timer_A3 in Up Mode (leaving remaining configuration unchanged)
    // OR it with a bitshift of 4
    TIMER_A3->CTL |= 1 << 4;
}

void disableStepperMotor(void) {
    // for future driver use (NOT NEEDED FOR EXERCISE)
    //  Configure Timer_A3 in Stop Mode (leaving remaining configuration unchanged)
    // 0000 0000 00__ 0000
    TIMER_A3->CTL &= ~(0x0030);
}

void stepClockwise(void) {
    currentStep = (currentStep + 1) % STEP_SEQ_CNT;  // increment to next step position
    // update output port for current step pattern
    //  do this as a single assignment to avoid transient changes on driver signals
    STEPPER_PORT->OUT = (STEPPER_PORT->OUT & 0x0F) | (stepperSequence[currentStep] << 4);
}

void stepCounterClockwise(void) {
    currentStep = ((uint8_t)(currentStep - 1)) % STEP_SEQ_CNT;  // decrement to previous step position (counter-clockwise)
    //  update output port for current step pattern
    //  do this as a single assignment to avoid transient changes on driver signals
    STEPPER_PORT->OUT = (STEPPER_PORT->OUT & 0x0F) | (stepperSequence[currentStep] << 4);
}

void setStepperSpeed(uint32_t rpm, uint32_t clock) {
    // modify TA3 CCR0
    float denom = rpm * 2048;
    float num = clock * 60;
    float stepFlt = num / denom;
    stepPeriod = (uint16_t) stepFlt;

//    printf("RPM %d ticks %d\n", rpm, stepPeriod);
    TIMER_A3->CCR[0] = stepPeriod;
//    printf("Received in %d rpm and %d clock\n", rpm, clock);
//    printf("Setting step period to %d\n", stepPeriod);
//    TIMER_A3->R = 0;
}

// Timer A3 CCR0 interrupt service routine
void TA3_0_IRQHandler(void)
{
    /* Not necessary to check which flag is set because only one IRQ
     *  mapped to this interrupt vector     */
    if (stepperDir == 0) {
        stepClockwise();
    } else {
        stepCounterClockwise();
    }
    // clear timer compare flag in TA3CCTL0
    TIMER_A3->CCTL[0] &= ~0x0001;
}

void toggleDirection(void) {
    stepperDir = 1 - stepperDir;
}
