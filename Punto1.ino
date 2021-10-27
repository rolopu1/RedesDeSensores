#include <WiFi.h>
#include <ESP32Ping.h>
 
const char* ssid = "Rodrigo";
const char* password =  "12345678";
 
void setup() {
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println(WiFi.localIP());
  
  bool success = Ping.ping("www.google.com", 3);
  
  //int resultao = WiFi.ping(WiFi.localIP());
  
  if(!success){
    Serial.println("Ping failed");
    return;
  }
 
  Serial.println("Ping succesful.");
 
}
 
void loop() { }
