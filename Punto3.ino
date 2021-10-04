volatile int interruptHappened;
const int channel = 0;
const int PWMpin = 16;
const int freq= 5000;
const int resolucion = 8;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptHappened++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}
 
void setup() {
 
  Serial.begin(115200);
 
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);

  //PWM
  ledcSetup(channel, freq, resolucion);
  ledcAttachPin(PWMpin, channel);
  // ADC
  
  analogReadResolution(12); //12 bits de resolución es por defecto
  analogSetWidth(12);
  /*
  * Attach pin to ADC (will also clear any other analog mode that could be on)
  * */
  adcAttachPin(14);

  /*
  * Start ADC conversion on attached pin's bus
  * */
  adcStart(14); 
}
 
void loop() {
  int ADC = 0;
  int PWMdutty = 0; //Dutty en 8 bits 0-255
  if (interruptHappened > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptHappened--;
    portEXIT_CRITICAL(&timerMux);

    ADC = analogRead(14);
    Serial.print("An interrupt as occurred. ACD: ");
    Serial.println(ADC);

    PWMdutty = ADC * 255 / 1023; //Para pasar de 10 a 8 bits. Primero multiplico para no generar un numero decimal en el camino.

    Serial.println(PWMdutty);
    ledcWrite(channel, PWMdutty);
 
  }
}
