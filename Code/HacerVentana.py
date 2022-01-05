import numpy as np
import pandas as pd
import os
import matplotlib.pyplot as plt
import matplotlib.dates as md
import dateutil

data_folder = "E:/RODRIGO/UNI/Master/RedesSensores/Trabajo/Datos/"
gaddress = "E:/RODRIGO/UNI/Master/RedesSensores/Trabajo/Graficas/"

data = pd.read_csv(os.path.join(data_folder,'csv','inc',"incTotalReposoExtended.csv"))
if 'Unnamed: 0' in data.columns:
    data = data.drop('Unnamed: 0',axis = 1)
data=data[~data.persona.isin(['Rodr'])]

ventana = 1
for i in np.arange(0,data.shape[0]-10,10):
    if data.loc[i,'persona'] == data.loc[i+9,'persona']: # data.loc[i,'golpe'] == data.loc[i+9,'golpe'] and 
        data.loc[i:i+9,'ventana'] = int(ventana)
        ventana = ventana + 1

for i in range(data.shape[0]):
    a = data.loc[i,'golpe']
    if a == 'Reposo':
        data.loc[i,'golpeint'] = int(0)
    elif a == 'Derecha':
        data.loc[i,'golpeint'] = int(1)
    elif a == 'Reves':
        data.loc[i,'golpeint'] = int(2)
    elif a == 'Corte':
        data.loc[i,'golpeint'] = int(3)
    elif a == 'Mate':
        data.loc[i,'golpeint'] = int(4)

data.to_csv(os.path.join(data_folder,'csv','inc',"AllVentanas.csv"))
"""
for p in data.persona.unique():
    d2 = data[data.persona.isin([p])]
    for g in data.golpe.unique():
        d3 = data[data.golpe.isin([g])]
        for i in np.arange(1,d3.shape[0]-10,10):
            d3.loc[i:i+10,'ventana'] = ventana
            ventana = ventana + 1

"""