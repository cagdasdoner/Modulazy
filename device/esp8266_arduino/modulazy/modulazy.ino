#include "Hardware.h"
#include "VLC.h"

void setup() 
{
  HWInit();
  VLCStartConfiguration();
}

void loop() 
{
  HWLoop();
  VLCLoop();
}
