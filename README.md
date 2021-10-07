# RedesDeSensores

Diseño de firmware basado en sistema operativo de tiempo real (RTOS) 

Los objetivos son:
    1.- Comprender el uso y los conceptos asociados a un sistema operativo en tiempo real (RTOS)
    2.- Diseñar un firmware basado en RTOS


Trabajaremos con el sistema FreeRTOS: https://www.freertos.org/Documentation/RTOS_book.html 

    1.-Crea un programa que cree dos tareas, una que parpadee un LED cada 200 ms y otra que envíe un “hola mundo” por la UART cada segundo. https://github.com/uagaviria/ESP32_FreeRtos 
    
    2.-Implementa la tarea 5 del punto anterior con FREETOS (Conecta un sensor inercial por I2C (o SPI), muestrea la aceleración cada 100 ms y manda los datos cada segundo vía UART (cada vez que envíes activa un LED durante 200ms)) [ENSEÑAR]
    
    3.-Investiga el modo de bajo consumo utilizando FREETOS (iddleHook - https://freertos.org/a00016.html). Modifica el programa anterior para dormir mientras el microcontrolador no tiene tareas que hacer (http://www.iotsharing.com/2017/06/arduino-esp32-freertos-how-to-use-task-param-task-priority-task-handle.html) [OPCIONAL] 
