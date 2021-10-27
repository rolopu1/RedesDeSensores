#include <WiFi.h>
#include <ESP32Ping.h>
#include <time.h>;

const char* ssid = "Rodrigo";
const char* password =  "12345678";

const uint16_t port = 455;
WiFiServer server(port);

void setup() {
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println(WiFi.localIP());
  server.begin();  
}

void loop() 
{
  WiFiClient client = server.available();
  
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
      while(Serial.available()>0)
      {
        client.write(Serial.read());
      }
    }
    client.stop();
    Serial.println("Client disconnected");    
  }

}
