import numpy as np
import pandas as pd
import os
import matplotlib.pyplot as plt
import matplotlib.dates as md

data_folder = "E:/RODRIGO/UNI/Master/RedesSensores/Trabajo/Datos/"
gaddress = "E:/RODRIGO/UNI/Master/RedesSensores/Trabajo/Graficas/"
Longitud = 6
cmap = plt.cm.get_cmap("nipy_spectral")
colors = cmap(np.linspace(0,1,Longitud))
div = 1
data = pd.read_csv(os.path.join(data_folder,'csv','inc',"incTotal.csv"))
#data = data.dropna()
print(data.columns)
lim =50
if 'Unnamed: 0' in data.columns:
    data = data.drop('Unnamed: 0',axis = 1)
for i in range(data.shape[0]):
    incax = data.iloc[i,6]
    incay = data.iloc[i,7]
    incaz = data.iloc[i,8]
    incgX = data.iloc[i,9]
    incgY = data.iloc[i,10]
    if incgX < lim*1.5 and incgX > -lim*1.5 and data.loc[i,"golpe"] == "Mate":
        data.loc[i,"golpe"] = "Reposo"
    if incgX < lim and incgX > -lim and data.loc[i,"golpe"] == "Reves":
        data.loc[i,"golpe"] = "Reposo"
    if incay < lim and incay > -lim and data.loc[i,"golpe"] == "Corte":
        data.loc[i,"golpe"] = "Reposo"
    if incax < lim and incax > -lim and data.loc[i,"golpe"] == "Derecha":
        data.loc[i,"golpe"] = "Reposo"
for i in range(data.shape[0]-1):  
    if data.loc[i+1,"golpe"] != "Reposo":
        data.loc[i,"golpe"] = data.loc[i+1,"golpe"]
rep=data[data.golpe.isin(["Reposo"])]
print(rep.shape)

data.to_csv(os.path.join(data_folder,'csv','inc',"IncTotalReposoExtended.csv"))