#include "Map.h"
#include "list.h"
#include <stdio.h>

typedef struct Pokemon Pokemon;
typedef struct PokedexData PokedexData;
typedef struct Almacenamiento Almacenamiento;

void llenarAlmacenamientos(FILE* pokemonsFile, Map* pokedex, Almacenamiento* almac);

Almacenamiento* crearAlmacenameintoVacio();