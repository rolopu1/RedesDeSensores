# RedesDeSensores

Código de la práctica 1, que incluye interrupciones, ADC y comunicación SPI o I2C. En nuestro caso I2C.

Gy-91 incluye la comunicación i2C con el acelerómetro.

El resto de ficheros se conrresponden con los puntos a entregar del siguiente enunciado:

    1.-   Con una periodicidad de un segundo, lee el valor del ADC y muéstralo por consola. Utiliza la salida de 3V3 de la placa con un potenciómetro para generar la tensión analógica. Antes de conectarla al módulo comprueba que no superas 3V3 con un multímetro. 
          https://arduinobasics.blogspot.com/2019/05/sprintf-function.html 
    2.-   Utilizando los timer hardware genera una interrupción cada 10s que lea el ADC y lo muestre por pantalla (https://techtutorialsx.com/2017/10/07/esp32-arduino-timer interrupts/). [ENSEÑAR]
    3.-   Genera una salida pwm a 5kHz proporcional a la lectura del ADC. Comprueba en el osciloscopio tanto la medida analógica como la salida PWM. 
    4.-   Comanda por la UART los periféricos mediante un protocolo de tal modo que si le mandas [ENSEÑAR]:
              ADC: Envíe la lectura del ADC actual
              ADC(x): envíe la lectura del ADC cada x segundos. Si x=0, deja de mandar datos
              PWM(x): comanda el duty cycle del módulo PWM con números del 0 al 9. 
          Utiliza el objeto string y sus métodos 
          https://www.arduino.cc/reference/en/language/variables/data-types/string/ 
          (https://www.arduino.cc/reference/en/language/functions/communication/serial/) 
          https://www.arduino.cc/reference/en/language/functions/communication/stream/ 
    5.-   Conecta un sensor inercial por I2C (o SPI), muestrea la aceleración cada 100 ms y manda los datos cada segundo vía UART (cada vez que envíes activa un LED durante 200ms).
          Utilizaremos el sensor https://es.aliexpress.com/item/32843038592.html y https://es.aliexpress.com/item/1904029297.html u otro similar [ENSEÑAR]
          
El primer punto a hacer es que un ADC lea el valor analógico cada 10 segundos y lo muestre por pantalla. Para eso utilizaremos la función analogRead() y una interrupción por timer de 10 segundos. Ponemos la resolución y la anchura de bits a 12 con analogReadResolution(12) y analogsetWidth(12).

El siguiente paso es generar una salida PWM proporcional a la lectura hecha por el sensor ADC. Para eso utilizaremos las funciones ledcSetup y LedAttachPin. Una vez configurado el puerto de salida se mete en el loop que tras la lectura, el valor del ADC pasarlo de 10 bits a 8 bits (* 255 / 1023). Para la salida del valor se utilizará la función ledcWrite().

Uniendo todo esto con una parte de control por el usuario, se requiere que dependiendo de 3 posibles comandos del usuario el sistema haga una cosa u otra. Para leer el comando se utiliza Serial.ReadString(), que devuelve un objeto String para compararlo si se ha encontrado un comando u otro. Con las funciones anteriores, simplemente es detectar el nuevo comando y hacer la acción correspondiente.

Por último, el punto 5 se sale un poco de la dirección de los puntos anteriores. El objetivo de este punto es la comunicación de un sensor, en este caso el GY-91 por I2C o SPI. En este caso se ha escogido I2C. 

Para el uso y la comunicación i2C con el acelerómetro se ha utilizado la librería MPU9250_asukiaaa que se puede descargar del propio administrador de librerías del Arduino IDE.

