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

#include "BluetoothSerial.h"
#ifdef __cplusplus
extern "C" {
#endif

uint8_t temprature_sens_read();
//uint8_t g_phyFuns;

MPU9250_asukiaaa mySensor;
float aX[10], aY[10], aZ[10], gX[10], gY[10], gZ[10], incaX[10], incaY[10], incaZ[10], incgX[10], incgY[10], incgZ[10];
char orden = 0;
char golpe = 0x00;

#ifdef __cplusplus
}
#endif

BLEAdvertising *pAdvertising;


#define BEACON_UUID           "54c1d7db-57a2-4dc3-bc9a-c77cb4a17875" // UUID 1 128-Bit (may use linux tool uuidgen or random numbers via https://www.uuidgenerator.net/)
BluetoothSerial SerialBT;
void Enviar() {
  SerialBT.write(golpe+'0');//Serial.read()
  
}

void Muestreo(){
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

void Decision(){
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
  Enviar();
}

void setup() {

    
  Serial.begin(115200);
  SerialBT.enableSSP();
  // Create the BLE Device
  BLEDevice::init("");

  // Create the BLE Server
  // BLEServer *pServer = BLEDevice::createServer(); // <-- no longer required to instantiate BLEServer, less flash and ram usage

  pAdvertising = BLEDevice::getAdvertising();
  
  //xTaskCreate(Muestreo, "Muestreo",100,NULL,1,NULL);
  //xTaskCreate(Decision, "Decision" ,1000,NULL,1,NULL);
}

void loop() {
  for(int w =0;w<10;w++){
    Muestreo();
    delay(80);
  }
  Decision();
}
