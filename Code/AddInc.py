import numpy as np
import pandas as pd
import os
import matplotlib.pyplot as plt
import matplotlib.dates as md
import dateutil
data_folder = "E:/RODRIGO/UNI/Master/RedesSensores/Trabajo/Datos/"
gaddress = "E:/RODRIGO/UNI/Master/RedesSensores/Trabajo/Graficas/"
Total = pd.DataFrame()
for f in os.listdir(data_folder+"/csv/"):
    if f.endswith(".csv") and 'Total.csv' != f:
        data = pd.read_csv(os.path.join(data_folder,'csv',f))
        data = data.drop('Unnamed: 0',axis = 1)
        if 'Unnamed: 0' in data.columns:
            data = data.drop('Unnamed: 0',axis = 1)
        if ':0' in data.columns:
            data = data.drop(':0',axis = 1)
        if '46' in data.columns:
            data = data.drop('46',axis = 1)
        data.iloc[:,:3] = data.iloc[:,:3]*100 
        df = pd.DataFrame(columns = ['incaX','incaY','incaZ','incgX','incgY','incgZ'])
        for i in range(data.shape[0]):
            if i == 0:
                df.loc[i,'incaX'] = 0
                df.loc[i,'incaY'] = 0
                df.loc[i,'incaZ'] = 0
                df.loc[i,'incgX'] = 0
                df.loc[i,'incgY'] = 0
                df.loc[i,'incgZ'] = 0
            else:
                df.loc[i,'incaX'] = data.loc[i,'aX']-data.loc[i-1,'aX']
                df.loc[i,'incaY'] = data.loc[i,'aY']-data.loc[i-1,'aY']
                df.loc[i,'incaZ'] = data.loc[i,'aZ']-data.loc[i-1,'aZ']
                df.loc[i,'incgX'] = data.loc[i,'gX']-data.loc[i-1,'gX']
                df.loc[i,'incgY'] = data.loc[i,'gY']-data.loc[i-1,'gY']
                df.loc[i,'incgZ'] = float(data.loc[i,'gZ'])-float(data.loc[i-1,'gZ'])
        data = pd.concat([data,df],axis = 1)
        new_order = [0,1,2,3,4,5,8,9,10,11,12,13,6,7]
        data=data[data.columns[new_order]]
        data.to_csv(os.path.join(data_folder,'csv','inc',"inc"+f))
        print(os.path.join(data_folder,'csv','inc',"inc"+f))
        Total = Total.append(data)

Total = Total.reset_index(drop = True)
Total.to_csv(os.path.join(data_folder,'csv','inc',"IncTotal.csv"))

