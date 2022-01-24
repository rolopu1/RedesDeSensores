# RedesDeSensores

Diseño de firmware basado en sistema operativo de tiempo real (RTOS) 

Los objetivos son:
    1.- Comprender el uso y los conceptos asociados a un sistema operativo en tiempo real (RTOS)
    2.- Diseñar un firmware basado en RTOS


Trabajaremos con el sistema FreeRTOS: https://www.freertos.org/Documentation/RTOS_book.html 

    1.-Crea un programa que cree dos tareas, una que parpadee un LED cada 200 ms y otra que envíe un “hola mundo” por la UART cada segundo. https://github.com/uagaviria/ESP32_FreeRtos 
    
    2.-Implementa la tarea 5 del punto anterior con FREETOS (Conecta un sensor inercial por I2C (o SPI), muestrea la aceleración cada 100 ms y manda los datos cada segundo vía UART (cada vez que envíes activa un LED durante 200ms)) [ENSEÑAR]
    
    3.-Investiga el modo de bajo consumo utilizando FREETOS (iddleHook - https://freertos.org/a00016.html). Modifica el programa anterior para dormir mientras el microcontrolador no tiene tareas que hacer (http://www.iotsharing.com/2017/06/arduino-esp32-freertos-how-to-use-task-param-task-priority-task-handle.html) [OPCIONAL] 

Para ello se crean las tareas que se quieren hacer a la vez con xTaskCreate. En esta task se pone tanto como se llama la función, cada cuanto se ha de ejecutar la función o la prioridad que tiene. Esto es algo que nos dará mucha versatilidad para implementar funciones que sean urgentes y no se puedan demorar debido a que se esté haciendo otra tarea. 

Una de las tareas se encargará de leer los datos provenientes del sensor y otra de las tareas gestionará los tiempos del LED. 

Hay un tercer punto opcional de optimización del consumo de energía dentro del sistema freeRTOS. Esto consiste en poner el micro a dormir en los tiempos en los que no esté haciendo ninguna de las tareas.

Esto se ha intentado hacer poniendo en un task llamado vApplicationIdleHook en la que entra cada vez que el micro se queda sin hacer nada. En esta función la idea es programar que se duerma el micro. Hay diversas maneras de despertar el micro una vez se ha dormido, por ejemplo, un timer, un touch pin…

En mi caso no he conseguido hacer que esta función funcione correctamente ya que nunca se duerme correctamente el micro. Esto no sé si puede ser debido a la manera en la que implemento las otras tareas que hace que el micro siempre deba estar ocupado haciendo delays y otras cosas entonces nunca se meta en la tarea vApplicationIdleHook.

El siguiente paso es probar a cambiar la manera a la que llamo a las tareas y dejar que se llamen solas por el parámetro que se le pone cuando se las crea. 
