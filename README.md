# Comunicaciones BLE y Bluetooth 

Los objetivos son:

  -Leer mensajes BLE emitidos por un sensor
  
  -Emitir mensajes de advertising vía BLE
  
  -Establecer una comunicación bidireccional basada en perfil SPP de Bluetooth

Para familiarizarse con la implementación Bluetooth:
  -https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/ 

Las tareas a realizar son:

  1)En el laboratorio habrá un sensor “RuuviTagSensor” que emite mensajes según el formato https://github.com/ruuvi/ruuvi-sensor-protocols/blob/master/dataformat_04.md (formato 2). Descarga la aplicación “nRF Connect” y escanea dispositivos BLE en el entorno. Extrae temperatura, humedad y presión de los mensajes recibidos utilizando Python (https://pypi.org/project/ruuvitag_sensor/).
Si no estás en el laboratorio, puedes utilizar una aplicación como https://play.google.com/store/apps/details?id=net.alea.beaconsimulator&hl=en_US&gl=US para hacer que tu móvil envíe beacons con el formato que definas.
Para adivinar cual de todos los dispositivos descubiertos es el sensor, lo acercaremos al móvil y observar con que potencia se recibe el broadcasting que hacen los dispositivos. De todos los dispositivos escogemos el que mayor potencia de transmisión tenga y apuntaremos esa MAC.

  2)Escanea con el módulo hardware y reporta los valores decodificados por el terminal vía terminal serie en formato JSON [ENSEÑAR].

  3)Haz un advertising con tu módulo siguiendo la identificación iBeacon (https://en.wikipedia.org/wiki/IBeacon) incluyendo el número de grupo dentro del campo UUID [ENSEÑAR].
  
  4)Establece un “chat” Bluetooth con perfil SPP con una APP en el móvil: https://github.com/espressif/arduino-esp32/tree/master/libraries/BluetoothSerial 

Para esto es muy importante entender lo que es el uuid del BLE y como envía y codifica la información el protocolo. El primer ejemplo en el que con una aplicación móvil se configura un iBeacon en el que es muy útil saber esto. A continuación se ve desgranado un uuid, en el que major y minor son el byte que usas para configurar un dato. en este caso era una temperatura, en el caso de mi trabajo se puede configurar en el minor o el mayor el movimiento en estilo “one hot”.

4c00 02 15 908d8bd3a0a54c04bf15774ec963d028 0100 0000 bf
Por ejemplo 
* 4C 00 :Apple beacon
02 :type ( constant ,defined by ibeacon spec )
15 :length ( constant defined by ibeacon spec )
908d8bd3a0a54c04bf15774ec963d028 :device UUID
0100 :major - temperature
0000 :minor - specific gravity (x1000)
22 signal power


La segunda parte consiste en coger los datos obtenidos por iBeacon y almacenarlos en un JSON. El dato obtenido hay que interpretarlo según el estándar propio para la temperatura del BLE. 12 signed int multiplicado por 100. 

La tercera es hacer el iBeacon que se hacía antes con el móvil, o con el sensor propio del laboratorio. Ahora hay que activar el server del BLE, configurar lo que vas a hacer bradcast y activarlo.

Por último, la cuarta parte se trata de establecer una conexión punto a punto entre el esp32 y el móvil con una aplicación que interprete los datos del BLE por el puerto serie. De esta manera establecer un chat entre el móvil y el ESP32
