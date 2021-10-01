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
  * Set number of samples in the range.
  * Default is 1
  * Range is 1 - 255
  * This setting splits the range into
  * "samples" pieces, which could look
  * like the sensitivity has been multiplied
  * that many times
  * */
  analogSetSamples(1);

  /*
  * Set the divider for the ADC clock.
  * Default is 1
  * Range is 1 - 255
  * */
  analogSetClockDiv(80);

  /*
  * Set the attenuation for all channels
  * Default is 11db
  * */
  analogSetAttenuation(ADC_11db); //ADC_0db, ADC_2_5db, ADC_6db, ADC_11db

  /*
  * Set the attenuation for particular pin
  * Default is 11db
  * */
  analogSetPinAttenuation(36, ADC_0db); //ADC_0db, ADC_2_5db, ADC_6db, ADC_11db


  /*
  * Attach pin to ADC (will also clear any other analog mode that could be on)
  * */
  adcAttachPin(14);

  /*
  * Start ADC conversion on attached pin's bus
  * */
  adcStart(14);

  /*
  * Check if conversion on the pin's ADC bus is currently running
  * */
  //adcBusy(uint8_t pin);

  /*
  * Get the result of the conversion (will wait if it have not finished)
  * */
  //adcEnd(uint8_t pin);


 
}
 
void loop() {
 
  if (interruptHappened > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptHappened--;
    portEXIT_CRITICAL(&timerMux);

    
    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(analogRead(14));
 
  }
}
