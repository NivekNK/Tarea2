#include "Map.h"
#include "list.h"
#include "pokemon.h"
#include "manageFiles.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    llenarAlmacenamientos(newFile, pokedex, almac, linea);
    // ACA DEBERIA EL SWITCH SI TUVIERAMOS SWITCH :(
     pokemonAtrapado(newFile,pokedex,almac);
     
    return 0;
}