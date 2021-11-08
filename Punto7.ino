#include <WiFi.h>
#include <ESP32Ping.h>
#include <time.h>;
#include <ESP32_FTPClient.h>
#include "Arduino.h"
#include <WiFiClient.h> 
#include <time.h>;
#include "ArduinoJson.h"
#include <iostream>

const char* ssid = "Rodrigo";
const char* password =  "12345678";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

char ftp_server[] = "155.210.150.77";
char ftp_user[]   = "rsense";
char ftp_pass[]   = "rsense";

ESP32_FTPClient ftp (ftp_server,ftp_user,ftp_pass);
struct tm timeinfo;

void printLocalTime()
{
  
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  
}

void setup() {
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("\nMax Free Heap: ");
  Serial.println(ESP.getMaxAllocHeap());
  Serial.println("");

  ftp.OpenConnection();

  //ftp.MakeDir("RodrigoPunto7");
  //ftp.ChangeWorkDir("/RodrigoPunto7");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();



}

void loop()
{
  /*
   * JSON CONFIGURATION
   */
  printLocalTime();
  StaticJsonDocument<200> doc;
  doc["sensor"] = "BME680";
  
  char timeStringBuff[50]; //50 chars should be enough
  strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M:%S", &timeinfo);
  //print like "const char*"
  Serial.println(timeStringBuff);  
  doc["time"] = timeStringBuff;

  // Add an array.
  //
  JsonArray dat = doc.createNestedArray("data");

  for(int i = 0; i<10;i++){
    dat.add(20+i);
    delay(1000);
  }
  serializeJson(doc, Serial);
  char* output;
  
  //serializeJson(doc, output, 200);
  
  ftp.InitFile("Type A");
  ftp.NewFile("RandomSensorData.json");
  ftp.Write(output);
  ftp.CloseFile(); 
}
