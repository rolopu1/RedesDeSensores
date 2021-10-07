#define pinLED 14
#include <MPU9250_asukiaaa.h>
//#include <esp_freertos_hook.h>
#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#define pinLED 14
#endif

MPU9250_asukiaaa mySensor;
float aX[10], aY[10], aZ[10], aSqrt[10];

void loop1( void * param){
  while(true){
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
    }
  }
}

void loop2 (void *param){
  while(true){
    for(int j =0;j<10; j++){
      Serial.println("accelX: " + String(aX[j]));
      Serial.println("accelY: " + String(aY[j]));
      Serial.println("accelZ: " + String(aZ[j]));
      Serial.println("accelSqrt: " + String(aSqrt[j]));
    }      
    digitalWrite(pinLED,HIGH);
    delay(200);
    digitalWrite(pinLED,LOW);
    delay(800);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(112500);
  delay(500);
  
#ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin(SDA_PIN, SCL_PIN);
  mySensor.setWire(&Wire);
#endif

  pinMode(pinLED,OUTPUT);
  mySensor.beginAccel();
  
  //esp_register_freertos_idle_hook(vApplicationIdleHook);
  xTaskCreate(loop1, "Gyro",10000,NULL,1,NULL);
  xTaskCreate(loop2, "LED" ,10000,NULL,1,NULL);


}

void loop() {
  delay(1000);
}
/*
bool vApplicationIdleHook(void){
  esp_light_sleep_start();
}
*/
