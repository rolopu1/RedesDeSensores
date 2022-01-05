import numpy as np
import pandas as pd
import os
import matplotlib.pyplot as plt
import matplotlib.dates as md
import dateutil
data_folder = "E:/RODRIGO/UNI/Master/RedesSensores/Trabajo/Datos/"
gaddress = "E:/RODRIGO/UNI/Master/RedesSensores/Trabajo/Graficas/"
Longitud = 12
cmap = plt.cm.get_cmap("nipy_spectral")
colors = cmap(np.linspace(0,1,Longitud))
div = 3
intervalo = 50
for f in os.listdir(data_folder+"/csv/inc/"):
    data = pd.read_csv(os.path.join(data_folder,'csv','inc',f))
    data = data.drop('Unnamed: 0',axis = 1)
    #data.iloc[:,:3] = data.iloc[:,:3]*100 
    if f == 'incTotal.csv':
        data = data.dropna()
        for i in np.arange(1,data.shape[0]-intervalo,intervalo):
            dfaux = data.iloc[i:i+intervalo,:]
            title = f[7:-4]+f[:7]
            fig, axes = plt.subplots(1, 1,figsize=(16, 8), constrained_layout=True)

            axes.set_title(title)

            for c,temp in zip(dfaux.columns[:Longitud],np.arange(Longitud)):
                axes.plot(dfaux.index,dfaux.iloc[:,temp].values.astype(float),color=colors[temp],label=c)#range(dfaux.shape[0])

            # Turn on the minor TICKS, which are required for the minor GRID
            axes.set_xticks(dfaux.index[::div])
            axes.set_xticklabels(dfaux.index[::div],fontsize=10,rotation=90)

            axes.legend(loc="best",ncol=4, shadow=False, scatterpoints=1,fontsize=8)
            axes.set_ylabel('Resistance',fontsize=18)
            axes.set_ylim(-500,500)
            # Don't allow the axis to be on top of your data
            axes.set_axisbelow(True)       
            # Turn on the minor TICKS, which are required for the minor GRID
            axes.minorticks_on()       
            # Customize the major grid
            axes.grid(which='major', linestyle='-', linewidth='0.5', color='grey')
            # Customize the minor grid
            axes.grid(which='minor', linestyle=':', linewidth='0.5', color='black')
            #plt.show()
            fig.savefig(os.path.join(gaddress,'Graph_'+title+str(i)+'.png'))
            
            print(os.path.join(gaddress,'Graph_'+title+str(i)+'.png'))