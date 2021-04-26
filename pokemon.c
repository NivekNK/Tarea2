#include "Map.h"
#include "list.h"
#include "manageFiles.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Pokemon Pokemon;
typedef struct Pokedex Pokedex;

struct Pokemon
{
    int id;
    char* nombre;
    int pc;
    int ps;
    char* sexo;
    Pokedex* pokedex;
};

struct Pokedex
{
    char* nombre;
    int existencia;
    char* tipos;
    char* prev; //evolucion anterior
    char* pos;  //evolucion siguiente
    int numeroPokedex;
    char* region;
};

Pokemon* crearPokemon(char* linea)
{
    Pokemon* pokemon = (Pokemon*)malloc(sizeof(Pokemon));
    pokemon->id = 0;
    pokemon->nombre = (char*)malloc(sizeof(char) * 20);
    pokemon->pc = 0;
    pokemon->ps = 0;
    pokemon-> sexo = (char*)malloc(sizeof(char) * 10);

    for (int i = 0; i < 10; i++)
    {
        if (i == 0)
            pokemon->id = get_csv_field(linea, i);
    }

    return pokemon;
}

Pokedex* crearPokemonEnPokedex(char* linea)
{
    Pokedex* pokedex = (Pokedex*)malloc(sizeof(Pokedex));
    pokedex->nombre = (char*)malloc(sizeof(char) * 20);
    pokedex->existencia = 0;
    pokedex->tipos = (char*)malloc(sizeof(char) * 30);
    pokedex->prev = (char*)malloc(sizeof(char) * 20);
    pokedex->pos = (char*)malloc(sizeof(char) * 20);
    pokedex->numeroPokedex = 0;
    pokedex->region = (char*)malloc(sizeof(char) * 20);

    return pokedex;
}

void llenarPokedex(FILE* pokemonsFile, Map* pokedex)
{
    char linea[1024];
    char *aux = (char*)malloc(sizeof(char));
    int lineas = 0;
    while (fgets(linea, 1023, pokemonsFile) != NULL)
    {
        if (lineas != 0)
        {
            
        }
        lineas++;
    }
}

Map* crearPokedex(FILE* pokemonsFile)
{
    Map *pokedex = createMap(is_equal_string);

    llenarPokedex(pokemonsFile, pokedex);

    return pokedex;
}