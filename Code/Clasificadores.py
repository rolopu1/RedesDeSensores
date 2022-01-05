import numpy as np
import pandas as pd
import os
import matplotlib.pyplot as plt
import matplotlib.dates as md
import dateutil
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC
from sklearn.naive_bayes import GaussianNB
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import RandomForestClassifier, AdaBoostClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.linear_model import LogisticRegression
from time import time
from sklearn.metrics import precision_score,recall_score,f1_score
from sklearn.metrics import confusion_matrix

def plot_confusion_matrix(y_true, y_pred, classes,
                          normalize=False,
                          title=None,
                          cmap=plt.cm.Blues):
    """
    This function prints and plots the confusion matrix.
    Normalization can be applied by setting `normalize=True`.
    """
    if not title:
        if normalize:
            title = 'Normalized confusion matrix'
        else:
            title = 'Confusion matrix, without normalization'

    # Compute confusion matrix
    cm = confusion_matrix(y_true, y_pred)
    # Only use the labels that appear in the data
    #    classes = classes[unique_labels(y_true, y_pred)]
    if normalize:
        cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
        print("Normalized confusion matrix")
    else:
        print('Confusion matrix, without normalization')

    print(cm)

    fig, ax = plt.subplots(figsize=(4, 4), constrained_layout=True)
    im = ax.imshow(cm, interpolation='nearest', cmap=cmap)
    #ax.figure.colorbar(im, ax=ax)
    # We want to show all ticks...

    ax.set(xticks=np.arange(cm.shape[1]),
           yticks=np.arange(cm.shape[0]),
           # ... and label them with the respective list entries
           xticklabels=['Corte', 'Derecha', 'Mate', 'Reposo', 'Reves'], yticklabels=['Corte', 'Derecha', 'Mate', 'Reposo', 'Reves'],
           title=title,
           ylabel='True label',
           xlabel='Predicted label')

    # Rotate the tick labels and set their alignment.
    plt.setp(ax.get_xticklabels(), rotation=45, ha="right",
             rotation_mode="anchor")

    # Loop over data dimensions and create text annotations.
    fmt = '.2f' if normalize else 'd'
    thresh = cm.max() / 2.
    for i in range(cm.shape[0]):
        for j in range(cm.shape[1]):
            ax.text(j, i, format(cm[i, j], fmt),
                    ha="center", va="center",
                    color="white" if cm[i, j] > thresh else "black")
    fig.tight_layout()
    plt.close(fig)
    return fig

data_folder = "E:/RODRIGO/UNI/Master/RedesSensores/Trabajo/Datos/"
gaddress = "E:/RODRIGO/UNI/Master/RedesSensores/Trabajo/Graficas/"

df = pd.read_csv(os.path.join(data_folder,'csv','inc',"IncTotalReposo.csv"))

feature = "golpe"
if 'Unnamed: 0' in df.columns:
    df = df.drop('Unnamed: 0',axis = 1)
df = df.dropna()
df=df[~df.persona.isin(['Rodr'])]
for p in df.persona.unique():
    df_train=df[~df.persona.isin([p])]
    df_test=df[df.persona.isin([p])]
    X_train=df_train.iloc[:,:12].values
    y_train=df_train.golpe.values.astype(str)
    X_test=df_test.iloc[:,:12].values
    #y_test=np.concatenate([df19_test.loc[:,feature].values.astype(str),df20_test.loc[:,feature20].values.astype(str)])
    y_test=df_test.golpe.values.astype(str)

    #class    
    clf1 = GaussianNB()
    clf2 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=4),algorithm="SAMME", n_estimators=50)    
    clf3 = RandomForestClassifier(max_depth=6,n_estimators=6*6, random_state=0)    
    clf5 = SVC()
    clf6 = LogisticRegression(C=1000, solver='lbfgs', multi_class='multinomial')

    classi=[clf1,clf2,clf3,clf5,clf6]#[clf5, clf3]
    class_name=['NaiveBayes','AdaBoost','RandomForest','SVC','LogisticReg']#['SVC','RandomForest']
    aces = [feature +'_'+ cla for cla in class_name]
    predictions=pd.DataFrame(columns=aces)
    accuracy_l=[]
    train_label=['F1_score']
    for clf,name,column,j in zip(classi,class_name,predictions.columns,np.arange(len(class_name))):
        t0 = time()
        clf.fit(X_train, y_train)
        print ("training time:", round(time()-t0, 3), "s")

        t1=time()
        prediction=clf.predict(X_test)
        print ("Prediction time:", round(time()-t1, 3), "s")


        if 'Recall' in train_label:
            accuracy=recall_score(y_test, prediction,np.unique(y_test),average='weighted')
            param='Recall'
            print(accuracy)
        elif 'F1_score' in train_label:
            accuracy=f1_score(y_test,prediction,average='weighted')
            param='F1_score'
            print(accuracy)
        elif 'Precision' in train_label:
            accuracy=precision_score(y_test,prediction,average='macro')
            param='Precision'
            print(accuracy)
        else:
            #accuracy=accuracy_score(y_test,prediction)
            accuracy=f1_score(y_test,prediction,average='weighted')

        predictions[column]=prediction
        accuracy_l.append(accuracy)

        fig_cf=plot_confusion_matrix(y_test, prediction, classes=np.unique(y_test), title='CM_'+name+'_'+str(train_label[0:4]))
        #fig_cf.savefig(workspace+'/AllCW/NoOut_Product/Classifiers/'+'cm/'+param+'/'+train_label[0:4]+'/'+train_label[0:4]+'_Confusion matrix_'+name+'.jpg') 
        #Create folders to download
        try:
            os.mkdir(os.path.join(gaddress, 'cm'))
        except FileExistsError:
            print("La carpeta cm ya existe")
        print(p)
        print (name)      
        print (accuracy*100)
        fig_cf.savefig(os.path.join(gaddress+'/cm',param+'_Confusion matrix_'+p+name+'_'+str(np.round(accuracy,4))+'.png'))

        predictions,accuracy_l,confusion_matrix(y_test, prediction, labels=np.unique(y_test)),class_name