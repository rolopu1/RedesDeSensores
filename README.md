# RedesDeSensores

Código de la práctica 1, que incluye interrupciones, ADC y comunicación SPI o I2C. En nuestro caso I2C.

Gy-91 incluye la comunicación i2C con el acelerómetro.

El resto de ficheros se conrresponden con los puntos a entregar del siguiente enunciado:

    1.-   Con una periodicidad de un segundo, lee el valor del ADC y muéstralo por consola. Utiliza la salida de 3V3 de la placa con un potenciómetro para generar la tensión                 analógica. Antes de conectarla al módulo comprueba que no superas 3V3 con un multímetro. 
          https://arduinobasics.blogspot.com/2019/05/sprintf-function.html 
    2.-   Utilizando los timer hardware genera una interrupción cada 10s que lea el ADC y lo muestre por pantalla (https://techtutorialsx.com/2017/10/07/esp32-arduino-timer-                 interrupts/). [ENSEÑAR]
    3.-   Genera una salida pwm a 5kHz proporcional a la lectura del ADC. Comprueba en el osciloscopio tanto la medida analógica como la salida PWM. 
    4.-   Comanda por la UART los periféricos mediante un protocolo de tal modo que si le mandas [ENSEÑAR]:
              ADC: Envíe la lectura del ADC actual
              ADC(x): envíe la lectura del ADC cada x segundos. Si x=0, deja de mandar datos
              PWM(x): comanda el duty cycle del módulo PWM con números del 0 al 9. 
          Utiliza el objeto string y sus métodos 
          https://www.arduino.cc/reference/en/language/variables/data-types/string/ 
          (https://www.arduino.cc/reference/en/language/functions/communication/serial/) 
          https://www.arduino.cc/reference/en/language/functions/communication/stream/ 
    5.-   Conecta un sensor inercial por I2C (o SPI), muestrea la aceleración cada 100 ms y manda los datos cada segundo vía UART (cada vez que envíes activa un LED durante                 200ms).
          Utilizaremos el sensor https://es.aliexpress.com/item/32843038592.html y https://es.aliexpress.com/item/1904029297.html u otro similar [ENSEÑAR]
