#include <MPU9250_asukiaaa.h>

#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#define pinLED 14
#endif

MPU9250_asukiaaa mySensor;
float aX[10], aY[10], aZ[10], aSqrt[10];

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("started");

#ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  mySensor.setWire(&Wire);
#endif
  pinMode(pinLED,OUTPUT);
  mySensor.beginAccel();
}

void loop() {
  uint8_t sensorId;
  if (mySensor.readId(&sensorId) == 0) {
    Serial.println("sensorId: " + String(sensorId));
  } else {
    Serial.println("Cannot read sensorId");
  }
  for(int i = 1; i<10 ; i++){
    if (mySensor.accelUpdate() == 0) {
      aX[i] = mySensor.accelX();
      aY[i] = mySensor.accelY();
      aZ[i] = mySensor.accelZ();
      aSqrt[i] = mySensor.accelSqrt();
    } else {
      Serial.println("Cannod read accel values");
    }
    delay(100);
    digitalWrite(pinLED,LOW);
  }
  for(int j =0;j<10; j++){
    Serial.println("accelX: " + String(aX[j]));
    Serial.println("accelY: " + String(aY[j]));
    Serial.println("accelZ: " + String(aZ[j]));
    Serial.println("accelSqrt: " + String(aSqrt[j]));
  }
      
  Serial.println("LED");
  digitalWrite(pinLED,HIGH);
  aX[0] = mySensor.accelX();
  aY[0] = mySensor.accelY();
  aZ[0] = mySensor.accelZ();
  aSqrt[0] = mySensor.accelSqrt();
  delay(100);
}
