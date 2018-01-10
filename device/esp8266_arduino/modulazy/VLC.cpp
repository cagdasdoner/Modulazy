#include "VLC.h"
#include "WiFi.h"
#include "Hardware.h"

bool bumperCheck = false;
unsigned short bumperPhase = 0;
/* Bumper is FF. So it is a 8 bit duration of time.*/
unsigned short bumperDuration = TIME_SLOT * 8;
unsigned short dataPhase = 0;
char dataConv = 0;
char bufferStr[BUFFER_LEN] = {0};
/* TODO: Adjust to correct SSID and pas length. */
char ssid[BUFFER_LEN] = {0};
char pass[BUFFER_LEN] = {0};
unsigned short bufferIndex = 0;
unsigned short dataSequence = 0;
bool configured = false;

void resetVariables()
{
  memset(ssid, 0, BUFFER_LEN);
  memset(pass, 0, BUFFER_LEN);
  memset(bufferStr, 0, BUFFER_LEN);
  bumperCheck = false;
  bumperPhase = 0;
  dataPhase = 0;
  dataConv = 0;
  bufferIndex = 0;
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
  
  unsigned short sensorValue = analogRead(A0);
  if(!bumperCheck)
  {
    if(sensorValue == 1024)
    {
      bumperPhase++;
      if(bumperPhase == bumperDuration / TIME_SLOT)
      {
        bumperCheck = true;
        bumperPhase = 0;
        HWSetBlueLed();
        Serial.println("Bumper done.");
        /* A little delay and slide to read light data in the middle of SLOT time. */
        delay(DELAY_ON_READ);
      }
      delay(TIME_SLOT);
    }
    else
    {
      bumperPhase = 0;
    }
  }
  else
  {
    if(sensorValue == 1024)
    {
      /* Bit order is MSB. */
      dataConv = dataConv + pow(2, 7 - dataPhase);
    }

    if(dataSequence == MAX_DATA_PART && !configured) 
    {
      configured = true;
      HWSetBlueLed();
      printf("Data done!\n%s, %s\n", ssid, pass);
      /* Connect to WiFi */
      WiFiBegin(ssid, pass);
    }

    if(dataPhase == 7)
    {
      printf("%x,%c\n",dataConv, dataConv);
      bufferStr[bufferIndex++] = dataConv;

      if(dataConv == '\0')
      {
        if (dataSequence == 0)
        {
          strncpy(ssid, bufferStr, bufferIndex);
          printf("%s\n", ssid);
        }
        else if (dataSequence == 1)
        {
          strncpy(pass, bufferStr, bufferIndex);
          printf("%s\n", pass);
        }
        bufferIndex = 0;
        /* TODO: remove memset, not needed. */
        memset(bufferStr, 0, BUFFER_LEN);
        dataSequence++;
      }
           
      dataPhase = 0;
      dataConv = 0;
    }
    else
    {
      dataPhase++;
    }

    delay(TIME_SLOT);
  }
}
