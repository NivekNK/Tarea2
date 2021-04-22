#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "archivos.h"
#include "pokemon.h"

void printOpciones()
{
    printf("1-. Importar/Exportar desde un archivo\n");
    printf("2-. Pokémon atrapado\n");
    printf("3-. Evolucionar pokemon\n");
    printf("4-. Buscar mis pokémon por tipo\n");
    printf("5-. Buscar mis pokémon por nombre\n");
    printf("6-. Buscar por nombre en pokédex\n");
    printf("7-. Mostrar todos los pokémon de la pokédex\n");
    printf("8-. Mostrar mis pokémon ordenados por PC\n");
    printf("9-. Liberar pokémon\n");
    printf("10-. Mostrar pokémon por región\n");
    printf("0-. Salir\n");
}

void opciones(int solicitud, pokedex j)
{
  switch ( solicitud ) 
  {
    case 1:
    break;

    case 2: 
    pokemon_atrapado(j);
    break;

    case 3:

    break;

    case 4:

    break;

    case 5:

    break;

    default:
    break;
  }
}

FILE* LeerArchivo()
{
  FILE *file = NULL;
  printf("ingrese nombre archivo: ");
  while (file == NULL)
  {
    char* filename = (char*)malloc(sizeof(char));
    fgets(filename, 30, stdin);
    filename[strcspn(filename, "\n")] = 0;
    strcat(filename, ".csv");
    file = fopen(filename, "r");
    if (file == NULL)
    {
      printf("Archivo no encontrado!. Ingrese otro archivo:\n ");
    }
  }
  
  return file;
}