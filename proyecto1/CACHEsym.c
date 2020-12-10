#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <windows.h>
#include <unistd.h>

int numfallos = 0;
int tiempoglobal = 0;

typedef struct{
    short int ETQ;
    short int Datos[8];
} T_LINEA_CACHE;

int leerFichero(FILE *f);
void separarCampos(int acceso, int *camposD);
void inicializarCache(T_LINEA_CACHE*  lineaCache);
void datos_finales(int num_fallos,int tiempoglobal, int cont_acc,int tiempo_medio);

/* Función principal */
int main (int argc,char **argv){
	FILE *f = NULL;
	FILE *ram;
	unsigned char RAM[1024];
	char texto[100];
	int tamTexto;
	int acceso;
	int mover;
	int i;
	int bloque;
	int cont_acc = 0;
	int tiempo_medio;
	int camposD[3];
	T_LINEA_CACHE lineaCache[4];

/*Inicializamos Cache*/
	inicializarCache(lineaCache);

/*Leemos RAM*/
	ram = fopen("RAM.bin", "rb"); //abre archivo en modo binario
	if (ram ==NULL){
		printf("el fichero ram.bin no existe");
		exit(-1);
	}
	
	rewind(ram);  
	fgets(RAM, 1024, ram);
	fclose(ram);

/*leemos fichero*/
	f = fopen("accesos_memoria.txt", "r");	
	if (f ==NULL){
		printf("el fichero acceso_memoria.txt no existe");
		exit(-1);
	}

	/*leemos fichero memoria hasta el final*/
	while (!feof(f)){
		acceso = leerFichero(f);
		separarCampos(acceso, camposD); 
		tiempoglobal += 1;
		bloque = camposD[1] + camposD[2];
		/*comprobamos los fallos y aciertos de cache*/
		if(lineaCache[camposD[1]].ETQ != camposD[2]){
		  
		  printf("\nT:%d, Fallo de CACHE %d, ADDR %04X ETQ %X linea %02X palabra %02X bloque %02X\n", tiempoglobal,numfallos,acceso ,camposD[2],camposD[1],camposD[0],bloque);
		  printf("Cargando el bloque %X y la linea %02X\n",bloque, camposD[1]);
		  
		  lineaCache[camposD[1]].ETQ = camposD[2];
		  mover = acceso & 0b1111111000; 
		  
		  for(i = 0; i < 8; i++){
			lineaCache[camposD[1]].Datos[i] = RAM[(mover++)];
		  }

		  tiempoglobal += 10;
		  numfallos++;

		  printf("T:%d, Acierto de CACHE, ADDR %04X ETQ %X linea %02X palabra %02X DATO %2X\n", tiempoglobal, acceso, camposD[2], camposD[1], camposD[0], lineaCache[camposD[1]].Datos[camposD[0]]);
	      for(int a=0; a<4; a++){
				printf("ETQ:%X   Datos: ",lineaCache[a].ETQ);
				for(i=7; i >= 0;i--){
					printf("%X ",lineaCache[a].Datos[i]);
				}
				printf("\n");
			}

		}
		else{	
			printf("\nT:%d, Acierto de CACHE, ADDR %04X ETQ %X linea %02X palabra %02X DATO %2X\n", tiempoglobal, acceso, camposD[2], camposD[1], camposD[0], lineaCache[camposD[1]].Datos[camposD[0]]);
			
    		for(int a=0; a<4; a++){
				printf("ETQ:%X   Datos: ",lineaCache[a].ETQ);
				for(i=7; i >= 0;i--){
					printf("%X ",lineaCache[a].Datos[i]);
				}
				printf("\n");
			}
		}
		texto[tamTexto++] = lineaCache[camposD[1]].Datos[camposD[0]];
		cont_acc++;
		sleep(2);
		}
	texto[tamTexto] = '\0';
	
    datos_finales(numfallos,tiempoglobal,cont_acc,tiempo_medio);
	/*imprimimos el texto final*/
	printf("\nString final: %s", texto);
	/*cerramos el fichero*/
	fclose(f);
 
   return 0;
}

/*leemos fichero accesos_memoria*/
int leerFichero(FILE *f){
	char numADDR[5]; 

	fscanf(f, "%s", numADDR);
	return (int)strtol(numADDR, NULL, 16);
}
/*separamos la palabra, linea y la etiqueta*/
void separarCampos(int acceso, int *camposD){

	camposD[0] = acceso & 0b111;
	camposD[1] = acceso >> 3 & 0b11;
	camposD[2] = acceso >> 5 & 0b11111;

}
/*inicializamos las etiquetas a FF y los datos a 0*/
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
/*imprimimos el tiempo medio, numero de fallos y numero de accesos*/
void datos_finales(int numfallos,int tiempoglobal, int cont_acc,int tiempo_medio ){
  
    tiempo_medio = tiempoglobal / (numfallos + cont_acc);
    printf("\nEl numero total de accesos ha sido: %d, el numero total de fallos ha sido: %d y el tiempo medio es: %d",cont_acc, numfallos, tiempo_medio);
	
}
