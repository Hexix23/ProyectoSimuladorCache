#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
// para linux usar #include <unistd.h>

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


	while (!feof(f)){
		acceso = leerFichero(f);
		separarCampos(acceso, camposD); 
		tiempoglobal += 1;
		bloque = camposD[1] + camposD[2];
		if(lineaCache[camposD[1]].ETQ != camposD[2]){
		  tiempoglobal += 10;
		  
		  printf("\nT:%d, Fallo de CACHE %d, ADDR %04X ETQ %X linea %02X palabra %02X bloque %02X\n", tiempoglobal,numfallos,acceso ,camposD[0],camposD[1],camposD[2],bloque);
		  printf("Cargando el bloque %X y la linea %02X",bloque, camposD[1]);
		  
		  lineaCache[camposD[1]].ETQ = camposD[2];
		  mover = acceso & 0b1111111000; 
		  
		  for(i = 0; i < 8; i++){
			lineaCache[camposD[1]].Datos[i] = RAM[(mover++)];
		  }

		  numfallos++;
		}
		else{	
			printf("T:%d, Acierto de CACHE, ADDR %04X ETQ %X linea %02X palabra %02X DATO %02X\n", tiempoglobal,numfallos,acceso ,camposD[0],camposD[1],camposD[2],lineaCache[camposD[1]].Datos[i]);
			printf("ETQ:%X  Datos: ",camposD[2]);
			while(i--){
				printf("%X", lineaCache[camposD[1]].Datos[i]);
		    	printf(" ");
		  	}
		}
		texto[tamTexto++] = lineaCache[camposD[1]].Datos[camposD[0]];
		cont_acc++;
		Sleep(2000);
	}
	texto[tamTexto] = '\0';
	printf("%s", texto);
	
    datos_finales(numfallos,tiempoglobal,cont_acc,tiempo_medio);
	
	fclose(f);

   return 0;
}

int leerFichero(FILE *f){
	char numADDR[5]; 

	fscanf(f, "%s", numADDR);
	return (int)strtol(numADDR, NULL, 16);
}
void separarCampos(int acceso, int *camposD){

	camposD[0] = acceso & 0b111;
	camposD[1] = acceso >> 3 & 0b11;
	camposD[2] = acceso >> 5 & 0b11111;

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
void datos_finales(int numfallos,int tiempoglobal, int cont_acc,int tiempo_medio ){
  
    tiempo_medio = tiempoglobal / (numfallos + cont_acc);
    printf("\n\nEl numero total de accesos ha sido: %d, el numero total de fallos ha sido: %d y el tiempo medio es: %d",cont_acc, numfallos, tiempo_medio);
	
}