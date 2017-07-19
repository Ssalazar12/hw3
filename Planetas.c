#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define SIZE 1000 //cantidad de iteraciones, size de arrays
#define PLANETS 10
double G; //constante gravitacional
float dt; //intervalo de tiempo

/*Posiciones para cada planeta y el sol el primer indice indica el planeta
	[0][:] es el sol y [9][:] es pluton
*/
float x[PLANETS][SIZE]; 
float y[PLANETS][SIZE]; 
float z[PLANETS][SIZE]; 
/* velocidades para cada planeta y el sol*/
float vx[PLANETS][SIZE]; 
float vy[PLANETS][SIZE];
float vz[PLANETS][SIZE];
float m[PLANETS]; //masas

char *line; // recibe la linea de buffer
char *part; // recibe las partes de la linea
FILE *fstream;
float data[10][8]; //guarda los datos incluyendo el valor en float de los strings
float numbers[10][7]; //condiciones iniciales
char buffer[1000]; //guarda cada linea del fgets

void init(){
/* inicializa las condiciones iniciales */
	int k;
	dt=0.00005;
	G= 6.67408E-11;
	
	for(k=0;k<PLANETS;k++){
		
		m[k] = numbers[k][0];

		x[k][0] = numbers[k][1];
		y[k][0] = numbers[k][2];
		z[k][0] = numbers[k][3];
	
		vx[k][0] = numbers[k][4];
		vy[k][0] = numbers[k][5];
		vz[k][0] = numbers[k][6];
	}
}
	
float Fx(int planet, int time){
	/*Calcula la fuerza en x, recibe el tiempo(time) y el planeta(planet), retorna la fuerza en x*/
	int k;
	float forcex=0;
	float r; //distancia entre planet y k
	//sumatoria de fuerzas
	for(k=0;k<PLANETS;k++){ 
		//primero se asegura de que no incluya el planeta sobre el que se calcula
		if(k==planet){
			continue;	
		}
		r=sqrt( pow((x[k][time]-x[planet][time]),2) + pow((y[k][time]-y[planet][time]),2) + pow((z[k][time]-z[planet][time]),2));
		forcex += (m[k]/pow(r,3))*(x[k][time]-x[planet][time]);
	}
	return forcex;
}

float Fy(int planet, int time){
	/*Calcula la fuerza en y, recibe el tiempo(time) y el planeta(planet), retorna la fuerza en y*/
	int k;
	float forcey=0;
	float r; //distancia entre planet y k
	//sumatoria de fuerzas
	for(k=0;k<PLANETS;k++){ 
		//primero se asegura de que no incluya el planeta sobre el que se calcula
		if(k==planet){
			continue;	
		}
		r=sqrt( pow((x[k][time]-x[planet][time]),2) + pow((y[k][time]-y[planet][time]),2) + pow((z[k][time]-z[planet][time]),2));
		forcey += (m[k]/pow(r,3))*(y[k][time]-y[planet][time]);
	}
	return forcey;
}

float Fz(int planet, int time){
	/*Calcula la fuerza en z, recibe el tiempo(time) y el planeta(planet), retorna la fuerza en z*/
	int k;
	float forcez=0;
	float r; //distancia entre planet y k
	//sumatoria de fuerzas
	for(k=0;k<PLANETS;k++){ 
		//primero se asegura de que no incluya el planeta sobre el que se calcula
		if(k==planet){
			continue;	
		}
		r=sqrt( pow((x[k][time]-x[planet][time]),2) + pow((y[k][time]-y[planet][time]),2) + pow((z[k][time]-z[planet][time]),2));
		forcez += (m[k]/pow(r,3))*(z[k][time]-z[planet][time]);
	}
	return forcez;
}

void files(){
	/*Crea archivos csv con los resultados para ser graficados en python. Solo se necesitan las posiciones*/
	FILE *fx;
	FILE *fy;
	FILE *fz;
	int i;
	
	fx = fopen("resultadosx.csv", "w+");
	fy = fopen("resultadosy.csv", "w+");	
	fz = fopen("resultadosz.csv", "w+");	

	for(i=0; i<SIZE; i++){
	fprintf(fx,"%f %f %f %f %f %f %f %f %f %f\n",x[0][i],x[1][i],x[2][i],x[3][i],x[4][i],x[5][i],x[6][i],x[7][i],x[8][i],x[9][i]);	fprintf(fy,"%f %f %f %f %f %f %f %f %f %f\n",y[0][i],y[1][i],y[2][i],y[3][i],y[4][i],y[5][i],y[6][i],y[7][i],y[8][i],y[9][i]);
	fprintf(fz,"%f %f %f %f %f %f %f %f %f %f\n",z[0][i],z[1][i],z[2][i],z[3][i],z[4][i],z[5][i],z[6][i],z[7][i],z[8][i],z[9][i]);
	}
	
	fclose(fx);
	fclose(fy);
	fclose(fz);
	
}


int main()
{
	/*
		Comienza leyendo y guardando el archivo
	*/
	int i;
	int j;
	
	fstream=fopen("coordinates.csv", "r"); //abre el stream
	if(fstream == NULL) {//asegura si abrio el archivo
		printf("\n No se pudo abrir el archivo \n ");
	}
	
	//itera sobre las lineas del archivo
	for(i=0;i<10;i++){
		line=fgets(buffer, sizeof(buffer), fstream);
		if(line!=NULL){
			part= strtok(line, ",");
			for(j=0;j<8;j++){
				//printf("%s", part);
				data[i][j]=atof(part);	
				part=strtok(NULL,",");
			}	
		}	
	}
	
	//se copian los datos a otra matriz para sacar los nombres
	/*
	Los datos para condiciones iniciales estan organizados asi: 
				masa--posiciones--velocidades
	sol
	mercurio
	.
	.
	.	
	*/
	for(i=0;i<10;i++){
		for(j=0;j<7;j++){
			numbers[i][j]=data[i][j+1];	
		}	
	}
	
	/*incializa las variables*/
	init();
	
	/*Busca la solucion por leapfrog. i es el time step y j cambia el indice del planeta
		por ejemplo vx[j=0][i] se refiere a la velocidad en x del sol en el tiempo i
	*/
	for(i=0;i<SIZE;i++){
		for(j=0;j<PLANETS;j++){
			vx[j][i+1] = vx[j][i] +	G*dt*Fx(j,i);
			vy[j][i+1] = vy[j][i] +	G*dt*Fy(j,i);
			vz[j][i+1] = vz[j][i] +	G*dt*Fz(j,i);
			
			x[j][i+1] = x[j][i] + dt*(vx[j][i+1]);
			y[j][i+1] = y[j][i] + dt*(vy[j][i+1]);
			z[j][i+1] = z[j][i] + dt*(vz[j][i+1]);
 		}	
	}
	
	/* Crea los archivos con las orbitas*/
	files();
	
}