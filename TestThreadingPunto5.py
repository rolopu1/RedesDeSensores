from numpy.core.fromnumeric import shape
import serial
import time
import os
import sys
import numpy as np
import matplotlib.pyplot as plt
from collections import deque
import matplotlib.animation as animation
import threading
import random

files = os.listdir()

aceleraciones = np.zeros((100,3))
Data = np.zeros((24,6)) #12 index es un minuto y 6 para los 3 promedios y las 3 desviaciones estandard. Aqui se guardan los datos a mostrar

if 'historico' in files:
    print("Sobrescribe en 2 sec")
    time.sleep(2)
else:
    file = open("historico.txt","w")

def CogerDatos():
    columna = 0
    index = 0
    while True:
        try:
            while esp32.in_waiting > 0:
                line = esp32.readline().decode(sys.getdefaultencoding(), 'ignore')
                print(line[:-2])

                if line[:-2] == "":
                    # print("Línea en blanco")
                    file.write("\n")
                    for i in np.arange(23):
                        Data[i,:] = Data[i+1,:]
                    Data[23,:3] = np.mean(aceleraciones,axis=0)
                    Data[23,3:] = np.std(aceleraciones,axis=0)

                else:
                    file.write(line[:-2] + ';') #+ os.linesep
                    aceleracion = line[9:-2]
                    if True:#aceleracion.isnumeric():
                        aceleraciones[index%50, columna%3] = random.randint(-30,30)# aceleracion
                        columna = columna + 1
                        index = index + 1
                    else:
                        print("Error al leer la aceleración")
                        # print(line)
                time.sleep(50)

        except KeyboardInterrupt:
            break

def update_line(num, hl, Data):
    hl.set_data(np.array(range(24)), Data[:,0])
    return hl,

esp32 = serial.Serial(
            port='COM7',
            baudrate=115200,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            rtscts=False,
            dsrdtr=False
        )
    

Colector = threading.Thread(target=CogerDatos)
Colector.start()

#Creamos la figura
fig = plt.figure(figsize=(10,8))
hl, = plt.plot(np.arange(24),Data[:,0])
plt.ylim([-30,30])

line_animation = animation.FuncAnimation(fig, update_line, fargs = (hl,Data), interval = 5000, blit=False)

plt.show()

Colector.join()

esp32.close()