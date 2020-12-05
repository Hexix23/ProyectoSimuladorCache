#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
// para linux usar #include <unistd.h>

int numfallos = 0;
float tiempoglobal = 0;

typedef struct{
    short int ETQ;
    short int Datos[8];
} T_LINEA_CACHE;

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
	int camposD[3];
	T_LINEA_CACHE lineaCache[4];

/*Inicializamos Cache*/
	inicializarCache(lineaCache);

/*Leemos RAM*/
	ram = fopen("RAM.bin", "rb"); //abre archivo en modo binario
	
	rewind(ram);  
	fgets(RAM, 1024, ram);
	fclose(ram);

/*leemos fichero*/
	f = fopen("accesos_memoria.txt", "r");	
	
	while (!feof(f)){
		acceso = leerFichero(f);
		separarCampos(acceso, camposD); 
		if(lineaCache[camposD[1]].ETQ != camposD[2]){
		  printf("Ha habido un error en la linea %02X con la etiqueta %X\n", camposD[1], lineaCache[camposD[1]].ETQ);
		  lineaCache[camposD[1]].ETQ = camposD[2];
		  mover = acceso & 0b1111111000;
		  //mover +=  8; 
		  printf("Datos: ");
		  for(i = 0; i < 8; i++){
			lineaCache[camposD[1]].Datos[i] = RAM[(mover++)];
		  }
		  while(i--){
			printf("%X", lineaCache[camposD[1]].Datos[i]);
		    printf(" ");
		  }
		  printf("\n");
		}
		texto[tamTexto++] = lineaCache[camposD[1]].Datos[camposD[0]];
		Sleep(2);
	}
	texto[tamTexto] = '\0';
	printf("%s", texto);
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