#include <WiFi.h>
#include <ESP32Ping.h>
#include <time.h>;
//#include <WebSocketsServer.h>;

const char* ssid = "Rodrigo";
const char* password =  "12345678";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

const uint16_t port = 455;
WiFiServer server(port);

char timeStringBuff[50];

void printLocalTime()
{
  struct tm timeinfo;
  
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  
  strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);

  //String asString(timeStringBuff);
}
 
void setup() {
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println(WiFi.localIP());
  server.begin();
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  
}

void loop() 
{
  WiFiClient client = server.available();
  String Reading = ""; 
  if (client) {
    if(client.connected())
    {
      Serial.println("Client Connected");
    }
    
    while(client.connected()){      
      while(client.available()>0){
        // read data from the connected client
        Serial.write(client.read()); 
      }
      //Send Data to connected client
      if(Serial.available()>0)
      {
        printLocalTime();  
        client.write(timeStringBuff, sizeof(timeStringBuff));
      }
      delay(995);
    }
    client.stop();
    Serial.println("Client disconnected");    
  }
  
  
}
