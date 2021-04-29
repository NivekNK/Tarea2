#include "Map.h"
#include "list.h"
#include "pokemon.h"
#include "manageFiles.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printOpciones();
void opciones(int, FILE*, Pokedex*, Almacenamiento*);
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
    Pokedex* pokedex = crearPokedexVacio();
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
  printf("5-. Buscar mis pokemons por nombre\n");
  printf("6-. Buscar pokemon por nombre en pokedex\n");
  printf("7-. Mostrar todos los pokemons de la pokedex\n");
  printf("8.- Mostrar mis pokemon por PC(se encunetran ordenados\n");
  printf("9-. Liberar pokemon\n");
  printf("10-. Mostrar mis pokemons por region\n");
  printf("0-. Salir\n");
}

void opciones(int solicitud,FILE* newFile, Pokedex* pokedex, Almacenamiento* almac)
{
  switch (solicitud)
  {
    case 1:
    //subir_archivo();
    break;
    case 2:
      pokemonAtrapado(pokedex,almac);
    break;
    case 5:
      buscarNombreEnAlmacenamiento(almac);
    break;
    case 6:
      buscarNombreEnPokedex(pokedex);
    break;
    case 7:
      mostrarPokemons(pokedex);
    break;
    case 8:
    mostrarxPC (almac);
    break;
    case 9:
     scanLiberarPokemon(pokedex,almac);
    break;
    case 10:
      mostrarRegion(almac);
    default:
    break;
  }
}