#include "WiFi.h"
#include "Hardware.h"

#include <ESP8266WiFi.h>

void WiFiBegin(char* ssid, char* pass)
{
  WiFi.begin(ssid, pass);
  printf("Waiting for Wi-Fi connection ...\n");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(WIFI_DELAY);
    printf(".");
  }
  
  HWSetGreenLed();
  char ipBuffer[MAX_IP_LEN];
  memset(ipBuffer, 0, MAX_IP_LEN);
  IPAddress ip = WiFi.localIP();
  snprintf(ipBuffer, MAX_IP_LEN, "%d.%d.%d.%d",ip[0],ip[1],ip[2],ip[3]);
  printf("\nConnected\nIP Address : %s\n", ipBuffer);
}
