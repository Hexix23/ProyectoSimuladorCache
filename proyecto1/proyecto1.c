#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numfallos = 0;
float tiempoglobal = 0;

typedef struct{
    short int ETQ;
    short int Datos[8];
} T_LINEA_CACHE;

int leerFichero(FILE *f);
int* separarCampos(int acceso);
void inicializarCache(T_LINEA_CACHE*  lineaCache); 



/* Función principal */
int main (int argc,char **argv){
	FILE *f = NULL;
	FILE *ram;
	unsigned char RAM[1024];
	int acceso;
	int *campos;
	T_LINEA_CACHE lineaCache[4];

/*Inicializamos Cache*/
	inicializarCache(lineaCache);

/*Leemos RAM*/
	ram = fopen("RAM.bin", "rb"); //abre archivo en modo binario
	rewind(ram);  // no entiendo muy bien la funcion del rewind
	fgets(RAM, 1024, ram);
	fclose(ram);

/*leemos fichero*/
	f = fopen("accesos_memoria.txt", "r");	
	
	while (!feof(f)){
		acceso = leerFichero(f);
		campos = separarCampos(acceso); 
		printf("Campo 1: %d // Campo 2: %d // Campo 3: %d\n", campos[0], campos[1], campos[2]);
	}
	fclose(f);

 /* if (f==NULL){
   printf("Error al abrir fichero.txt");
   return -1;
  }else if (f2==NULL){
   	printf("Error al abrir fichero2.txt");
   		return -1;
  	}*/
   return 0;
}

int leerFichero(FILE *f){
	char numADDR[5];  //¿porque es un array de 5?

	fscanf(f, "%s", numADDR);
	return (int)strtol(numADDR, NULL, 16);
}
int* separarCampos(int acceso){
	int *camposD = malloc(3 * sizeof(int));

	camposD[0] = acceso & 0b111;
	camposD[1] = acceso >>= 3 & 0b11;
	camposD[2] = acceso >> 2 & 0b11111;
	
	return camposD;
}

void inicializarCache(T_LINEA_CACHE  * lineaCache){
	int i;
	int a = 4;
	
	while (a-- > 0){
		i = 8;
		lineaCache[a].ETQ = 0xFF;
		while (i-- > 0){
			lineaCache[a].Datos[i] = 0; 
		}
		
	}
	
}