//ESP 01 Program

#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"

// WiFi Parameters
const char *ssid = "YOUR WIFI SSID HERE";
const char *password = "PASSWORD HERE";

void setup()
{
  // Relay's config
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting...");
    delay(1000);
  }
}
void loop()
{
  // Check WiFi Status
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http; //Object of class HTTPClient
    http.begin("PUT YOUR SEVER ADDRESS HERE");
    int httpCode = http.GET();
    //Check the returning code
    if (httpCode > 0)
    {
      // Get the request response payload
      String command = http.getString();
      Serial.print(command);
      delay(500);
    }
    http.end(); //Close connection
  }   
  delay(500);
}
;
