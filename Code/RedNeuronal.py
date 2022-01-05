import numpy as np
import pandas as pd
import os
import matplotlib.pyplot as plt
import matplotlib.dates as md
import dateutil
from keras.models import Sequential
from keras.utils import to_categorical
from sklearn.model_selection import train_test_split
from keras.datasets import mnist
from keras.models import Sequential
from keras.layers import Conv2D
from keras.layers import MaxPool2D
from keras.layers import Flatten
from keras.layers import Dropout
from keras.layers import Dense

data_folder = "E:/RODRIGO/UNI/Master/RedesSensores/Trabajo/Datos/"
gaddress = "E:/RODRIGO/UNI/Master/RedesSensores/Trabajo/Graficas/"


df = pd.read_csv(os.path.join(data_folder,'csv','inc',"AllVentanas.csv"))

if 'Unnamed: 0' in df.columns:
    df = df.drop('Unnamed: 0',axis = 1)

model = Sequential()
model.add(Conv2D(120,(10,12),activation='relu',input_shape=(10,12,1)))
model.add(Flatten())
model.add(Dense(1, activation = 'softmax'))

model.compile(loss='mean_squared_error', optimizer='adam', metrics = ['accuracy'])
scores = []
df = df.dropna()
ventanas = df.ventana.unique()
vX, vY = train_test_split(ventanas, shuffle=True)

df_test=df[df.ventana.isin([vY])]
X_test=df_test.iloc[:,:12].values
y_test=df_test.golpeint.values.astype(int)
y_test1h = to_categorical(y_test, 5)

for ven in vX:
    df_train=df[df.ventana.isin([ven])]
    X_train=df_train.iloc[:,:12].values
    X_train= np.expand_dims(X_train,axis = 1)
    y_train=df_train.golpeint.values.astype(int)
    y_train = np.max(y_train)
   
    model.fit(X_train, np.array(y_train))

scores = model.evaluate(X_test, y_test)

print("\n%s: %.2f%%" % (model.metrics_names, scores*100))
#print (model.predict(training_data).round())


