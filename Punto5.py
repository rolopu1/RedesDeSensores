import serial
import time
import os
import sys
import numpy as np
import matplotlib.pyplot as plt


files = os.listdir()

esp32 = serial.Serial(
            port='COM5',
            baudrate=115200,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            rtscts=False,
            dsrdtr=False
        )
        

aceleraciones = np.zeros((100,3))
Data = np.zeros((24,6)) #12 index es un minuto (elijo mostrar 2min) y 6 para los 3 promedios y las 3 desviaciones estandard. Aqui se guardan los datos a mostrar

if 'historico' in files:
    print("Sobrescribe en 2 sec")
    time.sleep(2)
else:
    file = open("historico.txt","w")

    columna = 0
    index = 0
time0 = time.time()
timeLinea = time.time()
while True:
    try:
        while esp32.in_waiting > 0:
            line = esp32.readline().decode(sys.getdefaultencoding(), 'ignore')
            print(line[:-2])

            if line[:-2] == "":
                # print("Línea en blanco")
                file.write("\n")
                
                time.sleep(0.05-(timeLinea-time.time()))

            else:
                file.write(line[:-2] + ';') #+ os.linesep
                aceleracion = line[8:-2]
                if aceleracion != "ead accel values":
                    aceleraciones[index, np.int(columna%3)] = float(aceleracion)

                    columna = columna + 1
                    if index == 99:
                        index = 0
                    else:
                        index = index + 1
                else:
                    print("Error al leer la aceleración")
                    # print(line)
            
            
            time1 = time.time()
            print(time1-time0)
            if (time1-time0>5):
                for i in np.arange(23):
                    Data[i,:] = Data[i+1,:]
                Data[23,:3] = np.mean(aceleraciones,axis=0)
                Data[23,3:] = np.std(aceleraciones,axis=0)            
                plt.plot(np.arange(Data.shape[0]),Data)
                time0 = time1
                plt.show()

    except KeyboardInterrupt:
        break

esp32.close()