# Practica 4
# Comunicaciones WIFI y stack IP 

Los objetivos son:

  1) Poner en práctica los conocimientos de redes inalámbricas WIFI 

  2) Manejar comunicaciones IP a bajo nivel mediante sockets y entre diversas plataformas: PC, Móvil, CLOUD, Sensor

  3) Manejar protocolos de alto nivel: HTTP, FTP, NTP, MQTT y estándares de interoperabilidad: SENML

Las tareas a realizar son:

  1.- Conéctate a la red wifi del laboratorio o a una creada por el móvil como punto de acceso. Extrae tu IP. Comprueba la conectividad con Google mediante un ping.
  
  2.- Pon en hora el módulo mediante un servidor NTP (Network Time Protocol) 
    https://www.esp32.com/viewtopic.php?t=5978 
    https://lastminuteengineers.com/esp32-ntp-server-date-time-tutorial/ 
    
  3.- Monta un chat una aplicación software PC (http://sockettest.sourceforge.net/) o con una aplicación móvil (simple TCP socket tester). A veces el firewall del ordenador no permite las conexiones externas, y es necesario configurarlo correctamente.
  
  4.- Después sustituye uno de los extremos por el módulo hardware siendo cliente y envía cada segundo tu hora local. 
  
  
  5.- Añadir una capa de control de tal modo que cuando se le mande “start” empiece a mandar la hora hasta que se le mande “stop”.  [ENSEÑAR]
  
  6.- Monta un servidor WEB (https://randomnerdtutorials.com/esp32-web-server-spiffs-spi-flash-file-system/) que muestre la hora y tenga un botón para resetear la hora a las 0:00 [ENSEÑAR]
  
  7.- Basándote en el estándar SENML (https://tools.ietf.org/html/rfc8428) crea un fichero json (https://arduinojson.org/) que se genere cada 10 segundos, que contenga datos de temperatura inventados, las unidades y la marca temporal. Súbelos a un servidor ftp con un nombre que sea grupoXX_ddmmss.json. (https://platformio.org/lib/show/6543/esp32_ftpclient) Usa el del laboratorio (IP: 155.210.150.77, user: rsense, pass: rsense) o móntate uno con https://filezilla-project.org/download.php?type=server (asegúrate que el firewall permite conexiones entrantes). [ENSEÑAR]
  
  8.- Sube datos a la nube, en concreto al servicio gratuito proporcionado por Adafruit.  
  
    a) En primer lugar se debe de crear un usuario en https://io.adafruit.com/, generar una aplicación y obtener un “AIO Key” y crear un feed al que subir datos. Leer info en https://learn.adafruit.com/adafruit-io-basics-feeds 
    
    c)Sube datos desde navegador (https://www.apirequest.io/) utilizando POST HTTP según documentación (https://io.adafruit.com/api/docs/?shell#create-data)
    
    d)Usa librería de Adafruit IO (https://github.com/adafruit/Adafruit_IO_Arduino) para subir datos al feed usando MQTT.
    
    e)Usa la librería para suscribirte al feed y comprueba que recibes actualización al escribir desde el navegador. [ENSEÑAR] 
    
    e)https://github.com/plapointe6/EspMQTTClient 
    
Empieza con un primer punto enseñando a conectar el esp32 con ifi. Para ello se hace ping utilizando la librería “ESP32ping.h” y el ejemplo de esa librería cambiándole las credenciales. Haremos que nos muestre por pantalla la IP. (en nuestro caso sale 192.168.43.137)

El segundo punto consiste en consultar la fecha y la hora por ntp y mostrarla por el puerto serie.

El tercer punto consiste en montar un chat. para ello nos descargaremos una aplicación que haga de socket entre nuestro esp32 y el ordenador. Necesitaremos instalar java y configurar en el panel de control de java la lista de excepciones a sitios seguros para que nos deje ejecutar el programa (http://sockettest.sourceforge.net). Una vez abierto el programa. Metemos la IP de nuestro módulo, que ya hemos visto como se obtenía en el apartado 1. Una vez activamos el ESP32, conectamos el socket y así podemos montar un chat con nuestro ordenador.

El punto 4  hace que el ERP32 coja la hora como en el punto dos y la envía periódicamente por el puerto.

El punto 5 es una modificación del punto 4 en el que ahora es el esp quien manda la hora periódicamente al ordenador cuando este le da el comando de “start”. Si el usuario envía “stop” se deja de envíar la hora.

Estos dos puntos se pueden hacer también con el móvil conectándose a Simple TCP Socket Tester

El punto 6 cambia, se trata de montar un servidor ewb que aparezca la hora cogida como hacemos en el punto 2. Se cogen los archivos html y css de https://randomnerdtutorials.com/esp32-web-server-spiffs-spi-flash-file-system/ . Cuando se le da a reset se coge una hora de referencia para luego restarla y hacer un cronómetro. Hay que acordarse que la primera vez después de subir el programa hay que darle a “Herramientas > ESP32 Sketch Data Upload” para que se suban los datos hrml y css.

En el punto 7 se genera un JSON con datos inventados de un sensor. Se almacenan los datos durante 10 segundos, se comprimen en un JSON y se envía por FTP. Se puede hacer con la red del laboratorio, que me gustaría ver como se conecta ahí. También se puede hacer con fileZilla. Aunque no he conseguido subir bien los archivos.

En el punto 8 se crea un feed en adafruit. Primero se prueban a subir datos manualmente para ver que funciona todo correcto en https://www.apirequest.io/. y después la dinámica de crear un JSON y subir los datos al feed la hará el ESP32.

Se tiene ue escribir esta URL
https://io.adafruit.com/api/v2/rolopu1/feeds/punto8/data y estos parámetros


command:
curl -X POST -H 'Content-Type: application/json' -H 'X-AIO-Key: aio_FOtG46jZBcp6wi7FcsZAui3lkXeL' -d '{"value": 50}' -v -i 'https://io.adafruit.com/api/v2/rolopu1/feeds/punto8/data'
