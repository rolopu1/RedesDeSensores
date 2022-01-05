import pandas as pd
import os

def AddRaw(df,data):
    col = data[:2]
    valor = data [3:]
    df = df.append({col: valor}, ignore_index=True)
    return df
    
data_folder = "E:/RODRIGO/UNI/Master/RedesSensores/Trabajo/Datos/"
gaddress = "E:/RODRIGO/UNI/Master/RedesSensores/Trabajo/Graficas/"

print("Leido")
Total = pd.DataFrame(columns = ['aX','aY','aZ','gX','gY','gZ','golpe','persona'])
for f in os.listdir(data_folder+"/txt/"):
    Raw = pd.read_csv(os.path.join(data_folder,'txt',f))
    Datos = pd.DataFrame(columns = ['aX','aY','aZ','gX','gY','gZ'])


    indice = 0

    for i in Raw.index:
        if Raw.iloc[i,0][-1] == ";":
            
            d = Raw.iloc[i,0][13:-1]
            index = d.find(";")
            if index ==-1:
                col = d[:2]
                valor = d [3:]
                Datos.loc[indice,col] = valor
                if col == "gZ":
                    indice = indice +1
            else:
                while index!=-1:
                    d2 = d[:index]
                    if d.find(";") == -1:
                        d2 = d
                    col = d2[:2]
                    valor = d2 [3:]
                    Datos.loc[indice,col] = valor
                    if col == "gZ":
                        indice = indice +1
                    index = d.find(";")
                    d = d[(index+1):]
    Datos["golpe"] = f[4:-4]
    Datos["persona"] = f[:4]
    if Datos.loc[0,"persona"] == 'Jaco':
        Datos.loc[:,'aX'] = -Datos.loc[:,'aX'].values.astype(float)
        Datos.loc[:,'aY'] = -Datos.loc[:,'aY'].values.astype(float)
        Datos.loc[:,'aZ'] = -Datos.loc[:,'aZ'].values.astype(float)
        Datos.loc[:,'gX'] = -Datos.loc[:,'gX'].values.astype(float)
        Datos.loc[:,'gY'] = -Datos.loc[:,'gY'].values.astype(float)
    print(Datos.columns)
    Datos.to_csv(os.path.join(data_folder,'csv',f[:-4]+".csv"))
    Total = Total.append(Datos)
Total = Total.reset_index(drop = True)
Total.to_csv(os.path.join(data_folder,'csv',"Total.csv"))
    
print("Limpiado")