#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "archivos.h"
#include "Map.h"
#include "list.h"
#include "pokemon.h"

int main()
{
  FILE* archivo = LeerArchivo();

  Map* almac_nombre = createMap(is_equal_string);
  Map* pokedex_nombre = createMap(is_equal_string);

  Map* almac_id = createMap(is_equal_int);
  Map* pokedex_id = createMap(is_equal_int);

  List* pcs = createList();
  Map* almac_pc = createMap(is_equal_int);

  List* tipos = createList();
  Map* almac_tipo = createMap(is_equal_string);
  
  List* region =createList();
  Map* almac_region= createMap(is_equal_string);
  Map* pokedex_region= createMap(is_equal_string);
  
  int solicitud = 100;
  
  while( solicitud != 0 )
    {
      printf("Selecione una opcion\n");
      printOpciones();
      scanf("%d", &solicitud);
      while(solicitud < 0 || solicitud > 10 ) 
      {
          printf("Selecione una opcion correcta\n");
          printOpciones();
          scanf("%d", &solicitud);
      }
      //operaciones
      opciones(solicitud,pokemon);
    }
    
    return 0;
}