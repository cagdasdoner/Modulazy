#include "Hardware.h"

void SerialInit()
{
  Serial.begin(SERIAL_BAUD);
}

void GPIOInit()
{

}

void HWSetGreenLed()
{
  analogWrite(LED_GREEN, LED_PWM);
  analogWrite(LED_BLUE, 0);
  analogWrite(LED_RED, 0);
}

void HWSetBlueLed()
{
  analogWrite(LED_BLUE, LED_PWM);
  analogWrite(LED_GREEN, 0);
  analogWrite(LED_RED, 0);
}

void HWSetRedLed()
{
  analogWrite(LED_RED, LED_PWM);
  analogWrite(LED_BLUE, 0);
  analogWrite(LED_GREEN, 0);
}

short HWAnalogRead()
{
  return analogRead(A0);
}

void HWInit()
{
  SerialInit();
  GPIOInit();
  HWSetRedLed();
}

void HWLoop()
{

}

