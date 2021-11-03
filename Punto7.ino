#include <WiFi.h>
#include <ESP32Ping.h>
#include <time.h>;
#include <ESP32_FTPClient.h>
#include "Arduino.h"
#include <WiFiClient.h> 
#include <time.h>;
#include "ArduinoJson.h"

const char* ssid = "Rodrigo";
const char* password =  "12345678";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

char ftp_server[] = "155.210.150.77";
char ftp_user[]   = "rsense";
char ftp_pass[]   = "rsense";

ESP32_FTPClient ftp (ftp_server,ftp_user,ftp_pass);

void printLocalTime()
{
  struct tm timeinfo;
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

  ftp.MakeDir("RodrigoPunto7");
  ftp.ChangeWorkDir("/RodrigoPunto7");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  /*
   * JSON CONFIGURATION
   */
  StaticJsonDocument<200> doc;

}

void loop()
{
  static tmElements_t tm; 
  doc["sensor"] = "BME680";
  doc["time"] = tm.;

  // Add an array.
  //
  JsonArray sensor = doc.createNestedArray("sensor");

  for(int i = 0; i<10;i++){
    sensor.add(20+i);
  }
  
  ftp.InitFile("Type A");
  ftp.NewFile("RandomSensorData.json");
  ftp.Write("Hi, I'm a new file");
  ftp.CloseFile(); 
}
