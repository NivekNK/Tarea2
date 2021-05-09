
#include "Map.h"
#include "list.h"
#include "manageFiles.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define QUOTE "\""

typedef struct Pokemon Pokemon;
typedef struct PokedexData PokedexData;
typedef struct Almacenamiento Almacenamiento;
typedef struct Pokedex Pokedex;
//struct que almacena los pokemones y sus catct
struct Pokemon
{
    int id;
    char* nombre;
    int pc;
    int ps;
    char* sexo;
    PokedexData* pokedex;
};
//lo mismo que el anterior pero con con evoluciones porque es la pokedex
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
//mapas que guradan los pokemones caracteristicas
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
//se crea la pokedex alocando memoria 
PokedexData* crearPokedexDataVacio()
{
    PokedexData* pokedex = (PokedexData*)malloc(sizeof(PokedexData));

    pokedex->nombre = (char*)malloc(sizeof(char) * 20);
    pokedex->existencia = 0;
    pokedex->tipos = (char*)malloc(sizeof(char) * 20);
    pokedex->prev = (char*)malloc(sizeof(char) * 20);
    pokedex->pos = (char*)malloc(sizeof(char) * 20);
    pokedex->numeroPokedex = 0;
    pokedex->region = (char*)malloc(sizeof(char) * 20);

    return pokedex;
}
//esta funciom recive los datos y los pasa a la pokedex
void crearPokemonEnPokedex(PokedexData* pokedex, char* aux, int i)
{
    if (i == 1) pokedex->nombre = aux;
    if (i == 2) pokedex->tipos = aux;
    if (i == 6) pokedex->prev = aux;
    if (i == 7) pokedex->pos = aux;
    if (i == 8) pokedex->numeroPokedex = atoi(aux);
    if (i == 9) pokedex->region = aux;
}

Pokemon* crearPokemon(char* linea, Pokedex* pokedex, Almacenamiento* almac)
{
  Pokemon* pokemon = (Pokemon*)malloc(sizeof(Pokemon));
  PokedexData* pokedexData = crearPokedexDataVacio();
  pokemon->id = 0;
  pokemon->nombre = (char*)malloc(sizeof(char) * 20);
  pokemon->pc = 0;
  pokemon->ps = 0;
  pokemon-> sexo = (char*)malloc(sizeof(char) * 10);
//va creando la pokedex enviandose con un for, los datos se ingresan en la funcion de arriba
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
  if (aux != NULL) pokedexData->existencia += aux->existencia;
  List* listPoke = (List*) searchMap(almac->nombres,pokemon->nombre);
  Pokemon* poke = (Pokemon*) firstList(listPoke);
  while ( poke != NULL )
  {
    poke->pokedex->existencia = pokedexData->existencia;
    poke = (Pokemon*) nextList(listPoke);
  }
  pokemon->pokedex = pokedexData;
  return pokemon;
}

//crea almacenamiento en mapas
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
//se crea la pokedex con mapa
Pokedex* crearPokedexVacio()
{
  Pokedex* pokedex = (Pokedex*)malloc(sizeof(Pokedex));

  pokedex->nombre = createMap(is_equal_string);
  pokedex->numeroEnPokedex = createMap(is_equal_int);
  setSortFunction(pokedex->numeroEnPokedex, lower_than_int);

  return pokedex;
}
//se ve que no se inserte el mismo pokemon
void insertNoCopy(Pokedex* pokedex, Pokemon* pokemon)
{
  if (searchMap(pokedex->nombre, pokemon->nombre) != NULL)
  {
    eraseMap(pokedex->nombre, pokemon->nombre);
  }
  insertMap(pokedex->nombre, pokemon->nombre, pokemon->pokedex);

  if (searchMap(pokedex->numeroEnPokedex, &pokemon->pokedex->numeroPokedex) != NULL)
  {
      eraseMap(pokedex->numeroEnPokedex, &pokemon->pokedex->numeroPokedex);
  }
  insertMap(pokedex->numeroEnPokedex, &pokemon->pokedex->numeroPokedex, pokemon->pokedex);
}
//se ponen listas en los mapas 
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
//se ponen los mapas de tipo  
void insertarTiposAMapa(Map* tiposMap, char* tipos, void* value)
{
  char* aux = (char*)malloc(sizeof(char) * 20);
  strcpy(aux, tipos);
  aux[strcspn(aux, "\n")] = 0;
  strcat(aux, ",");

  char* key = (char*)get_csv_field(aux, 0);
  for (int i = 1; key != NULL; i++)
  {
    insertListToMap(tiposMap, key, value);
    key = (char*)get_csv_field(aux, i);
    if (key != NULL) remove_spaces(key);
  }
}
//funcion  que inserta en el almacenamiento
void insertarAMapas(Pokedex* pokedex, Almacenamiento* almac, char* linea)
{
  Pokemon* pokemon = crearPokemon(linea, pokedex, almac);
  //se asegura de que no halla mas de 100 pokemones
  int size = sizeMapa(almac->ids);
  if (size + 1 > 100)
  {
    printf("Tu almacenamiento sobrepasa el maximo de 100, pokemon no agregado al almacenamiento!\n");

    if (pokemon->pokedex->existencia == 1)
    {
      pokemon->pokedex->existencia = 0;
      insertNoCopy(pokedex, pokemon);
      printf("El pokemon que se ha intentado agregar era uno nuevo! Ha sido agregado a tu pokedex.\n");
    }
    else
    {
      free(pokemon);
      return;
    }
  }
  else
  {
    insertNoCopy(pokedex, pokemon);
    
    Pokemon* aux = (Pokemon*)searchMap(almac->ids, &pokemon->id);
    if (aux != NULL) pokemon->id = size + 1;
    insertMap(almac->ids, &pokemon->id, pokemon);

    insertListToMap(almac->nombres, pokemon->nombre, pokemon);
    insertListToMap(almac->pcs, &pokemon->pc, pokemon);
    insertListToMap(almac->regiones, pokemon->pokedex->region, pokemon);
    insertarTiposAMapa(almac->tipos, pokemon->pokedex->tipos, pokemon);
  }
}
//se lee la linea y se llena el almacenamiento
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
//se borra un pokemon de la lista de estos
void borrarDeLista(Map* map, void* key, int id)
{
  List* pokeList = (List*)searchMap(map, key);
  if (pokeList == NULL) return;
  Pokemon* aux = (Pokemon*)firstList(pokeList);
  while(aux != NULL)
  {
    if (aux->id == id)
    {
      popCurrent(pokeList);
      break;
    }
    aux = (Pokemon*)nextList(pokeList);
  }

  eraseMap(map, key);
  insertMap(map, key, pokeList);
}
// se saca el tipo del mapa con el aux 
void eliminarTiposDeMapa(Map* tiposMap, char* tipos, int id)
{
  char* aux = (char*)malloc(sizeof(char) * 20);
  strcpy(aux, tipos);
  aux[strcspn(aux, "\n")] = 0;
  strcat(aux, ",");

  char* key = (char*)get_csv_field(aux, 0);
  for (int i = 1; key != NULL; i++)
  {
    borrarDeLista(tiposMap, key, id);
    key = (char*)get_csv_field(aux, i);
    if (key != NULL) remove_spaces(key);
  }
}

/* 
listas
lista nombres
lista pcs
lista tipos
lista region
*/
//funciones que muestran los menus
void mostrarMenuAlmac()
{
  printf("ID,");
  printf("NOMBRE,");
  printf("PC,");
  printf("PS,");
  printf("SEXO\n");
}

void mostrarMenuPokedex()
{
  printf("NOMBRE,");
  printf("EXISTENCIA,");
  printf("TIPOS,");
  printf("EV. PREV,");
  printf("EV. POS,");
  printf("NUM POKEDEX \n");
}
void mostrarMenuRegion()
{
  printf("ID, ");
  printf("NOMBRE,");
  printf("EXISTENCIA,");
  printf("TIPOS,");
  printf("PC,");
  printf("PS,");
  printf("SEXO,");
  printf("EV. PREV,");
  printf("EV. POS,");
  printf("NUM POKEDEX \n");
}

PokedexData* disminuirExistencia(Pokedex* pokedex, Pokemon* pokemon)
{
  PokedexData* data = (PokedexData*)searchMap(pokedex->numeroEnPokedex, &pokemon->pokedex->numeroPokedex);
  data->existencia -= 1; 

  eraseMap(pokedex->nombre, pokemon->nombre);
  insertMap(pokedex->nombre, pokemon->nombre, data);
  eraseMap(pokedex->numeroEnPokedex, &pokemon->pokedex->numeroPokedex);
  insertMap(pokedex->numeroEnPokedex, &pokemon->pokedex->numeroPokedex, data); 

  return data;
}

void liberarPokemon(int id, Pokedex* pokedex, Almacenamiento* almac)
{
  Pokemon* poke = (Pokemon*)searchMap(almac->ids, &id);
  if (poke != NULL) 
  {
    eraseMap(almac->ids,&id);
    // borra el pokemon de lista de nombres //
    borrarDeLista(almac->nombres, poke->nombre, id); 
    // borra el pokemon de la lista de pcs // 
    borrarDeLista(almac->pcs, &poke->pc, id);
    // borra de la lista de tipos //
    eliminarTiposDeMapa(almac->tipos, poke->pokedex->tipos, id);
    // borra de la lista de regiones //
    borrarDeLista(almac->regiones, poke->pokedex->region, id);  
    PokedexData* aux = disminuirExistencia(pokedex, poke);
    
    List* listPoke = (List*) searchMap(almac->nombres,poke->nombre);
    Pokemon* poketemp = (Pokemon*) firstList(listPoke);
    while ( poketemp != NULL )
    {
      poketemp->pokedex->existencia = aux->existencia;
      poketemp = (Pokemon*) nextList(listPoke);
    }
  }
}
//se hace lee una linea del imput para liberar el pokemon
void scanLiberarPokemon(Pokedex* pokedex, Almacenamiento *almac)
{
  int id;
  printf("Ingrese el id del pokemon a eliminar: ");
  scanf("%i", &id);
  Pokemon* currentPoke = (Pokemon*)searchMap(almac->ids, &id);
  liberarPokemon(id, pokedex, almac);
  Pokemon* newPoke = (Pokemon*)searchMap(almac->ids, &id);
  if (currentPoke != newPoke)
  {
    printf("----------------------------------------------\n");
    printf("|%45c|\n", 32);
    printf("|%13cPOKEMON LIBERADO%16c|\n",32,32);
    printf("|%45c|\n", 32);
    printf("----------------------------------------------\n");
  }
  else
  {
    printf("El id ingresado no existe!\n");
  }
}
//da las instrucciones
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

void mostrarTipos()
{
  printf("Fuego,Volador,Normal,Electrico,Hada\n");
  printf("Lucha,Psiquico,Agua,Veneno,Fantasma\n");
  printf("Planta,Hielo,Roca,Siniestro,Acero\n");
  printf("Tierra,Dragon,Bicho\n");
}

char* comrpobarTipo() 
{
 char* res = (char*)malloc(sizeof(char) * 20);
 scanf("%s", res);
 while ( is_equal_string(res,"Fuego") != 1 &&  is_equal_string(res,"Volador") != 1 &&  is_equal_string(res,"Normal") != 1 &&  is_equal_string(res,"Electrico") != 1 &&  is_equal_string(res,"Hada") != 1 &&  is_equal_string(res,"Lucha") != 1 &&  is_equal_string(res,"Psiquico") != 1 &&  is_equal_string(res,"Agua") != 1 &&  is_equal_string(res,"Veneno") != 1 &&  is_equal_string(res,"Fantasma") != 1 &&  is_equal_string(res,"Planta") != 1 &&  is_equal_string(res,"Hielo") != 1 &&  is_equal_string(res,"Roca") != 1 &&  is_equal_string(res,"Siniestro") != 1  &&  is_equal_string(res,"Acero") != 1 &&  is_equal_string(res,"Tierra") != 1 &&  is_equal_string(res,"Dragon") != 1 && is_equal_string(res,"Bicho") != 1 )
 {
   printf("Debe ingresar un tipo existente!!\n");
   printf("Si no sabe que tipos existen escriba help\n");
   printf("Ingrese un tipo valido: ");
   scanf("%s", res);
   if ( is_equal_string("help", res) == 1 ) mostrarTipos();
 }
 return res;
}
//comprobacion del sexo cuando se ingresa
char* comprobarSexo()
{
  char* res = (char*)malloc(sizeof(char) * 20);
  scanf("%s", res);
  while ( is_equal_string(res,"Macho") != 1 && is_equal_string(res,"Hembra") != 1 && is_equal_string(res, "Double") != 1 )
  {
    printf("Por favor, ingrese un sexo valido\n");
    printf("Ingrese el sexo del pokemon: ");
    scanf("%s", res);
  }
  return res;
}

//se lee el tipo 
char* retornaRespuesta(int i)
{
  char* res = (char*)malloc(sizeof(char) * 20);
  retornaPrint(i);
  if ( i == 1 ) 
  {
    char* lineaTipos = (char*) malloc(sizeof(char) * 20);
    scanf("%s", res);
    while ( atoi(res) <= 0 ) 
    {
      printf("Debe ingresar al menos un tipo: ");
      scanf("%s", res);
    }
    if ( atoi(res) == 1 ) 
    {
      printf("Ingrese tipo: ");
      res = comrpobarTipo();
      strcpy(lineaTipos, res);
    } 
    else
    {
      strcat(lineaTipos, QUOTE);
      for (int j = atoi(res) ; j > 0 ; j-- )
      {
        printf("Ingrese el %dº tipo: ", j);
        res = comrpobarTipo();
        strcat(lineaTipos, res);
        if ( j > 1 ) strcat(lineaTipos, ", ");
      }
      strcat(lineaTipos, QUOTE);
    }
    return lineaTipos;
  }
  if ( i == 4 ) 
  {
    res = comprobarSexo();
    return res;
  }
  else scanf("%s", res);
  
  return res;
}

char* retornaRespuestaConPokemon(Pokemon* pokemon, PokedexData* data, int i)
{
  char* res = (char*)malloc(sizeof(char) * 20);

  if (i == 0) sprintf(res, "%d", pokemon->id);
  if (i == 1) strcpy(res, pokemon->pokedex->pos);
  if (i == 2)
  {
    strcpy(res, QUOTE);
    if (data == NULL) strcat(res, pokemon->pokedex->tipos);
    else strcat(res, data->tipos);
    strcat(res, QUOTE);
  }
  if (i == 3)
  {
    int value = pokemon->pc + (pokemon->pc * 0.5f);
    sprintf(res, "%d", value);
  }
  if (i == 4)
  {
    int value = pokemon->ps + (pokemon->ps * 0.25f);
    sprintf(res, "%d", value);
  }
  if (i == 5) strcpy(res, pokemon->sexo);
  if (i == 6) strcpy(res, pokemon->nombre);
  if (i == 7)
  {
    if (data == NULL) strcpy(res, "No tiene");
    else strcpy(res, data->pos);
  }
  if (i == 8)
  {
    if (data == NULL)
    {
      int value = rand() % (3000 - 2000 + 1) + 2000;
      sprintf(res, "%d", value);
    }
    else sprintf(res, "%d", data->numeroPokedex);
  }
  if (i == 9)
  {
    if (data == NULL) strcpy(res, pokemon->pokedex->region);
    else strcpy(res, data->region);
  }

  return res;
}
//linea que contiene los datos de un pokemon
char* crearLinea(Almacenamiento* almac, Pokemon* pokemon, PokedexData* data)
{
  char* nuevaLinea = (char*)malloc(sizeof(char) * 80);
  char* respuesta;

  if (pokemon == NULL)
  {
    Pokemon* aux = (Pokemon*)tailMap(almac->ids);
    char* ultimoId = (char*)malloc(sizeof(char) * 20);
    sprintf(ultimoId, "%d", aux->id + 1);
    strcpy(nuevaLinea, ultimoId);
    strcat(nuevaLinea, ",");

    for (int i = 0 ; i < 9 ; i++)
    {
      respuesta = retornaRespuesta(i);
      strcat(nuevaLinea, respuesta);
      strcat(nuevaLinea, ",");
    }
  }
  else
  {
    strcpy(nuevaLinea, retornaRespuestaConPokemon(pokemon, data, 0));
    strcat(nuevaLinea, ",");
    for (int i = 1 ; i < 10 ; i++)
    {
      respuesta = retornaRespuestaConPokemon(pokemon, data, i);
      strcat(nuevaLinea, respuesta);
      strcat(nuevaLinea, ",");
    }
  }

  return nuevaLinea;
}
//se inserta en el mapa un pokemon
void pokemonAtrapado(Pokedex* pokedex, Almacenamiento* almac)
{
  char* nuevaLinea = crearLinea(almac, NULL, NULL);
  insertarAMapas(pokedex,almac,nuevaLinea);
}

void evolucionPokemon(Pokedex* pokedex, Almacenamiento* almac)
{
  int id;
  printf("Indique id de pokemon a evolucionar: ");
  scanf("%i", &id);
  Pokemon* pokemon = (Pokemon*)searchMap(almac->ids, &id);
  if (pokemon != NULL) 
  {
    if (strcmp(pokemon->pokedex->pos, "No tiene") == 0 || pokemon->pokedex->pos == NULL)
    {
      printf("EL pokemon se encuentra en su maxima evolucion\n");
      return;
    }
    PokedexData* newData = (PokedexData*) searchMap(pokedex->nombre, pokemon->pokedex->pos);
    char* linea = crearLinea(almac, pokemon, newData);
    liberarPokemon(pokemon->id, pokedex, almac);
    insertarAMapas(pokedex, almac, linea);
  }
  else printf("El id ingresado no existe!\n");
}

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
//se busca por nombre y si no esta se envia mensaje y lee de nuevo
void buscarNombreEnPokedex(Pokedex* pokedex)
{
  Map* pokedexMap = pokedex->nombre;
  char* poke = (char*)malloc(sizeof(char));
  printf("Nombre del pokemon a buscar: ");
  scanf("%s", poke);
  printf("----------------------------------------------\n");
  printf("|%46c|\n", 32);
  printf("%7cPOKEMON INGRESADO: %s\n",32,poke );
  PokedexData* aux = (PokedexData*)searchMap(pokedexMap, poke);
  if (aux == NULL)
  {
    printf("|%7cPOKEMON NO ECONTRADO%19c|\n", 32, 32);
    printf("|%46c|\n", 32);
    printf("----------------------------------------------\n");
    return;
  } 
  printf("|%46c|\n", 32);
  printf("----------------------------------------------\n");
  mostrarMenuPokedex();
  printf("%s, %i, %s, %s, %s, %i, %s\n", aux->nombre, aux->existencia, aux->tipos, aux->prev, aux->pos, aux->numeroPokedex, aux->region);
}
//se busca el nombre y si no se encuentra da un mensaje
void buscarNombreEnAlmacenamiento(Almacenamiento* almac)
{
  char* poke = (char*)malloc(sizeof(char));
  printf("Nombre del pokemon a buscar: ");
  scanf("%s", poke);
  printf("-----------------------------------------------\n");
  printf("|%46c|\n", 32);
  printf("%7cPOKEMON INGRESADO: %s\n",32,poke );
  List* list = (List*)searchMap(almac->nombres, poke);
  if ( (list == NULL) || (listSize(list) == 0) )
  {
    printf("|%7cPOKEMON NO ECONTRADO%19c|\n", 32, 32);
    printf("|%46c|\n", 32);
    printf("-----------------------------------------------\n");
    return;
  }
  Pokemon* aux = (Pokemon*)firstList(list);
  printf("|%46c|\n", 32);
  printf("-----------------------------------------------\n");
  mostrarMenuAlmac();
  while ( aux != NULL )
  {
    printf("%i, %s, %i, %i, %s\n", aux->id, aux->nombre, aux->pc, aux->ps, aux->sexo);
    aux = (Pokemon*)nextList(list);
  }
}
//se muestran por pc
void mostrarxPC(Almacenamiento* almac)
{
  List* listPc = (List*) firstMap(almac->pcs);
  if ( listPc == NULL ) return;
  printf("PC,NOMBRE");
  while ( listPc != NULL ) 
  {
    Pokemon* poke = (Pokemon*) firstList(listPc);
    if ( poke != NULL ) printf("\n");
    while ( poke != NULL )
    {
      printf("%d,", poke->pc);
      printf("%s", poke->nombre);
      poke = (Pokemon*)nextList(listPc);
      if ( poke != NULL) printf("\n");
    }
    listPc = (List*) nextMap(almac->pcs);
    
  }
  printf("\n");
  printf("----------------------------------------------\n");
}
//se muestran todos los pokemones de la region
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
    
    mostrarMenuRegion();
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
//se busca por tipo  y retorna
void buscarPorTipo(Almacenamiento* almac)
{
  char* tipo = (char*)malloc(sizeof(char));
  printf("Ingrese el tipo a buscar: ");
  scanf("%s", tipo);

  List* listaTipos = (List*)searchMap(almac->tipos, tipo);
  if ( listaTipos == NULL )
  {
     printf("---------------------------------------------------------------------\n");
    printf("Este tipo no existe en tu almacenamiento!\n");
     printf("---------------------------------------------------------------------\n");
    return;
  }
  Pokemon* aux = (Pokemon*)firstList(listaTipos);
  while ( aux != NULL )
  {
    printf("%i, %s, %i, %i, %s\n", aux->id, aux->nombre, aux->pc, aux->ps, aux->sexo);
    aux = (Pokemon*)nextList(listaTipos);
  }
   printf("---------------------------------------------------------------------\n");
}