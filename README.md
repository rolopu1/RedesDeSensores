# Practica 4
# Comunicaciones WIFI y stack IP 

Los objetivos son:

Poner en práctica los conocimientos de redes inalámbricas WIFI 

Manejar comunicaciones IP a bajo nivel mediante sockets y entre diversas plataformas: PC, Móvil, CLOUD, Sensor

Manejar protocolos de alto nivel: HTTP, FTP, NTP, MQTT y estándares de interoperabilidad: SENML

Las tareas a realizar son:

  Conéctate a la red wifi del laboratorio o a una creada por el móvil como punto de acceso. Extrae tu IP. Comprueba la conectividad con Google mediante un ping.
  
  Pon en hora el módulo mediante un servidor NTP (Network Time Protocol) 
    https://www.esp32.com/viewtopic.php?t=5978 
    https://lastminuteengineers.com/esp32-ntp-server-date-time-tutorial/ 

Monta un chat una aplicación software PC (http://sockettest.sourceforge.net/) o con una aplicación móvil (simple TCP socket tester). A veces el firewall del ordenador no permite las conexiones externas, y es necesario configurarlo correctamente.

Después sustituye uno de los extremos por el módulo hardware siendo cliente y envía cada segundo tu hora local. 
  Añadir una capa de control de tal modo que cuando se le mande “start” empiece a mandar la hora hasta que se le mande “stop”.  [ENSEÑAR]

  Monta un servidor WEB (https://randomnerdtutorials.com/esp32-web-server-spiffs-spi-flash-file-system/) que muestre la hora y tenga un botón para resetear la hora a las 0:00 [ENSEÑAR]

  Basándote en el estándar SENML (https://tools.ietf.org/html/rfc8428) crea un fichero json (https://arduinojson.org/) que se genere cada 10 segundos, que contenga datos de temperatura inventados, las unidades y la marca temporal. Súbelos a un servidor ftp con un nombre que sea grupoXX_ddmmss.json. (https://platformio.org/lib/show/6543/esp32_ftpclient) Usa el del laboratorio (IP: 155.210.150.77, user: rsense, pass: rsense) o móntate uno con https://filezilla-project.org/download.php?type=server (asegúrate que el firewall permite conexiones entrantes). [ENSEÑAR]
  
  Sube datos a la nube, en concreto al servicio gratuito proporcionado por Adafruit.  
  En primer lugar se debe de crear un usuario en https://io.adafruit.com/, generar una aplicación y obtener un “AIO Key” y crear un feed al que subir datos. Leer info en https://learn.adafruit.com/adafruit-io-basics-feeds 
  Sube datos desde navegador (https://www.apirequest.io/) utilizando POST HTTP según documentación (https://io.adafruit.com/api/docs/?shell#create-data)
Usa librería de Adafruit IO (https://github.com/adafruit/Adafruit_IO_Arduino) para subir datos al feed usando MQTT.
Usa la librería para suscribirte al feed y comprueba que recibes actualización al escribir desde el navegador. [ENSEÑAR]
https://github.com/plapointe6/EspMQTTClient 
