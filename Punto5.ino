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
  //timeStringBuff[48] = '\\';
  //timeStringBuff[49] = 'n';
}

void loop() 
{
  char buf[50];
  bool EnviandoHora = false;
  WiFiClient client = server.available();
  String Reading = ""; 
  if (client) {
    if(client.connected())
    {
      Serial.println("Client Connected");
    }
    int contador = 0;
    char buf[50];
    while(client.connected()){  
      contador = 0;    
      while(client.available()>0){
        // read data from the connected client
        char cutup = client.read();
        Serial.write(cutup);
        buf[contador] = cutup;
        contador++;
        if(buf[0]=='S' and buf[1]=='t' and buf[2]=='a' and buf[3]=='r' and buf[4]=='t'){
          EnviandoHora = true;
        }else if(buf[0]=='S' and buf[1]=='t' and buf[2]=='o' and buf[3]=='p'){
          EnviandoHora = false;
        }
        
      }
      //Send Data to connected client
      while(Serial.available()>0){
        client.write(Serial.read());
        /*
        Reading = Serial.readString();
        if(Reading.equals("Start\n")){
          EnviandoHora = true;
        }else if(Reading.equals("Stop\n")){
          EnviandoHora = false;
        }
        Reading.toCharArray(buf, sizeof(buf));
        //Serial.println(Reading.indexOf("\n"));
        //Serial.println(Reading.substring(0,Reading.indexOf("\n")));
        //Serial.println(sizeof(Reading.substring(0,Reading.indexOf("\n"))));
        client.write(buf,Reading.indexOf("\n")+2);*/
      }
      if(EnviandoHora){
        printLocalTime();  
        
        client.write(timeStringBuff, sizeof(timeStringBuff));
        client.write("\n");
      }
      delay(1000);
    }
    client.stop();
    Serial.println("Client disconnected");    
  }
  
  
}
