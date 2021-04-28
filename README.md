# Tarea2

void agregarBombero(List *bomberos, Dias *dias) 
{
  char* nuevaLinea = (char*) malloc(sizeof(char) * 65); 
  char* nombre = (char*) malloc(sizeof(char) * 20);
  char* apellido = (char*) malloc(sizeof(char) * 20);
  char* rut = (char*) malloc(sizeof(char)* 11);
  char* respuesta = (char*) malloc(sizeof(char) * 3);

  printf("Ingrese rut:");
  scanf("%s", rut);
  strcpy(nuevaLinea, rut);
  strcat(nuevaLinea, ",");

  printf("Ingrese nombre:");
  scanf("%s%s", nombre, apellido);
  strcat(nuevaLinea, nombre);
  strcat(nuevaLinea, " ");
  strcat(nuevaLinea, apellido);
  strcat(nuevaLinea, ",");


  printf("Ingrese disponibilidad:\n");
  respuesta = comprobarSINO("Lunes: ");
  strcat(nuevaLinea, respuesta);
  strcat(nuevaLinea, ",");

  respuesta = comprobarSINO("Martes: ");
  strcat(nuevaLinea, respuesta);
  strcat(nuevaLinea, ",");

  respuesta = comprobarSINO("Miercoles: "); 
  strcat(nuevaLinea, respuesta);
  strcat(nuevaLinea, ",");

  respuesta = comprobarSINO("Jueves: ");
  strcat(nuevaLinea, respuesta);
  strcat(nuevaLinea, ",");

  respuesta = comprobarSINO("Viernes: ");
  strcat(nuevaLinea, respuesta);
  strcat(nuevaLinea, ",");

  respuesta = comprobarSINO("Sabado: ");
  strcat(nuevaLinea, respuesta);
  strcat(nuevaLinea, ",");

  respuesta = comprobarSINO("Domingo: ");
  strcat(nuevaLinea, respuesta);
  strcat(nuevaLinea, ",");

  pushBack(bomberos, crearBombero(nuevaLinea, dias));
}