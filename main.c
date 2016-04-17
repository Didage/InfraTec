/*
PROYECTO 1 FUNDAMENTOS DE INFRAESTRUCTURA TECNOLOGICA - 201610
*/

//Presentado por:
//	Juan David Guzman 
//	Diego Alejandro Tovar
//	Juan M. Lovera

#include "stdlib.h"
#include "stdio.h"
#include "math.h"

typedef struct archivo
{
	int tamanho;
	unsigned char *informacion;
} ARCHIVO;


void cargarArchivo(ARCHIVO *info, ARCHIVO *resultado, char *nomArchivoEntrada);
void guardarArchivo(ARCHIVO *data, char *nomArchivoSalida);
int obtenerNumBits(unsigned short clave);
void codificar(ARCHIVO *arch, ARCHIVO *resultado, unsigned short claveBinaria, int tamanhoClave);

unsigned char append(int offset, unsigned short origen, unsigned char destino);
unsigned char copy(unsigned short aCopiar, unsigned char destino);


int main(int argc, char* argv[])
{
	//Declaración de variables
	unsigned short claveBinaria;
	int tamanhoClave;

	//Se comprueba que se ingresaran 2 argumentos
	if (argc != 3)
	{
		printf("Faltan argumentos - Debe ser: archivoEntrada archivoSalida\n");
		system("pause");
		return -1;
	}

	//Se reserva espacio para las variables ARCHIVO en las cuales se guarda el archivo de entrada y el archivo de salida
	ARCHIVO *arch = (ARCHIVO*)malloc(sizeof (ARCHIVO));//Archivo de entrada
	ARCHIVO *resultado = (ARCHIVO*)malloc(sizeof (ARCHIVO));//Archivo de salida


	//Se imprimen los archivos de entrada y salida
	printf("Archivo de entrada %s\n", argv[1]);
	printf("Archivo de salida %s\n", argv[2]);


	//Se obtiene la clave
	printf("Ingrese la clave: ");
	scanf("%hu", &claveBinaria);
	printf("Clave ingresada: %hu\n", claveBinaria);


	//Se obtiene la longitud de la clave
	tamanhoClave = obtenerNumBits(claveBinaria);
	printf("numero de bits: %hu\n",tamanhoClave);

	if(tamanhoClave > 16)
	{
		printf("La clave no puede tener mas de 16 bits\n");
		system("pause");
		return -1;
	}

	//Se cargan los datos del archivo de entrada en la variable arch de tipo ARCHIVO
	cargarArchivo(arch, resultado, argv[1]);


	//Se codifica la informacion
	codificar(arch, resultado, claveBinaria, tamanhoClave);


	//Se guarda el archivo codificado
	guardarArchivo(resultado, argv[2]);


	//Fin
	system("pause");
	return 0;
}


/*
* Procedimiento que codifica
*/
//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION
void codificar(ARCHIVO *arch, ARCHIVO *resultado,  unsigned short claveBinaria, int tamanhoClave)
{
	unsigned int tamanhoArchivoLectura = (*arch).tamanho;
	unsigned int vecesCabeClaveEnArchivo = tamanhoArchivoLectura*8 / tamanhoClave;
	int bitsFaltan = tamanhoArchivoLectura % tamanhoClave;

	unsigned char * informacionLeida=(*arch).informacion;
	unsigned char * informacionEscrita = (*resultado).informacion;

	unsigned char * claveCompilada = (unsigned char*)malloc(tamanhoArchivoLectura);

	// Para Claves menores a 8 bits

	if(tamanhoClave<8) {

		
		unsigned char * iteradorSobreCompilada = claveCompilada;

		int offset = 0;

		for (int i = 0; i < tamanhoArchivoLectura; i++)	{
			unsigned char claveLocal = '\0';
			unsigned char claveAux = claveBinaria;

			claveLocal = append(offset, claveAux, claveLocal);

			for (int j = 0; j < 8/tamanhoClave; j++) {
				claveLocal = copy(claveBinaria, claveLocal);
			}

			if(offset!=0) {
				claveAux = claveAux >> (tamanhoClave-((8%tamanhoClave)-offset));
				claveLocal = copy(claveAux, claveLocal);
			}


			if(8%tamanhoClave!=0) {
				offset++;
				offset = offset%tamanhoClave;
			}

			(*iteradorSobreCompilada) = claveLocal;
			iteradorSobreCompilada++;
		}

	}

	for (int i = 0; i < tamanhoArchivoLectura; ++i) {
		(*informacionEscrita) = (*informacionLeida)^(*claveCompilada);
		informacionLeida++;
		informacionEscrita++;
		claveCompilada++;
	}

}

/*
* Procedimiento para poner partes de la clave al comienzo de una clave rotada. Esto es usado para contruir la clave char a char cuando tiene longitud menor a 8 bits
*/
unsigned char append(int offset, unsigned short origen, unsigned char destino) {
	if(offset==0){return destino;}
	unsigned char construible = (unsigned char) origen;
	construible = construible << (8-offset);
	construible = construible >> (8-offset);
	destino = destino | construible;
	return destino;
}

/*
* Procedimiento copia los bits en aCopiar en el destino conservando lo que el destino tiene mientras a copiar no tenga un tamaño mayor
*/
unsigned char copy(unsigned short aCopiar, unsigned char destino) {
	destino = destino << obtenerNumBits(aCopiar);
	unsigned char info = (unsigned char) aCopiar;
	destino = destino | info;
	return destino;
}


/*
* Procedimiento para obtener el numero de bits de la contraseña
*/
//TODO: DESARROLLAR COMPLETAMENTE ESTA FUNCION
int obtenerNumBits(unsigned short clave)
{
	unsigned short claveAux = clave;
	int respuesta = 1;
	while (claveAux = claveAux / 2)
	{
		respuesta++;
	}
	return respuesta;
}

/*
* Procedimiento para cargar un archivo de disco
*/
//NO MODIFICAR
void cargarArchivo(ARCHIVO *data, ARCHIVO *resultado, char *nomArchivoEntrada)
{
	FILE *streamArchivo;
	int tam;
	unsigned char * aux1;
	unsigned char * aux2;

	if (!(streamArchivo = fopen(nomArchivoEntrada, "rb")))
	{
		printf("No se puede abrir el archivo: %s\n", nomArchivoEntrada);
		system("pause");
		exit(EXIT_FAILURE);
	}

	fseek(streamArchivo, 0, SEEK_END);
	tam = ftell(streamArchivo);
	fseek(streamArchivo, 0, SEEK_SET);

	aux1 = (unsigned char*)calloc(tam, 1);
	aux2 = (unsigned char*)calloc(tam, 1);

	fread(aux1, 1, tam, streamArchivo);

	data->tamanho = tam;
	data->informacion = aux1;

	resultado->tamanho = tam;
	resultado->informacion = aux2;

	if (fclose(streamArchivo) != 0)
	{
		printf("Error cerrando el archivo");
	}
}

/*
* Procedimiento para guardar un archivo en disco
*/
//NO MODIFICAR
void guardarArchivo(ARCHIVO *data, char *nomArchivoSalida)
{
	FILE *streamArchivo;

	if (!(streamArchivo = fopen(nomArchivoSalida, "wb")))
	{
		printf("No se puede abrir el archivo: %s\n", nomArchivoSalida);
		system("pause");
		exit(EXIT_FAILURE);
	}

	fwrite((data->informacion), 1, (data->tamanho), streamArchivo);

	if (fclose(streamArchivo) != 0)
	{
		printf("Error cerrando el archivo");
	}
}
