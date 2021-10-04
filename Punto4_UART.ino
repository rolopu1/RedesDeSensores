const int channel = 0;
const int PWMpin = 16;
const int freq= 5000;
const int resolucion = 8;
volatile int interruptHappened;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptHappened++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}

void setup() {
  // SERIAL
  Serial.begin(115200);
  while (!Serial) {
    ;                                                             // Espera hasta tener una comunicación serie
  }
  Serial.println(Serial.availableForWrite());

  //TIMER
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 10 * 1000000, true);                     //10 segundos
  timerAlarmDisable(timer);                                       //Empieza por defecto desactivada
  
  
  // PWM
  ledcSetup(channel, freq, resolucion);
  ledcAttachPin(PWMpin, channel);
  
  // ADC
  analogReadResolution(12);                                       //12 bits de resolución es por defecto
  analogSetWidth(12);
  adcAttachPin(14);
  //adcStart(14); 
}
 
void loop() {
  int ADC = 0;
  int PWMdutty = 0;                                               //Dutty en 8 bits 0-255
  String Reading = "ADC(10)";                                     //Hace el Size del string directamente de len(ADC(10)) == 8
  int Num = 0;
  if (Serial.available() > 0) {                                   //Sigue con la configuración actual hasta que le llegue un nuevo String
    Reading = Serial.readString();

    if(Reading.equals("ADC\n")){                                  //Si se envía ADC se envía el valor actual
      ADC = analogRead(14);
      Serial.print("ACD actual: ");
      Serial.println(ADC);
      
    }else if(Reading.substring(0,4) == "ADC("){                   //Si se envia ADC(
      if(Reading.indexOf(")") != -1){                             //Comprueba si se ha introducido paréntesis de cierre
        Num = Reading.substring(4,Reading.indexOf(")")).toInt();  //lee desde el paréntesis de inicio hasta el final
        if(Num > 0 and Num <600000){                              //Se pone como timer máximo de 10 minutos. Se puede cambiar en caso de querer
          timerAlarmWrite(timer, Num * 1000000, true);            //Se establece el intervalo de interrupción y el sentido del contador
          timerAlarmEnable(timer);                                //Se habilita la interrupción
          
        }else if(Num == 0){
          Serial.println("El timer del ADC se ha detenido");  
          timerAlarmDisable(timer);                               //En caso de x=0 se desablita la función
        }else{
          Serial.println("El número introducido para el intervalo del ADC no es válido.");
        }
      }else{
        Serial.println("Falta paréntesis de cierre");
      }
      
    }else if(Reading.substring(0,4) == "PWM("){                   //Busca que los 4 primero carácteres sean PWM(
      if(Reading.indexOf(")") != -1){                             //Comprueba que haya paréntesis de cierre
        Num = Reading.substring(4,Reading.indexOf(")")).toInt();  //Pasa el substring a número
        if(Num > 0 and Num <10){                                  //comprueba que esté entre 0-10
          PWMdutty = Num * 255 / 10;                              //Pasa el por diez a 8 bits  
          Serial.println("PWM con dutty fijado a: ");
          Serial.println(PWMdutty);
          ledcWrite(channel, PWMdutty);                           //Activa el PWM
        }else{
          Serial.println("El número introducido para el PWM no es correcto.");
        }
      }else{
        Serial.println("Falta paréntesis de cierre");
      }
    }else{
      Serial.println("Comándo no válido");
    }
  }
  
  if (interruptHappened > 0) {
    portENTER_CRITICAL(&timerMux);
    interruptHappened--;
    portEXIT_CRITICAL(&timerMux);
    Serial.print("An interrupt as occurred. ADC: ");
    Serial.println(analogRead(14));
 
  }
}
