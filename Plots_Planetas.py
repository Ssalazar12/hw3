import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

#Carga los datos
datax=np.genfromtxt('resultadosx.csv')
datay=np.genfromtxt('resultadosy.csv')
dataz=np.genfromtxt('resultadosz.csv')

fig = plt.figure(figsize=(13,7))
#crea el subplot 3d
ax = fig.add_subplot(121, projection= '3d')
ax.scatter(datax[:,0], datay[:,0],dataz[:,0])
plt.title('Orbitas en 3D')
for i in range(1,9):
    ax.plot(datax[:,i], datay[:,i],dataz[:,i])
    
#crea el subplot 2d   
plt.subplot(122)
plt.title('Orbitas en 2D')
for i in range(1,9):
    plt.plot(datax[:,i], datay[:,i])

plt.savefig('orbitas.png')
