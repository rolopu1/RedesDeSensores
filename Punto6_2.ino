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
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
bool resta= false;

// Replace with your network credentials
const char* ssid = "vodafone9320";//"Rodrigo";//""ONO6961";
const char* password = "QWNLTLDTZHFWNH";//"12345678";//"YK6dR5eCsS55";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

struct tm timeinfo;
char SecondsStartBuff[9], timeStringBuff[9];
static tm SecondsStart;

void printLocalTime()
{  
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M:%S", &timeinfo);
  
  return;
}

String enviarHora(){
  Serial.println("EnviarHora()");
  
  if(resta){
    return Relativa();
  }else{
    printLocalTime();
    String out = String(timeStringBuff);
    return out;
  }
  
}



String Relativa(){
  printLocalTime();
  char CadenaA[] = {'0','0',':','0','0',':','0','0','\n'};
  char CadenaP[] = {'0','0',':','0','0',':','0','0','\n'};
  char CadenaEnviada[] = {'0','0',':','0','0',':','0','0','\n'};
  String(timeStringBuff).toCharArray(CadenaA,9);
  String(SecondsStartBuff).toCharArray(CadenaP,9);
  char num; 
  int acarreo = 0;
  int segundosA = (int(CadenaA[6]-'0')*10) + CadenaA[7]-'0';
  int segundosP = (int(CadenaP[6]-'0')*10) + CadenaP[7]-'0';
  int minA = (int(CadenaA[3]-'0')*10) + CadenaA[4]-'0';
  int minP = (int(CadenaP[3]-'0')*10) + CadenaP[4]-'0';
  int horA = (int(CadenaA[0]-'0')*10) + CadenaA[1]-'0';
  int horP = (int(CadenaP[0]-'0')*10) + CadenaP[1]-'0';
  Serial.println(segundosA);
  int segundos, minutos, horas ;

  segundos = segundosA - segundosP;
  minutos = minA - minP;
  horas = horA-horP;
  if(segundos < 0){
    segundos = 60 + segundos;
     minutos = minutos -1;
  }
  if (minutos<0){
    minutos = 60+minutos;
    horas = horas -1;
  }if (horas<0){
    horas = 24 + horas ;
  }
  //Serial.println(horas);
  //Serial.println(minutos);
  //Serial.println(segundos);
  String uni, Sh, Ss, Sm;
  
  if(segundos < 10){
    Ss = '0' + String(segundos);
  }else{
    Ss = String(segundos);
  }
  if(minutos <10){
    Sm = '0' + String(minutos);
  }else{
    Sm = String(minutos);
  }
  if(horas < 10){
    Sh = '0' + String(horas);
  }else{
    Sh = String(horas);
  }
  
  uni = Sh + ':' + Sm + ':' + Ss;
  //Serial.println(uni);
  CadenaEnviada[2] = ':';
  CadenaEnviada[5] = ':';

  //Serial.println(String(CadenaEnviada));
  String out = String(uni);
  
  /*
  int res = int(timeStringBuff)-int(SecondsStartBuff);
  Serial.println(res);
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
  Serial.println(out);
  */
  return out;
}

// Replaces placeholder with LED state value
String processor(const String& var){
  return enviarHora();
}

String processor2(const String& var){
  return enviarHora();
}

void Arestar(){
  resta = true;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  strftime(SecondsStartBuff, sizeof(SecondsStartBuff), "%H:%M:%S", &timeinfo);
  delay(100);
  Serial.println(String(SecondsStartBuff));
  return;
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
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
