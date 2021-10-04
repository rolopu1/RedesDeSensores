volatile int interruptHappened;

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
  timerAlarmWrite(timer, 10000000, true);
  timerAlarmEnable(timer);

  // ADC
  
  analogReadResolution(12); //12 bits de resolución es por defecto
  analogSetWidth(12);

  /*
  * Set number of cycles per sample
  * Default is 8 and seems to do well
  * Range is 1 - 255
  * */
  analogSetCycles(8);

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
 
  if (interruptHappened > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptHappened--;
    portEXIT_CRITICAL(&timerMux);

    
    Serial.print("An interrupt as occurred. ADC: ");
    Serial.println(analogRead(14));
 
  }
}
