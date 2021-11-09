# Comunicaciones BLE y Bluetooth 

Los objetivos son:

Leer mensajes BLE emitidos por un sensor
Emitir mensajes de advertising vía BLE
Establecer una comunicación bidireccional basada en perfil SPP de Bluetooth

Para familiarizarse con la implementación Bluetooth:
https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/ 

Las tareas a realizar son:

En el laboratorio habrá un sensor “RuuviTagSensor” que emite mensajes según el formato https://github.com/ruuvi/ruuvi-sensor-protocols/blob/master/dataformat_04.md (formato 2). Descarga la aplicación “nRF Connect” y escanea dispositivos BLE en el entorno. Extrae temperatura, humedad y presión de los mensajes recibidos utilizando Python (https://pypi.org/project/ruuvitag_sensor/).

Si no estás en el laboratorio, puedes utilizar una aplicación como https://play.google.com/store/apps/details?id=net.alea.beaconsimulator&hl=en_US&gl=US para hacer que tu móvil envíe beacons con el formato que definas.

Para adivinar cual de todos los dispositivos descubiertos es el sensor, lo acercaremos al móvil y observar con que potencia se recibe el broadcasting que hacen los dispositivos. De todos los dispositivos escogemos el que mayor potencia de transmisión tenga y apuntaremos esa MAC.

Escanea con el módulo hardware y reporta los valores decodificados por el terminal vía terminal serie en formato JSON [ENSEÑAR].

Haz un advertising con tu módulo siguiendo la identificación iBeacon (https://en.wikipedia.org/wiki/IBeacon) incluyendo el número de grupo dentro del campo UUID [ENSEÑAR].
Establece un “chat” Bluetooth con perfil SPP con una APP en el móvil: https://github.com/espressif/arduino-esp32/tree/master/libraries/BluetoothSerial 
