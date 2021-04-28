#include "Map.h"
#include "list.h"
#include "manageFiles.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Pokemon Pokemon;
typedef struct PokedexData PokedexData;
typedef struct Almacenamiento Almacenamiento;

struct Pokemon
{
    int id;
    char* nombre;
    int pc;
    int ps;
    char* sexo;
    PokedexData* pokedex;
};

struct PokedexData
{
    char* nombre;
    int existencia;
    char* tipos;
    char* prev; //evolucion anterior
    char* pos;  //evolucion siguiente
    int numeroPokedex;
    char* region;
};

struct Almacenamiento
{
    Map* ids;
    Map* nombres;
    Map* pcs;
    Map* regiones;
};

PokedexData* crearPokedexDataVacio()
{
    PokedexData* pokedex = (PokedexData*)malloc(sizeof(PokedexData));

    pokedex->nombre = (char*)malloc(sizeof(char) * 20);
    pokedex->existencia = 0;
    pokedex->tipos = (char*)malloc(sizeof(char) * 30);
    pokedex->prev = (char*)malloc(sizeof(char) * 20);
    pokedex->pos = (char*)malloc(sizeof(char) * 20);
    pokedex->numeroPokedex = 0;
    pokedex->region = (char*)malloc(sizeof(char) * 20);

    return pokedex;
}

void crearPokemonEnPokedex(PokedexData* pokedex, char* aux, int i)
{
    if (i == 1) pokedex->nombre = aux;
    if (i == 2) pokedex->tipos = aux;
    if (i == 6) pokedex->prev = aux;
    if (i == 7) pokedex->pos = aux;
    if (i == 8) pokedex->numeroPokedex = atoi(aux);
    if (i == 9) pokedex->region = aux;
}

Pokemon* crearPokemon(char* linea, Map* map)
{
    Pokemon* pokemon = (Pokemon*)malloc(sizeof(Pokemon));
    PokedexData* pokedex = crearPokedexDataVacio();
    pokemon->id = 0;
    pokemon->nombre = (char*)malloc(sizeof(char) * 20);
    pokemon->pc = 0;
    pokemon->ps = 0;
    pokemon-> sexo = (char*)malloc(sizeof(char) * 10);

    for (int i = 0; i < 10; i++)
    {
        char* value = (char*)malloc(sizeof(char));
        value = (char*)get_csv_field(linea, i);

        if (i == 0) pokemon->id = atoi(value);
        if (i == 1)
        {
            pokemon->nombre = value;
            crearPokemonEnPokedex(pokedex, value, i);
        }
        if (i == 2) crearPokemonEnPokedex(pokedex, value, i);
        if (i == 3) pokemon->pc = atoi(value);
        if (i == 4) pokemon->ps = atoi(value);
        if (i == 5) pokemon->sexo = value;
        if (i > 5) crearPokemonEnPokedex(pokedex, value, i);
    }
       
    if (pokemon == searchMap(map, pokemon->nombre))
    {
        pokedex->existencia++;
    }
    else
    {
        pokedex->existencia = 1;
    }
    
    pokemon->pokedex = pokedex;
    return pokemon;
}

Almacenamiento* crearAlmacenameintoVacio()
{
    Almacenamiento* almac = (Almacenamiento*)malloc(sizeof(Almacenamiento));

    almac->ids = createMap(is_equal_int);
    setSortFunction(almac->ids, lower_than_int);

    almac->nombres = createMap(is_equal_string);

    almac->pcs = createMap(is_equal_int);
    setSortFunction(almac->pcs, lower_than_int);

    almac->regiones = createMap(is_equal_string);

    return almac;
}

void insertNoCopy(Map* pokedex, Pokemon* pokemon)
{
    Pokemon* poke = searchMap(pokedex, pokemon->nombre);
    if (poke != NULL)
    {
        eraseMap(pokedex, pokemon->nombre);
    }
    
    insertMap(pokedex, pokemon->nombre, pokemon->pokedex);
}

void insertListToMap(Map* map, void* key, void* value)
{
    List* list = searchMap(map, key);
    if (list == NULL)
    {
        free(list);
        list = createList();
    }
    else
    {
        eraseMap(map, key);
    }
    pushBack(list, value);

    insertMap(map, key, list);
}

/*int returnCurrentId()
{

}

void setCurrentId(int id)
{
    
}
*/

void llenarAlmacenamientos(FILE* pokemonsFile, Map* pokedexMap, Almacenamiento* almac,char* linea)
{
    char *aux = (char*)malloc(sizeof(char));
    int cont = 0;
    while (fgets(linea, 1023, pokemonsFile) != NULL)
    { 
        if (cont != 0)
        {
            Pokemon* pokemon = crearPokemon(linea, pokedexMap);

            insertNoCopy(pokedexMap, pokemon);

            insertMap(almac->ids, &pokemon->id, pokemon);
            insertMap(almac->nombres, pokemon->nombre, pokemon);
            insertListToMap(almac->pcs, &pokemon->pc, pokemon);
            insertListToMap(almac->regiones, pokemon->pokedex->region, pokemon);
        }
        cont++;
    }
}

void pokemonAtrapado(FILE* pokemonsFile, Map* pokedexMap, Almacenamiento* almac)
{
  char* nuevaLinea = (char *) malloc(sizeof(char) * 80);
  char* nombre = (char*)malloc(sizeof(char) * 20);
  char* tipos = (char*)malloc(sizeof(char)*20);
  char* PC = (char*)malloc(sizeof(char)*20);;
  char* PS=(char*)malloc(sizeof(char)*20);;
  char* numeroPokedex = (char*)malloc(sizeof(char)*20);;
  char* sexo = (char*)malloc(sizeof(char) * 20);
  char* evolucionPrevia = (char*)malloc(sizeof(char) * 20);
  char* evolucionPosterior = (char*)malloc(sizeof(char) * 20);
  char* region =(char*)malloc(sizeof(char) * 20); 
  printf("Ingrese nombre del pokemon atrapado");
  scanf("%s", nombre);
  strcpy(nuevaLinea, nombre);
  strcat(nuevaLinea, ",");
  printf("Ingrese tipos del pokemon atrapado");
  scanf("%s", tipos);
  strcpy(nuevaLinea, tipos);
  strcat(nombre, ",");
  printf("Ingrese los PC del pokemon atrapado");
  scanf("%s", PC);
  strcpy(nuevaLinea, PC);
  strcat(nuevaLinea, ",");
  printf("Ingrese los PS del pokemon atrapado");
  scanf("%s", PS);
  strcpy(nuevaLinea, PS);
  strcat(nuevaLinea, ",");
  printf("Ingrese el sexo del pokemon atrapado");
  scanf("%s", sexo);
  strcpy(nuevaLinea, sexo);
  strcat(nuevaLinea, ",");
  printf("Ingrese la evolucion previa del pokemon atrapado");
  scanf("%s", evolucionPrevia);
  strcpy(nuevaLinea, evolucionPrevia);
  strcat(nuevaLinea, ",");
    printf("Ingrese la evolucion posterior del pokemon atrapado");
  scanf("%s", evolucionPosterior);
  strcpy(nuevaLinea, evolucionPosterior);
  strcat(nuevaLinea, ",");
  printf("Ingrese  numero de pokedex del pokemon atrapado");
  scanf("%s", numeroPokedex);
  strcpy(nuevaLinea, numeroPokedex);
  strcat(nuevaLinea, ",");
  printf("Ingrese la Region del pokemon atrapado");
  scanf("%s", region);
  strcpy(nuevaLinea, region);
  strcat(nuevaLinea, ",");  
  llenarAlmacenamientos(pokemonsFile,pokedexMap,almac, nuevaLinea);
}

/*void mostrarPokemons(Map* pokedexMap)
{

}*/