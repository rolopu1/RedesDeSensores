 /*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by pcbreflux
*/

 
/*
   Create a BLE server that will send periodic iBeacon frames.
   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create advertising data
   3. Start advertising.
   4. wait
   5. Stop advertising.
   6. deep sleep
   
*/
#include "sys/time.h"

#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEBeacon.h"
#include "esp_sleep.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
#include <MPU9250_asukiaaa.h>

#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif

MPU9250_asukiaaa mySensor;
float aX[10], aY[10], aZ[10], gX[10], gY[10], gZ[10], incaX[10], incaY[10], incaZ[10], incgX[10], incgY[10], incgZ[10];
char orden = 0;
char golpe = 0x00;
boolean confirmRequestPending = true;
String stringOne;
#define GPIO_DEEP_SLEEP_DURATION     10  // sleep x seconds and then wake up
RTC_DATA_ATTR static time_t last;        // remember last boot in RTC Memory
RTC_DATA_ATTR static uint32_t bootcount; // remember number of boots in RTC Memory

#ifdef __cplusplus
extern "C" {
#endif

uint8_t temprature_sens_read();
//uint8_t g_phyFuns;

#ifdef __cplusplus
}
#endif

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
BLEAdvertising *pAdvertising;
struct timeval now;

#define BEACON_UUID           "54c1d7db-57a2-4dc3-bc9a-c77cb4a17875" // UUID 1 128-Bit (may use linux tool uuidgen or random numbers via https://www.uuidgenerator.net/)

void setBeacon() {
  pAdvertising->stop();
  BLEBeacon oBeacon = BLEBeacon();
  oBeacon.setManufacturerId(0x4C00); // fake Apple 0x004C LSB (ENDIAN_CHANGE_U16!)
  oBeacon.setProximityUUID(BLEUUID(BEACON_UUID));
  oBeacon.setMajor(0x0000);
  oBeacon.setMinor(0x00 & golpe);//Movimiento concatenado a 8 ceros para los 16 bits
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();
  
  oAdvertisementData.setFlags(0x04); // BR_EDR_NOT_SUPPORTED 0x04
  Serial.println("00");
  std::string strServiceData = "";
  Serial.println("0");
  strServiceData += (char)26;     // Len
  strServiceData += (char)0xFF;   // Type
  strServiceData += oBeacon.getData(); 
  Serial.println("1");
  oAdvertisementData.addData(strServiceData);
  Serial.println("2");
  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setScanResponseData(oScanResponseData);
  Serial.println("3");
  pAdvertising->stop();
  pAdvertising->start();
  
}

void Muestreo(void *param){
    if (mySensor.accelUpdate() == 0) {
      aX[orden] = mySensor.accelX();
      aY[orden] = mySensor.accelY();
      aZ[orden] = mySensor.accelZ();
    } else {
      Serial.println("Cannod read accel values");
    }
    if (mySensor.gyroUpdate() == 0) {
      gX[orden] = mySensor.gyroX();
      gY[orden] = mySensor.gyroY();
      gZ[orden] = mySensor.gyroZ();
    } else {
      Serial.println("Cannot read gyro values");
    }
    if(orden ==0){
      incaX[0] = aX[0] - aX[9];
      incaY[0] = aY[0] - aY[9];
      incaZ[0] = aZ[0] - aZ[9];
      incgX[0] = gX[0] - gX[9];
      incgY[0] = gY[0] - gY[9];
      incgZ[0] = gZ[0] - gZ[9];      
    }else{
      incaX[orden] = aX[orden] - aX[orden-1];
      incaY[orden] = aY[orden] - aY[orden-1];
      incaZ[orden] = aZ[orden] - aZ[orden-1];
      incgX[orden] = gX[orden] - gX[orden-1];
      incgY[orden] = gY[orden] - gY[orden-1];
      incgZ[orden] = gZ[orden] - gZ[orden-1];  
    }
    if (orden ==9){
      orden =0;
    }else{
      orden++;
    }
}

void Decision(void *param){
  float max_aX = aX[0];
  float min_aY = aY[0];
  float min_aZ = aZ[0];
  float min_gY = gY[0];
  float max_incaX = incaX[0];
  float max_incaY = incaY[0];
  float max_incaZ = incaZ[0];
  float min_incgX = incgX[0];
  float max_incgX = incgX[0];
  float min_incgY = incgY[0];
  float max_incgY = incgY[0];
  float max_incgZ = incgZ[0];
  for(int k=1;k<10;k++){
    if (max_aX < aX[k]){
      max_aX = aX[k];
    }
    if (min_aY > aY[k]){
      min_aY = aY[k];
    }
    if (min_aZ > aZ[k]){
      min_aZ = aZ[k];
    }
    if (min_gY > gY[k]){
      min_gY = gY[k];
    }
    if (max_incaX < incaX[k]){
      max_incaX = incaX[k];
    }
    if (max_incaY < incaY[k]){
      max_incaY = incaY[k];
    }
    if (max_incaZ < incaZ[k]){
      max_incaZ = incaZ[k];
    }
    if (min_incgX > incgX[k]){
      min_incgX = incgX[k];
    }
    if (max_incgX < incgX[k]){
      max_incgX = incgX[k];
    }
    if (min_incgY > incgY[k]){
      min_incgY = incgY[k];
    }
    if (max_incgY < incgY[k]){
      max_incgY = incgY[k];
    }
    if (max_incgZ < incgZ[k]){
      max_incgZ = incgZ[k];
    }
  }
  if(max_aX >2 && min_incgX < -450 && min_incgY < -450 && max_incgZ > 450){
    Serial.println("Mate");
    golpe = 0x01;
  }
  else if(max_aX > 2 && min_gY < -200 && max_incgX > 300 && max_incgZ < 250){
    Serial.println("Derecha");
    golpe = 0x02;
  }
  else if(min_aZ < -1 && min_gY > -200 && max_incaY > 200 && max_incgY > 200){
    Serial.println("Corte");
    golpe = 0x03;
  }
  else if(max_aX < 2 && min_aY < -1.5 && min_aZ < -1.5 && max_incaZ > 300 && max_incaX > 100){
    Serial.println("Reves");
    golpe = 0x04;
  }
  else{
    Serial.println("Reposo");
    golpe = 0x00;
  }
  //setBeacon();
}

void setup() {

    
  Serial.begin(115200);
  gettimeofday(&now, NULL);

  Serial.printf("start ESP32 %d\n",bootcount++);
  
  // Create the BLE Device
  BLEDevice::init("");

  // Create the BLE Server
  // BLEServer *pServer = BLEDevice::createServer(); // <-- no longer required to instantiate BLEServer, less flash and ram usage

  pAdvertising = BLEDevice::getAdvertising();
  
  xTaskCreate(Muestreo, "Muestreo",100,NULL,1,NULL);
  xTaskCreate(Decision, "Decision" ,1000,NULL,1,NULL);
}

void loop() {
  delay(1000);
}
