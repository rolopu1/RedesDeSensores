#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEBeacon.h>

#include "BluetoothSerial.h"

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
BluetoothSerial SerialBT;
boolean confirmRequestPending = true;
String stringOne;
char Buf[11];
void BTConfirmRequestCallback(uint32_t numVal)
{
  confirmRequestPending = true;
  Serial.println(numVal);
}

void BTAuthCompleteCallback(boolean success)
{
  confirmRequestPending = false;
  if (success)
  {
    Serial.println("Pairing success!!");
  }
  else
  {
    Serial.println("Pairing failed, rejected by user!!");
  }
}
BLEAdvertising *pAdvertising;

#define BEACON_UUID           "54c1d7db-57a2-4dc3-bc9a-c77cb4a17875" // UUID 1 128-Bit (may use linux tool uuidgen or random numbers via https://www.uuidgenerator.net/)

void setBeacon() {
  pAdvertising->stop();
  BLEBeacon oBeacon = BLEBeacon();
  oBeacon.setManufacturerId(0x4C00); // fake Apple 0x004C LSB (ENDIAN_CHANGE_U16!)
  oBeacon.setProximityUUID(BLEUUID(BEACON_UUID));
  oBeacon.setMajor(0x0000);
  oBeacon.setMinor(0x00 || golpe);//Movimiento concatenado a 8 ceros para los 16 bits
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

void Muestreo(){ //void *param
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
      Decision();
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
  Serial.print("max_aX: ");
  Serial.println(max_aX);
  Serial.print("min_aY: ");
  Serial.println(min_aY);
  Serial.print("min_aZ: ");
  Serial.println(min_aZ);
  Serial.print("min_gY: ");
  Serial.println(min_gY);
  Serial.print("max_incaX: ");
  Serial.println(max_incaX);
  Serial.print("max_incaY: ");
  Serial.println(max_incaY);
  Serial.print("max_incaZ: ");
  Serial.println(max_incaZ);
  Serial.print("min_incgX: ");
  Serial.println(min_incgX);
  Serial.print("max_incgX: ");
  Serial.println(max_incgX);
  Serial.print("min_incgY: ");
  Serial.println(min_incgY);
  Serial.print("max_incgY: ");
  Serial.println(max_incgY);
  Serial.print("max_incgZ: ");
  Serial.println(max_incgZ);

  if(min_incgX < -450 && min_incgY < -350 && min_aZ < -3){//max_aX >2 &&   && max_incgZ > 450
    Serial.println("Mate");
    stringOne = "Mate";
    golpe = 0x01;
  }
  else if(max_aX > 1.5 && min_gY < -150 && max_incgX > 200 && max_incgZ < 350){//
    Serial.println("Derecha");
    stringOne = "Derecha";
    golpe = 0x02;
  }
  else if(min_aZ < -1 && min_gY < -100 && max_incaX > 1.25 && max_incgY > 150){
    Serial.println("Corte");
    stringOne = "Corte";
    golpe = 0x03;
  }
  else if(max_aX < 2 && min_aZ < 0 && max_incgX > 300){// && max_incaZ > 2 && min_aY < -0.5
    Serial.println("Reves");
    stringOne = "Reves";
    golpe = 0x04;
  }
  else{
    Serial.println("Reposo");
    stringOne = "Reposo";
    golpe = 0x00;
  }
  //delay(1000);
  //setBeacon();
  stringOne.toCharArray(Buf, 11);
  for(int i =0;i<11;i++){
    SerialBT.write(uint8_t(stringOne[i]));//Serial.read()
  }
}

void setup() {

    
  Serial.begin(115200);
  while(!Serial);
  Serial.println("started");
  SerialBT.enableSSP();
  //SerialBT.onConfirmRequest(BTConfirmRequestCallback);
  //SerialBT.onAuthComplete(BTAuthCompleteCallback);
  SerialBT.begin("ESP32"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  mySensor.beginAccel();
  mySensor.beginGyro();

}

void loop() {
  delay(98);
  Muestreo();
  
}
