#include "BLEDevice.h"
#include "ArduinoJson.h"
//#include <ESP32_FTPClient.h>
#include "Arduino.h"
#include <time.h>;

int Lampara = 33;
int Contador = 0;
long Temperature;
long Gravity;
float TempDec;
float GravDec;
static BLEAddress *pServerAddress;
BLEScan* pBLEScan;
BLEClient*  pClient;
bool deviceFound = false;
bool Encendida = false;
bool BotonOff = false;
String knownAddresses[] = { "71:fc:dc:7b:13:83"};

//4c00 02 15 908d8bd3a0a54c04bf15774ec963d028 0100 0000 bf
/*
 * 4C 00 :Apple beacon
02 :type ( constant ,defined by ibeacon spec )
15 :length ( constant defined by ibeacon spec )
908d8bd3a0a54c04bf15774ec963d028 :device UUID
0100 :major - temperature
0000 :minor - specific gravity (x1000)
22 signal power ?? maybe
 * */

unsigned long entry;
static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  Serial.print("Notify callback for characteristic ");
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  Serial.print(" of data length ");
  Serial.println(length);
}
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice Device){
      //Serial.print("BLE Advertised Device found: ");
      //Serial.println(Device.toString().c_str());
      pServerAddress = new BLEAddress(Device.getAddress()); 
      bool known = false;
      bool Master = false;
      String ManufData = Device.toString().c_str();
      String TempHex = ManufData.substring(95,99 ); 

      //long Temperature =  strtol( TempHex.c_str, NULL, 16 );
      
      for (int i = 0; i < (sizeof(knownAddresses) / sizeof(knownAddresses[0])); i++) {
        if (strcmp(pServerAddress->toString().c_str(), knownAddresses[i].c_str()) == 0) 
          known = true;
      }
      if (known) {
        Serial.print("Device found: ");
        Serial.println(Device.getRSSI());
        
        
        Serial.print("Temperature   ");
        Serial.print(TempHex);
        Serial.println(TempHex[11]);
        Serial.print ("    " );
        Temperature = strtol( TempHex.c_str() , NULL, 16 );
        TempDec = -1*TempHex[11]+Temperature/200;
        Serial.println ( TempDec );
         //Serial.println( Temperature );
        //Serial.println(strtol(TempHex.c_str(), NULL, 16));        
        
        if (Device.getRSSI() > -85) {
          deviceFound = true;
        }
        else {
          deviceFound = false;
        }
        Device.getScan()->stop();
        delay(100);
      }
    }
};
void setup() {
  Serial.begin(115200);

  BLEDevice::init("");
  pClient  = BLEDevice::createClient();
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  Serial.println("Done");

  //ftp.OpenConnection();
  
}
void Bluetooth() {
  Serial.println();
  Serial.println("BLE Scan restarted.....");
  deviceFound = false;
  BLEScanResults scanResults = pBLEScan->start(5);

}
void loop() { 
  StaticJsonDocument<200> doc;
  doc["sensor"] = "TemperatureBLE";
  doc["address"] = "5f:b7:fb:a8:ba:c2";

  
  Bluetooth();
  char* output;
  doc["Temp"] = TempDec;

  serializeJson(doc, Serial);
}
