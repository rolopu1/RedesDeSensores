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
data = pd.read_csv(os.path.join(data_folder,'csv',"Total.csv"))
data = data.dropna()
print(data.columns)
lim = 25
for i in range(data.shape[0]):
    ax = data.iloc[i,0]
    ay = data.iloc[i,1]
    az = data.iloc[i,2]
    gx = float(data.iloc[i,3])
    gy = float(data.iloc[i,4])
    gz = float(data.iloc[i,5])
    if gx < lim and gx > -lim and gy < lim and gy > -lim and gz < lim and gz > -lim:
        data.loc[i,"golpe"] = "reposo"
    
rep=data[data.golpe.isin(["reposo"])]
print(rep.shape)

data.to_csv(os.path.join(data_folder,'csv',"IncTotalReposo.csv"))