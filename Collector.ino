//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Richard Li - 2020
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP with authentication)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

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
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
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


void setup()
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println("started");
  SerialBT.enableSSP();
  //SerialBT.onConfirmRequest(BTConfirmRequestCallback);
  //SerialBT.onAuthComplete(BTAuthCompleteCallback);
  SerialBT.begin("ESP32"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  #ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  mySensor.setWire(&Wire);
  #endif

  mySensor.beginAccel();
  mySensor.beginGyro();
}

void loop()
{
    if (mySensor.accelUpdate() == 0) {
      aX = mySensor.accelX();
      aY = mySensor.accelY();
      aZ = mySensor.accelZ();
      //aSqrt = mySensor.accelSqrt();
      //Serial.println("accelX: " + String(aX));
      //Serial.println("accelY: " + String(aY));
      //Serial.println("accelZ: " + String(aZ));
      //Serial.println("accelSqrt: " + String(aSqrt));
    } else {
      Serial.println("Cannod read accel values");
    }
  
    if (mySensor.gyroUpdate() == 0) {
      gX = mySensor.gyroX();
      gY = mySensor.gyroY();
      gZ = mySensor.gyroZ();
      //Serial.println("gyroX: " + String(gX));
      //Serial.println("gyroY: " + String(gY));
      //Serial.println("gyroZ: " + String(gZ));
    } else {
      Serial.println("Cannot read gyro values");
    }
  
    //Serial.println("at " + String(millis()) + "ms");
    //Serial.println(""); // Add an empty line
    delay(80);

      //Serial.println("accelX: " + String(aX));
      stringOne = "aX:"+String(aX)+";";
      Buf[11];
      stringOne.toCharArray(Buf, 11);
      for(int i =0;i<11;i++){
        //Serial.print(i);
        //Serial.print(uint8_t(stringOne[i]));
        SerialBT.write(uint8_t(stringOne[i]));//Serial.read()
      }
      stringOne = "aY:"+String(aY)+";";
      stringOne.toCharArray(Buf, 11);
      for(int i =0;i<11;i++){
        SerialBT.write(uint8_t(stringOne[i]));//Serial.read()
      }
      stringOne = "aZ:"+String(aZ)+";";
      stringOne.toCharArray(Buf, 11);
      for(int i =0;i<11;i++){
        SerialBT.write(uint8_t(stringOne[i]));//Serial.read()
      }
      stringOne = "gX:"+String(gX)+";";
      stringOne.toCharArray(Buf, 11);
      for(int i =0;i<11;i++){
        SerialBT.write(uint8_t(stringOne[i]));//Serial.read()
      }
      stringOne = "gY:"+String(gY)+";";
      stringOne.toCharArray(Buf, 11);
      for(int i =0;i<11;i++){
        SerialBT.write(uint8_t(stringOne[i]));//Serial.read()
      }
      stringOne = "gZ:"+String(gZ)+";";
      stringOne.toCharArray(Buf, 11);
      for(int i =0;i<11;i++){
        SerialBT.write(uint8_t(stringOne[i]));//Serial.read()
      }
}
