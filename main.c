#include "Map.h"
#include "list.h"
#include "pokemon.h"
#include "manageFiles.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void printOpciones();
void opciones(int, Pokedex*, Almacenamiento*, bool);
FILE* subir_archivo();

int main()
{
  bool imported = false;
  // Mapa de la pokedex, aqui se guarda con key nombre del pokemon
  Pokedex* pokedex = crearPokedexVacio();
  // Struct de almacenamiento, aqui estan los mapas necesarios para tu almacenamiento pokemon
  Almacenamiento *almac = crearAlmacenameintoVacio();
  int solicitud = 100;
  while ( solicitud != 0 )
  {
    //Solicitud de que elijan una opcion
    printf("Selecione una opcion\n");
    printOpciones();
    scanf("%d", &solicitud);
    //While para comprobar que la opcion que eliges este entre las opcines
    while(solicitud < 0 || solicitud > 10 ) 
    {
      printf("Selecione una opcion correcta\n");
      printOpciones();
      scanf("%d", &solicitud);
    }

    //Se solicita el nombre del archivo, lo almacenamos en newFile,luego comenzamos a rellenar el almacenamiento de este
    if (solicitud == 1)
    {
      FILE* newFile = subir_archivo();
      llenarAlmacenamientos(newFile, pokedex, almac);
      imported = true;
      fclose(newFile);
    }
    else opciones(solicitud, pokedex, almac, imported);

  }
 
  return 0;
}
//Estas son las opciones que mostramos con anterioridad.
void printOpciones ()
{
  printf("1-. Subir un archivo\n");
  printf("2-. Atrapar Pokemon\n");
  printf("3-. Evolucionar pokemon\n");
  printf("4-. Buscar pokemons por tipo\n");
  printf("5-. Buscar mis pokemons por nombre\n");
  printf("6-. Buscar pokemon por nombre en pokedex\n");
  printf("7-. Mostrar todos los pokemons de la pokedex\n");
  printf("8.- Mostrar mis pokemon ordenados por PC\n");
  printf("9-. Liberar pokemon\n");
  printf("10-. Mostrar mis pokemons por region\n");
  printf("0-. Salir\n");
}
//Cada opcion cumple con su funcion,llamando a la funcion necesaria.
void opciones(int solicitud, Pokedex* pokedex, Almacenamiento* almac, bool imported)
{
  switch (solicitud)
  {
    case 2:
      if(imported) pokemonAtrapado(pokedex, almac);
      else printf("Aun no has importado un archivo!\n");
    break;
    case 3:
      if(imported) evolucionPokemon(pokedex, almac);
      else printf("Aun no has importado un archivo!\n");
    break;
    case 4:
      if(imported) buscarPorTipo(almac);
      else printf("Aun no has importado un archivo!\n");
    break;
    case 5:
      if(imported) buscarNombreEnAlmacenamiento(almac);
      else printf("Aun no has importado un archivo!\n");
    break;
    case 6:
      if(imported) buscarNombreEnPokedex(pokedex);
      else printf("Aun no has importado un archivo!\n");
    break;
    case 7:
      if(imported) mostrarPokemons(pokedex);
      else printf("Aun no has importado un archivo!\n");
    break;
    case 8:
      if(imported) mostrarxPC (almac);
      else printf("Aun no has importado un archivo!\n");
    break;
    case 9:
      if(imported) scanLiberarPokemon(pokedex, almac);
      else printf("Aun no has importado un archivo!\n");
    break;
    case 10:
      if(imported) mostrarRegion(almac);
      else printf("Aun no has importado un archivo!\n");
    break;
    default:
    break;
  }
}

FILE* subir_archivo()
{
  FILE* newFile = NULL;
  while (newFile == NULL)
  {
    newFile = importarArchivo();
  }
    
  if (newFile != NULL) printf("Archivo importado!\n");

  return newFile;
}