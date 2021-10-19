# Práctica 3

Comunicación, gestión y representación de datos de sensores con Python.
 
Los objetivos son:
Familiarizarse con Python como lenguaje de programación
Gestionar datos procedentes de comunicaciones serie
Trabajar con estos datos en formato csv y json para almacenarlos en ficheros y redireccionarlos vía internet
Operar con los datos y representarlos gráficamente
Partimos de la anterior práctica en la que se conecta un sensor inercial por I2C (o SPI), muestrea la aceleración y manda los datos vía UART cada 50 ms al PC. 
Las tareas a realizar:
Si no sabes Python, primero necesitas un tutorial como por ejemplo https://uniwebsidad.com/libros/python (aquí tienes prácticamente todo lo necesario para esta práctica). La referencia oficial la tienes en https://docs.python.org/3/ 
Conecta el SMART IMU y comprueba la recepción de datos en el puerto serie con cualquier programa como TERMITE (https://www.compuphase.com/software_termite.htm). 
Programa en Python un programa que acceda al puerto serie y muestre por pantalla los datos en tiempo real (igual que el terminal). Necesitarás instalar el objeto pyserial como administrador. 
Modifica el programa para que almacene los datos en fichero .txt separando cada variable con “;” y con retorno de carro al final de cada muestra para abrirlo desde Excel. [ENSEÑAR]
Previamente guarda un fichero con texto fijo para asegurarte que generas el fichero adecuadamente. 
Modifica el programa para que acumule los datos durante 5 segundos, calcule el promedio y desviación estándar los represente en tiempo real. Necesitarás instalar la librería matplotlib (u otra de representación de gráficas) y configurar spyder para que no genere una gráfica detrás de otra, sino que permita la actualización automática de la gráfica tras cada muestra ploteada. [ENSEÑAR]
