#include "Map.h"
#include "list.h"
#include "manageFiles.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define QUOTE "\""

/* 
listas
lista pcs
lista nombres
lista tipos
lista region
*/

typedef struct Pokemon Pokemon;
typedef struct PokedexData PokedexData;
typedef struct Almacenamiento Almacenamiento;
typedef struct Pokedex Pokedex;

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
    Map* tipos;
};

struct Pokedex
{
  Map* nombre;
  Map* numeroEnPokedex;
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

Pokemon* crearPokemon(char* linea, Pokedex* pokedex)
{
    Pokemon* pokemon = (Pokemon*)malloc(sizeof(Pokemon));
    PokedexData* pokedexData = crearPokedexDataVacio();
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
            crearPokemonEnPokedex(pokedexData, value, i);
        }
        if (i == 2) crearPokemonEnPokedex(pokedexData, value, i);
        if (i == 3) pokemon->pc = atoi(value);
        if (i == 4) pokemon->ps = atoi(value);
        if (i == 5) pokemon->sexo = value;
        if (i > 5) crearPokemonEnPokedex(pokedexData, value, i);
    }
    pokedexData->existencia = 1;

    PokedexData* aux = (PokedexData*)searchMap(pokedex->nombre, pokemon->nombre);
    if (aux != NULL)
    {
      pokedexData->existencia += aux->existencia;
    }
    
    pokemon->pokedex = pokedexData;
    return pokemon;
}

Almacenamiento* crearAlmacenameintoVacio()
{
  Almacenamiento* almac = (Almacenamiento*)malloc(sizeof(Almacenamiento));

  almac->ids = createMap(is_equal_int);
  setSortFunction(almac->ids, lower_than_int);

  almac->nombres = createMap(is_equal_string);

  almac->pcs = createMap(is_equal_int);
  setSortFunction(almac->pcs, upper_than_int);

  almac->regiones = createMap(is_equal_string);
  almac->tipos = createMap(is_equal_string);
  return almac;
}

Pokedex* crearPokedexVacio()
{
  Pokedex* pokedex = (Pokedex*)malloc(sizeof(Pokedex));

  pokedex->nombre = createMap(is_equal_string);
  pokedex->numeroEnPokedex = createMap(is_equal_int);
  setSortFunction(pokedex->numeroEnPokedex, lower_than_int);

  return pokedex;
}

void insertNoCopy(Pokedex* pokedex, Pokemon* pokemon)
{
    if ( searchMap(pokedex->nombre, pokemon->nombre) != NULL )
    {
        eraseMap(pokedex->nombre, pokemon->nombre);
    }
    insertMap(pokedex->nombre, pokemon->nombre, pokemon->pokedex);

    if ( searchMap(pokedex->numeroEnPokedex, &pokemon->pokedex->numeroPokedex) != NULL )
    {
        eraseMap(pokedex->numeroEnPokedex, &pokemon->pokedex->numeroPokedex);
    }
    insertMap(pokedex->numeroEnPokedex, &pokemon->pokedex->numeroPokedex, pokemon->pokedex);
}

void insertListToMap(Map* map, void* key, void* value)
{
    List* list = (List*)searchMap(map, key);
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

void insertarAMapas(Pokedex* pokedex, Almacenamiento* almac, char* linea)
{
  Pokemon* pokemon = crearPokemon(linea, pokedex);
  insertNoCopy(pokedex, pokemon);
  insertMap(almac->ids, &pokemon->id, pokemon);
  insertListToMap(almac->nombres, pokemon->nombre, pokemon);
  insertListToMap(almac->pcs, &pokemon->pc, pokemon);
  insertListToMap(almac->regiones,pokemon->pokedex->region, pokemon);
  insertListToMap(almac->tipos, pokemon->pokedex->tipos,pokemon);
}

void llenarAlmacenamientos(FILE* pokemonsFile, Pokedex* pokedex, Almacenamiento* almac)
{
  char linea[1024];
  char *aux = (char*)malloc(sizeof(char));
  int cont = 0;
  while (fgets(linea, 1023, pokemonsFile) != NULL)
  { 
    if (cont != 0)
    {
      linea[strcspn(linea, "\n")] = 0;
      strcat(linea, ",");
      insertarAMapas(pokedex, almac, linea);
    }
    cont++;
  }
}

void borrarDeLista(Map* map, void* key, List* list, int id)
{
  if ( list != NULL ) 
  {
    Pokemon* poke = (Pokemon*) firstList(list);
    while ( poke != NULL) 
    {
      if ( poke->id == id )
      {
        popCurrent(list);
        break;
      }
      poke = nextList(list);
    }
    insertListToMap(map, key, list);
  }
  else 
  {
    printf("El id ingresado no existe");
    return;
  }
}

/* 
listas
lista nombres
lista pcs
lista tipos
lista region
*/
void liberarPokemon(int id, Pokedex* pokedex, Almacenamiento* almac)
{
  Pokemon* poke = (Pokemon*)searchMap(almac->ids, &id);
  if ( poke != NULL ) {
    eraseMap(almac->ids,&id);
    borrarDeLista(almac->nombres, poke->nombre, (List*)searchMap(almac->nombres, poke->nombre), id);
    borrarDeLista(almac->pcs, &poke->pc, (List*)searchMap(almac->pcs, &poke->pc), id);
    //borrarDeLista((List*)searchMap(almac->tipos, poke->tipos), id);
    borrarDeLista(almac->regiones, poke->pokedex->region, (List*)searchMap(almac->regiones, poke->pokedex->region), id);
  }
}

void scanLiberarPokemon(Pokedex* pokedex, Almacenamiento *almac)
{
  int id;
  printf("Ingrese el id del pokemon a eliminar: ");
  scanf("%i", &id);
  liberarPokemon(id, pokedex, almac);
}

void retornaPrint(int i)
{
  if ( i == 0 ) printf("Ingrese nombre del pokemon atrapado: ");
  if ( i == 1 ) printf("Ingrese cantidad de tipos del pokemon atrapado: ");
  if ( i == 2 ) printf("Ingrese los PC del pokemon atrapado: "); 
  if ( i == 3 ) printf("Ingrese los PS del pokemon atrapado: "); 
  if ( i == 4 ) printf("Ingrese el sexo del pokemon atrapado: "); 
  if ( i == 5 ) printf("Ingrese la evolucion previa del pokemon atrapado: "); 
  if ( i == 6 ) printf("Ingrese la evolucion posterior del pokemon atrapado: "); 
  if ( i == 7 ) printf("Ingrese numero de pokedex del pokemon atrapado: "); 
  if ( i == 8 ) printf("Ingrese la Region del pokemon atrapado: "); 
}

char* retornaRespuesta(int i)
{
  char* res = (char*)malloc(sizeof(char) * 20);
  retornaPrint(i);
  if ( i == 1 ) 
  {
    char* lineaTipos = (char*) malloc(sizeof(char) * 20);
    scanf("%s", res);
    strcat(lineaTipos, QUOTE);
    for (int j = atoi(res) ; j > 0 ; j-- )
    {
      printf("Ingrese tipo: ");
      scanf("%s", res);
      strcat(lineaTipos, res);
      if ( j == 1 ) break;
      strcat(lineaTipos, ", ");
    }
    strcat(lineaTipos, QUOTE);
    return lineaTipos;
  }
  else
  {
    scanf("%s", res);
  }
  return res;
}

char* crearLinea(Almacenamiento* almac)
{
  char* nuevaLinea = (char *) malloc(sizeof(char) * 80);
  char* respuesta;

  Pokemon* aux = (Pokemon*)tailMap(almac->ids);
  char* ultimoId = (char*)malloc(sizeof(char) * 20);
  sprintf(ultimoId, "%d", aux->id + 1);
  strcat(nuevaLinea, ultimoId);
  strcat(nuevaLinea, ",");

  for (int i = 0 ; i < 9 ; i++)
  {
    respuesta = retornaRespuesta(i);
    strcat(nuevaLinea, respuesta);
    strcat(nuevaLinea, ",");
  }

  return nuevaLinea;
}

void pokemonAtrapado(Pokedex* pokedex, Almacenamiento* almac)
{
  char* nuevaLinea = crearLinea(almac);
  //printf("%s\n", nuevaLinea);
  insertarAMapas(pokedex,almac,nuevaLinea);
}

void evolucionPokemon(Pokedex* pokedex, Almacenamiento* almac)
{
  char* id = (char*) malloc(sizeof(char));
  scanf("%s", id);
  Pokemon* pokemon = searchMap(almac->ids,id);
  if ( pokemon != NULL) 
  {
    PokedexData* pokedexData = pokemon->pokedex;
    if (pokedexData->pos != NULL) //comparar si esta en su maxima evolucion
    {
      
    }
    else printf("EL pokemon se encuentra en su maxima evolucion");
  } 
  else printf("El id ingresado no existe\n");

}
/*
 Pokemon* pokemon = searchMap(almac->ids,id);
 if ( pokemon != NULL)
 {
   PokedexData* pokedex = pokemon->pokedex;
   if (pokedex->pos != NULL) //comparar si esta en su maxima evolucion
   {
     liberarPokemon(pokemon);
     char* linea = crearLinea(pokemon); // crea un línea con todos los datos necesarios de un pokemon y aumenta los PC y PS
     pokemon = crearPokemon(linea); //Función que crea pokemons
     insertarAMapas(pokedex, almac, pokemon);
   }
   else printf("EL pokemon se encuentra en su maxima evolucion");
 }
 else printf("El id ingresado no existe\n");
*/

void mostrarPokemons(Pokedex* pokedex) // de la pokedex
{
  Map* pokedexMap = pokedex->numeroEnPokedex;
  PokedexData* aux = (PokedexData*)firstMap(pokedexMap);
  while (aux != NULL)
  {
    printf("%s, %i, %s, %s, %s, %i, %s\n", aux->nombre, aux->existencia, aux->tipos, aux->prev, aux->pos, aux->numeroPokedex, aux->region);
    aux = nextMap(pokedexMap);
  }
}

void buscarNombreEnPokedex(Pokedex* pokedex)
{
  Map* pokedexMap = pokedex->nombre;
  char* buscar = (char*)malloc(sizeof(char));
  printf("Nombre del pokemon a buscar: ");
  scanf("%s", buscar);
  PokedexData* aux = (PokedexData*)searchMap(pokedexMap, buscar);
  if (aux == NULL)
  {
    printf("Este pokemon no existe en tu pokedex!\n");
    return;
  }
  printf("%s, %i, %s, %s, %s, %i, %s\n", aux->nombre, aux->existencia, aux->tipos, aux->prev, aux->pos, aux->numeroPokedex, aux->region);
}

void buscarNombreEnAlmacenamiento(Almacenamiento* almac)
{
  char* buscar = (char*)malloc(sizeof(char));
  printf("Nombre del pokemon a buscar: ");
  scanf("%s", buscar);
  List* list = (List*)searchMap(almac->nombres, buscar);
  if ( list == NULL )
  {
    printf("Este pokemon no existe en tu almacenamiento!\n");
    return;
  }
  Pokemon* aux = (Pokemon*)firstList(list);
  while ( aux != NULL )
  {
    printf("%i, %s, %i, %i, %s\n", aux->id, aux->nombre, aux->pc, aux->ps, aux->sexo);
    aux = (Pokemon*)nextList(list);
    if (aux == NULL) printf("No hay mas!\n");
  }
  
}

void mostrarxPC (Almacenamiento* almac)
{
  List* listPc = (List*) firstMap(almac->pcs);
  if ( listPc == NULL ) return;
  Pokemon* poke = (Pokemon*) firstList(listPc);
  printf("PC,NOMBRE\n");
  while ( listPc != NULL ) 
  {
    printf("%d,", poke->pc);
    printf("%s", poke->nombre);
    listPc = (List*) nextMap(almac->pcs);
    poke = firstList(listPc);
    printf("\n");
  }
  printf("----------------------------------------------\n");
}

void mostrarMenuPokedex()
{
  printf("ID,");
  printf("NOMBRE,");
  printf("EXISTENCIA,");
  printf("TIPOS,");
  printf("PC,");
  printf("PS,");
  printf("SEXO,");
  printf("EV. PREV,");
  printf("EV. POS,");
  printf("NUM POKEDEX\n");
}
void mostrarRegion(Almacenamiento* almac)
{
  char* region = (char*) malloc(sizeof(char));
  printf("Ingrese la region a verificar: ");
  scanf("%s", region);
  List* listPoke = (List*) searchMap(almac->regiones,region);
 
  if ( listPoke != NULL ) {
     printf("----------------------------------------------\n");
    printf("|%46c|\n", 32);
    printf("%10cRegion ingresada: %s\n", 32, region);
    printf("|%5cSe tienen %d pokemons en la region%7c|\n",32, listSize(listPoke),32);
    printf("|%46c|\n", 32);
    printf("----------------------------------------------\n");
  
    Pokemon* poke = (Pokemon*) firstList(listPoke);
    mostrarMenuPokedex();
    while ( poke != NULL ) 
    {
      printf("%d,%s,%d,%s,%d,%d,%s,%s,%s,%d", poke->id, poke->nombre,poke->pokedex->existencia,poke->pokedex->tipos, poke->pc,poke->ps, poke->sexo, poke->pokedex->prev, poke->pokedex->pos, poke->pokedex->numeroPokedex);
      poke = nextList(listPoke);
      printf("\n");
    }
  }
  else printf("La region ingresada no existe\n");
  printf("---------------------------------------------------------------------\n");
}