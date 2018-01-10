#ifndef MODULAZY_HARDWARE_H
#define MODULAZY_HARDWARE_H

#include <Arduino.h>

#define SERIAL_BAUD 9600
#define LED_GREEN   12
#define LED_BLUE    13
#define LED_RED     15
#define LED_PWM     64

void HWInit();
void HWLoop();
void HWSetGreenLed();
void HWSetBlueLed();
void HWSetRedLed();

#endif /* MODULAZY_HARDWARE_H */
