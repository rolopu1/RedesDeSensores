from numpy.core.fromnumeric import shape
import serial
import time
import os
import sys
import numpy as np
import matplotlib.pyplot as plt
from collections import deque
import matplotlib.animation as animation

files = os.listdir()
if 'historico' in files:
    print("Sobrescribe en 2 sec")
    time.sleep(2)
else:
    file = open("historico.txt","w")

esp32 = serial.Serial(
            port='COM5',
            baudrate=115200,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            rtscts=False,
            dsrdtr=False
        )
    
while True:
    try:
        line = esp32.readline().decode(sys.getdefaultencoding(), 'ignore')
        print(line[:-2])

        if line[:-2] == "":
            # print("Línea en blanco")
            file.write("\n")
        else:
            file.write(line[:-2] + ';') #+ os.linesep
            
        time.sleep(0.05)

    except KeyboardInterrupt:
        break




esp32.close()