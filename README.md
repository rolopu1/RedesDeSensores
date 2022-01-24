# RedesDeSensores
# Práctica 3

Se van a adjuntar en diferentes branches el código de las diferentes prácticas y el trabajo. 
Comunicación, gestión y representación de datos de sensores con Python.

Los objetivos son:
Familiarizarse con Python como lenguaje de programación
Gestionar datos procedentes de comunicaciones serie
Trabajar con estos datos en formato csv y json para almacenarlos en ficheros y redireccionarlos vía internet
Operar con los datos y representarlos gráficamente
Partimos de la anterior práctica en la que se conecta un sensor inercial por I2C (o SPI), muestrea la aceleración y manda los datos vía UART cada 50 ms al PC. 
Las tareas a realizar:

  1.-Si no sabes Python, primero necesitas un tutorial como por ejemplo https://uniwebsidad.com/libros/python (aquí tienes prácticamente todo lo necesario para esta práctica). La referencia oficial la tienes en https://docs.python.org/3/ 
Conecta el SMART IMU y comprueba la recepción de datos en el puerto serie con cualquier programa como TERMITE (https://www.compuphase.com/software_termite.htm). 
Programa en Python un programa que acceda al puerto serie y muestre por pantalla los datos en tiempo real (igual que el terminal). Necesitarás instalar el objeto pyserial como administrador. 

2.-Modifica el programa para que almacene los datos en fichero .txt separando cada variable con “;” y con retorno de carro al final de cada muestra para abrirlo desde Excel. [ENSEÑAR]

3.-Previamente guarda un fichero con texto fijo para asegurarte que generas el fichero adecuadamente. 
Modifica el programa para que acumule los datos durante 5 segundos, calcule el promedio y desviación estándar los represente en tiempo real. Necesitarás instalar la librería matplotlib (u otra de representación de gráficas) y configurar spyder para que no genere una gráfica detrás de otra, sino que permita la actualización automática de la gráfica tras cada muestra ploteada. [ENSEÑAR]

Se ha probado dos maneras diferentes de hacer el punto 5, una que crea una funcion animada usando funcanimation y la otra que simplemente plotea y sobreescribe la anterior actualizando los datos.

Explicación

Primero comprobamos que lo hecho en la práctica anterior se lea bien en el puerto serie con un programa como termite.

Posteriormente, se crea un programa de Python que lea por el puerto serie utilizando la librería serial. ¡OJO! Si ya la tienes instalada pero te da problemas prueba a hacer “pip install” de “pyserial”

El puerto serie no puede estar comunicando con otro programa cuando quieras abrirlo desde python.

En el programa se abre la comunicación y se intenta leer todo el rato hasta que le usuario haga un “KeyboardInterrupt” (ctr + C).

Cada línea que se recibe se comprueba que sea correcta y se almacena en un fichero, separando con “;” cada valor de aceleración recibido, hasta que llegue una nueva muestra 50ms después. Una vez nos llega un salto de línea, solo un “\n” quiere decir que el “batch” de aceleraciones ya se ha terminado, por lo que saltamos de línea en el fichero de texto también.

Una vez hecho esto se da un paso más allá y se quiere representar los datos obtenidos por el puerto serie en tiempo real. Esto tiene dos partes, por un lado, la adquisición contínua de datos y por otro su representación contínua.
	
	-Para la toma de datos se irán almacenando en orden las 3 aceleraciones en un array de (100,3) llenándose así en 5 segundos, ya que los datos llegan cada 50ms. Una vez que han pasado esos 5 segundos, se hace la media y la desviación estándar de las aceleraciones y se añade en un nuevo array que es el que vamos a representar. Este array es de (24,6) ya que hemos decidido mostrar los últimos 2 minutos y son 6 líneas a mostrar

	-Representación gráfica. Se plantean dos maneras de solucionar esto. Por un lado se puede generar una comparación de tiempos dentro del loop de recogida de datos, que una vez se cumpla que hay que actualizar los datos recogidos los actualice, o, se puede crear dos threads independientes. Esta última manera es más compatible con una función orientada a la representación en tiempo real creada para python que es funcAnimation, pero es más compleja de implementar.

Se han hecho las dos opciones hasta cierto punto ya que había problemas al representar varias líneas en la misma gráfica de funcAnimation, así que se decidió por avanzar y presentar la opción de introducir el ploteado de el array de los datos dentro del bucle de recogida. Para esto en spyder hay que ir a Herramientas->Preferencias->->Gráficas y poner la refresca de las gráficas en automático. Esto hace que para que el código siga corriendo no haya que darle a cerrar la gráfica sacada.

En mi caso si utilizo Spyder el ordenador va demasiado lento como para sacar la gráfica correctamente mientras recibe los datos. El funcionamiento correcto se aprecia cuando paro el programa y la gráfica que se carga es la correcta.

He probado también el programa en visual studio code pero no he conseguido configurarlo para que se sobreescriba la imagen que sale por pantalla.
