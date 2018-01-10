#include <Arduino.h>
#include <ESP8266WiFi.h>

#define TIME_SLOT     250
#define BUFFER_LEN    64
#define S_BAUD        9600
#define MAX_DATA_PART 2
#define MAX_IP_LEN    17
#define DELAY_ON_READ TIME_SLOT / 6

bool bumperCheck = false;
unsigned short bumperPhase = 0;
/* Bumper is FF. So it is a 8 bit duration of time.*/
unsigned short bumperDuration = TIME_SLOT * 8;
unsigned short dataPhase = 0;
char dataConv = 0;
char bufferStr[BUFFER_LEN] = {0};
char ssid[BUFFER_LEN] = {0};
char pass[BUFFER_LEN] = {0};
unsigned short bufferIndex = 0;
unsigned short dataSequence = 0;
bool configured = false;

void WiFiBegin(char* ssid, char* pass)
{
  WiFi.begin(ssid, pass);
  printf("Waiting for Wi-Fi connection ...\n");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(TIME_SLOT);
    printf(".");
  }
  
  char ipBuffer[MAX_IP_LEN];
  memset(ipBuffer, 0, MAX_IP_LEN);
  IPAddress ip = WiFi.localIP();
  snprintf(ipBuffer, MAX_IP_LEN, "%d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);
  printf("\nConnected\nIP Address : %s\n", ipBuffer);
}

void VLCWaitForCredentials()
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
      printf("Data done!\n%s, %s\n", ssid, pass);
      /* Connect to WiFi */
      WiFiBegin(ssid, pass);
      configured = true;
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

void setup() {
  Serial.begin(S_BAUD);
  memset(ssid, 0, BUFFER_LEN);
  memset(pass, 0, BUFFER_LEN);
  memset(bufferStr, 0, BUFFER_LEN);
}

void loop() {
  VLCWaitForCredentials();
}
