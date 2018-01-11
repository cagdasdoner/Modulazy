#include "VLC.h"
#include "WiFi.h"
#include "Hardware.h"

bool bumperCheck = false;
unsigned short bumperIndex = 0;
/* Bumper is FF. So it is a 8 bit duration of time.*/
unsigned short bumperDuration = TIME_SLOT * 8;
unsigned short bitIndex = 0;
char dataAsByte = 0;
short lightThreshold = 1024;
char bufferStr[BUFFER_LEN] = {0};
/* TODO: Adjust to correct SSID and pas length. */
char ssid[BUFFER_LEN] = {0};
char pass[BUFFER_LEN] = {0};
unsigned short dataIndex = 0;
unsigned short dataSequence = 0;
bool configured = false;

void resetVariables()
{
  memset(ssid, 0, BUFFER_LEN);
  memset(pass, 0, BUFFER_LEN);
  memset(bufferStr, 0, BUFFER_LEN);
  bumperCheck = false;
  bumperIndex = 0;
  bitIndex = 0;
  dataAsByte = 0;
  dataIndex = 0;
  dataSequence = 0;
  configured = false;
}

void VLCStartConfiguration()
{
  resetVariables();
}

void VLCStopConfiguration()
{
  configured = true;
}

void VLCLoop()
{
  if(configured)
  {
    return; 
  }
  
  unsigned short sensorValue = HWAnalogRead();
  if(!bumperCheck)
  {
    /* TODO: Adapt into environment light level. So threshold check will be added by sampling. */
    if(sensorValue == lightThreshold)
    {
      bumperIndex++;
      if(bumperIndex == bumperDuration / TIME_SLOT)
      {
        bumperCheck = true;
        bumperIndex = 0;
        HWSetBlueLed();
        Serial.println("Bumper done.");
        /* A little delay and slide to read light data in the middle of SLOT time. */
        delay(DELAY_ON_READ);
      }
      delay(TIME_SLOT);
    }
    else
    {
      bumperIndex = 0;
    }
  }
  else
  {
    /* Current threshold is max value read from sensor. */
    if(sensorValue == lightThreshold)
    {
      /* Bit order is MSB. So reverse check. */
      dataAsByte = dataAsByte + pow(2, 7 - bitIndex);
    }

    if(dataSequence == MAX_DATA_PART && !configured) 
    {
      configured = true;
      HWSetBlueLed();
      printf("Data done!\n%s, %s\n", ssid, pass);
      /* Connect to WiFi */
      WiFiBegin(ssid, pass);
    }
    /* Bit order is 0 to 7 for a byte. Check byte completion. */
    if(bitIndex == 7)
    {
      printf("%x,%c\n",dataAsByte, dataAsByte);
      bufferStr[dataIndex++] = dataAsByte;

      if(dataAsByte == '\0')
      {
        /* First data chunk will be ssid. */
        if (dataSequence == 0)
        {
          strncpy(ssid, bufferStr, dataIndex);
          printf("%s\n", ssid);
        }
        /* Second data chunk will be pass. */
        else if (dataSequence == 1)
        {
          strncpy(pass, bufferStr, dataIndex);
          printf("%s\n", pass);
        }
        dataIndex = 0;
        /* TODO: remove memset, not needed. */
        memset(bufferStr, 0, BUFFER_LEN);
        dataSequence++;
      }
      bitIndex = 0;
      dataAsByte = 0;
    }
    else
    {
      bitIndex++;
    }

    delay(TIME_SLOT);
  }
}
