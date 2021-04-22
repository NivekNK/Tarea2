#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Map.h"
#include "list.h"
#include "pokemon.h"

typedef struct pokemon pokemon;
typedef struct pokedex pokedex;

struct pokemon
{
  int id;
  char* nombre;
  int pc;
  int ps;
  char* sexo;
  pokedex* pk;
};

struct pokedex
{
  int npokedex;
  int existencia;
  char* nombre;
  char* prev; //evolucion anterior
  char* pos;  //evolucion siguiente
  char* region;
};

int main ()
{
  
}