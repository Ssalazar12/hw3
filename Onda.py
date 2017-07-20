import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

#Crea la grid
N=300 #cantidad de puntos espaciales
xp=np.linspace(0 ,30 ,N)
yp=np.linspace(0 ,30 ,N)
dx=abs(xp[1]-xp[0])
dy=abs(yp[1]-yp[0])


c=1.0 #velocidad de propagacion
dt= 0.5*(dx/c) #unidad de tiempo con condiciones de estabilidad
time=int(60.0/dt) #cantidad de puntos de tiempo necesarios para que t=60 -->time=60/dt=1196
#como dx=dy se puede definir un gamma
gammax=(c*dt/dx) 
gammay=(c*dt/dy)

#CONDICIONES INICIALES
#La perturbacion inicial queda en 1/3 de un lado y comienza con 0.5, hay una barrera a 1/3 del lado opuesto con un hueco
#en los bordes y la barrera el potencial siempre es 0 y en t=0 u es 0 en todas partes menos en la perturbacion
#La solucion u va a depender de t,x
#velocidad en t=0 es 0

u0=np.zeros((N,N)) #array sin inicializar
u0[N/3-1,N/2-1]= -0.5 #preturbacion inicial

#crea las condiciones de frontera
frontier=np.zeros((N,N))
frontier[1:N-1,1:N-1]=1
frontier[N*(2/3)-1,N/2-2:N/+2]=1
frontier[199,:N/2-20]=0
frontier[199,N/2+20:]=0
frontier[N-1,:]=0

#Crea u1 usando la condicion inicial sobre la derivada en t=0
u1=np.zeros((N,N))
for j in range(1, len(xp)-1): #j corresponde a y
    for k in range(1, len(yp)-1):
        u1[j,k]= 0.5*((u0[j+1,k]-2*u0[j,k]+u0[j-1,k])*gammay**2 + (u0[j,k+1]-2*u0[j,k]+u0[j,k-1])*gammax**2)+ u0[j,k]
 
#fotogramas
u1=u1*frontier
frames=[u0,u1]

def solve30(): 
    #calcula la solucion despues de cierto intervalo de tiempo con barrera
    u=np.zeros((N,N))
    past=np.copy(u0) #i-1
    present=np.copy(u1) #i
    
    for i in range(0,time/2): 
        pre_r= np.roll(np.copy(present),1, axis=0) #equivalente a k+1, mueve las columnas
        pre_l= np.roll(np.copy(present),-1, axis=0) #equivalente a k-1, mueve las columnas  
        pre_u= np.roll(np.copy(present),-1, axis=1)# mueve las filas
        pre_d= np.roll(np.copy(present),1, axis=1)
        u=(pre_r-2*present+pre_l)*gammax**2 + (pre_d-2*present+pre_u)*gammay**2 + 2*present - past

        past=np.copy(present)   
        present=np.copy(u)
        present=present*frontier
    
    return present
        

def solve60(): 
    #calcula la solucion despues de cierto intervalo de tiempo con barrera
    u=np.zeros((N,N))
    past=np.copy(u0) #i-1
    present=np.copy(u1) #i
    counter=0 #para agregar los fotogramas
    
    for i in range(0,time): 
        
        pre_r= np.roll(np.copy(present),1, axis=0) #equivalente a k+1, mueve las columnas
        pre_l= np.roll(np.copy(present),-1, axis=0) #equivalente a k-1, mueve las columnas  
        pre_u= np.roll(np.copy(present),-1, axis=1)# mueve las filas
        pre_d= np.roll(np.copy(present),1, axis=1)
        u=(pre_r-2*present+pre_l)*gammax**2 + (pre_d-2*present+pre_u)*gammay**2 + 2*present - past
        
        counter+=1
        #pregunta para guardar un fotograma
        if(counter==20):
            frames.append(present)
            counter=0

        past=np.copy(present)   
        present=np.copy(u)
        present=present*frontier     
        
    return present
    
solution30 = solve30()
solution60 = solve60()

#crea graficas de t30 y t60
plt.figure(figsize=(10,7.5))
plt.subplot(121)
plt.title('Onda en t=60')
plt.imshow(solution60, cmap='Set2')

plt.subplot(122)
plt.title('Onda en t=30')
plt.imshow(solution30, cmap='Set2')
print np.shape(frames)

plt.savefig('Onda.png')

#animacion
fig= plt.figure(2)
im = plt.imshow( u0, animated ='True')

def animate(i):
    im.set_array(frames[i])
    return im

anim = animation.FuncAnimation(fig, animate, frames=58, interval=50, blit=False)
#########QUITAR EL COMENTARIO EN LA LINEA DE ABAJO###########################
	    		 #PARA GUARDAR LA ANIMACION#
#anim.save('Onda.mp4', writer='mencoder')


