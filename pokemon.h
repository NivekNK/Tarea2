#include "Map.h"
#include "list.h"
#include <stdio.h>

typedef struct Pokemon Pokemon;
typedef struct Pokedex Pokedex;

Map* crearPokedex(FILE* pokemonsFile);