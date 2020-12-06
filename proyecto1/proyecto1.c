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

void datos_finales(int num_fallos,int tiempoglobal, int cont_acc,int tiempo_medio);
int leerFichero(FILE *f);
void separarCampos(int acceso, int *camposD);
void inicializarCache(T_LINEA_CACHE*  lineaCache);

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
	int camposD[4];
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
	/*if (f ==NULL){
		printf("el fichero acceso_memoria.txt no existe");
		exit(-1);
	}*/	
	
	while (!feof(f)){
		acceso = leerFichero(f);
		separarCampos(acceso, camposD); 
		if(lineaCache[camposD[1]].ETQ != camposD[2]){
		  printf("Ha habido un error en la linea %02X con la etiqueta %X\n", camposD[1], lineaCache[camposD[1]].ETQ);
		  lineaCache[camposD[1]].ETQ = camposD[2];
		  mover = acceso & 0b1111111000; 
		  printf("Datos: ");
		  for(i = 0; i < 8; i++){
			lineaCache[camposD[1]].Datos[i] = RAM[(mover++)];
		  }
		  while(i--){
			printf("%X", lineaCache[camposD[1]].Datos[i]);
		    printf(" ");
		  }
		  printf("\n");
		  numfallos++;
		  tiempoglobal += 10;
		}
		else{
			printf("Acierto de cache....\n");
		}
		texto[tamTexto++] = lineaCache[camposD[1]].Datos[camposD[0]];
		Sleep(2000);
	}
	texto[tamTexto] = '\0';
	printf("%s", texto);

    int a=4;
    //imprimir fallos y aciertos
    if(camposD[2] != lineaCache[a].ETQ){
		numfallos+=1;
		printf("T: %d, Fallo de CACHE %d, ADDR %04X ETQ %X linea %02X palabra %02X bloque %02X", tiempoglobal,numfallos,acceso ,camposD[0],camposD[1],camposD[2],camposD[3]);
		tiempoglobal+=10;

		printf("\n cargando el bloque %02X y la linea %02X",camposD[3], camposD[1]);
	}else
	{
		printf(" T: %d, Acierto de CACHE, ADDR %04X ETQ %X linea %02X palabra %02X DATO %02X", tiempoglobal,numfallos,acceso ,camposD[0],camposD[1],camposD[2],camposD[3]);
	}


    //imprimir los datos
    for(a=0; a<4; a++){
		printf("\n ETQ: %X",lineaCache[a].ETQ);
		printf("\t datos: ");
		for(i=0; i<8;i++){
			printf("%X",lineaCache[a].Datos[i]);
		}

	}

	int cont_acc, tiempo_medio;
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
    camposD[3] = acceso >> 3 & 0b1111111;
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
    cont_acc = 7;
    tiempo_medio = tiempoglobal / (numfallos + cont_acc);
    printf("\n\nEl numero total de accesos ha sido: %d, el numero total de fallos ha sido: %d y el tiempo medio es: %d",cont_acc, numfallos, tiempo_medio);
	
}
