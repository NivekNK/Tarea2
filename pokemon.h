#include "Map.h"
#include "list.h"
#include <stdio.h>

typedef struct Pokemon Pokemon;
typedef struct PokedexData PokedexData;
typedef struct Almacenamiento Almacenamiento;

void llenarAlmacenamientos(FILE* pokemonsFile, Map* pokedex, Almacenamiento* almac,char* linea);

void pokemonAtrapado(FILE* pokemonsFile, Map* pokedexMap, Almacenamiento* almac);

void mostrarPokemons(Map* pokedex);

Almacenamiento* crearAlmacenameintoVacio();