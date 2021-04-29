#include "Map.h"
#include "list.h"
#include <stdio.h>

typedef struct Pokemon Pokemon;
typedef struct PokedexData PokedexData;
typedef struct Almacenamiento Almacenamiento;
typedef struct Pokedex Pokedex;

void llenarAlmacenamientos(FILE* pokemonsFile, Pokedex* pokedex, Almacenamiento* almac);

void pokemonAtrapado(Pokedex* pokedex, Almacenamiento* almac);

void mostrarPokemons(Pokedex* pokedex);

void scanLiberarPokemon(Pokedex* pokedex, Almacenamiento *almac);

void mostrarxPC (Almacenamiento* almac);

void mostrarRegion(Almacenamiento* almac);

void buscarNombreEnPokedex(Pokedex* pokedex);

void buscarNombreEnAlmacenamiento(Almacenamiento* almac);

Almacenamiento* crearAlmacenameintoVacio();

Pokedex* crearPokedexVacio();