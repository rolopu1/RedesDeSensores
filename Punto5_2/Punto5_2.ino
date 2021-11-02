/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "time.h"
#include "Time.h"

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;
bool resta= false;

// Replace with your network credentials
const char* ssid = "ONO6961";
const char* password = "YK6dR5eCsS55";

// Set LED GPIO
const int ledPin = 14;
// Stores LED state
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

struct tm timeinfo;
char SecondsStartBuff[50], timeStringBuff[50];
static tm SecondsStart;

void printLocalTime()
{  
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
  
  return;
}

String enviarHora(){
  if(resta){
    return Relativa();
    resta = false;
  }else{
    printLocalTime();
    String out = String(timeStringBuff);
    return out;
    
  }
}

String Relativa(){
  int res = int(timeStringBuff)-int(SecondsStartBuff);
  int minutos = res/60;
  int horas = minutos/60;
  minutos = minutos - horas*60;
  int segundos = segundos - horas *3600 - minutos *60;
  String seg = String(segundos);
  if(segundos <10){
    seg = "0"+seg;
  }
  String mini = String(minutos);
  if(minutos <10){
    mini = "0"+mini;
  }
  String h = String(horas);
  if(horas <10){
    h = "0"+h;
  }
  String out = h+":"+mini+":"+seg;
  
  return out;
}

// Replaces placeholder with LED state value
String processor(const String& var){
  return enviarHora();
}

void Arestar(){
  resta = true;
  strftime(SecondsStartBuff, sizeof(SecondsStartBuff), "%A, %B %d %Y %H:%M:%S", &SecondsStart);
  return;
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/GETT", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  server.on("/RESET", HTTP_GET, [](AsyncWebServerRequest *request){
    Arestar();
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Start server
  server.begin();
}
 
void loop(){
  //server.sendContent(SPIFFS, "/index.html", String(), false, processor);
  //delay(1000);
}
