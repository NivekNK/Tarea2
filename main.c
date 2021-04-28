#include "Map.h"
#include "list.h"
#include "pokemon.h"
#include "manageFiles.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printOpciones();
void opciones(int, FILE*, Map*, Almacenamiento*);
void subir_archivo();

int main()
{
    FILE* newFile = NULL;
    char* linea = (char*) malloc(sizeof(char) * 1024);
    while (newFile == NULL)
    {
        newFile = importarArchivo();
        fflush(stdin);
    }
    
    if (newFile != NULL) printf("Archivo importado!\n");

    // Mapa de la pokedex, aqui se guarda con key nombre del pokemon
    Map* pokedex = createMap(is_equal_string);
    // Struct de almacenamiento, aqui estan los mapas necesarios para tu almacenamiento pokemon
    Almacenamiento *almac = crearAlmacenameintoVacio();
    llenarAlmacenamientos(newFile, pokedex, almac);
  
    int solicitud = 100;
    while ( solicitud != 0 ) {
      printf("Selecione una opcion\n");
      printOpciones();
      scanf("%d", &solicitud);
    while(solicitud < 0 || solicitud > 10 ) 
    {
        printf("Selecione una opcion correcta\n");
        printOpciones();
        scanf("%d", &solicitud);
    }
      //operaciones
      opciones(solicitud, newFile,pokedex, almac);
    }

      
     
     
    return 0;
}
void printOpciones ()
{
  printf("1-. Subir archivo\n");
  printf("2-. Atrapar Pokemon\n");
  printf("3-. Evolucionar pokemon\n");
  printf("4-. Buscar pokemons por tipo\n");
  printf("5-. Mostrar existencia de mi pokemon por su nombre\n");
  printf("6-. \n");
  printf("7-. Mostrar todos los pokemons de la pokedex\n");
  printf("8.- Mostrar mis pokemon por PC(se encunetran ordenados\n");
  printf("9-. Liberar pokemon\n");
  printf("0-. Salir\n");
}

void opciones(int solicitud,FILE* newFile, Map* pokedex, Almacenamiento* almac)
{
  switch (solicitud)
  {
    case 1:
    //subir_archivo();
    break;
    case 2:
      pokemonAtrapado(pokedex,almac);
    break;
    default:
    break;
  }
}