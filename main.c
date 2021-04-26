#include "Map.h"
#include "list.h"
#include "pokemon.h"
#include "manageFiles.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
    FILE* newFile = NULL;
    while (newFile == NULL)
    {
        newFile = importarArchivo();
        fflush(stdin);
    }
    
    if (newFile != NULL) printf("Archivo importado!\n");

    Map* pokedex = crearPokedex(newFile);

    return 0;
}