#include "hashmap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>



#define MAP_SIZE_X 20
#define MAP_SIZE_Y 20

// Estructura para el mapa
typedef struct {
  char data[MAP_SIZE_X][MAP_SIZE_Y];
} Mapa;


typedef struct {
  char *nombre;      // nombre del jugador
  int prota;         // 1 si es el prota, 0 si es un enemigo
  long pH;           // puntos de vida
  long psMax;        // puntos de vida maximos
  int comp;          // bonificador de competencia
  int fuerza;        // fuerza del jugador
  int bonifFuerza;   // bonificacion de fuerza del jugador
  int destreza;      // destreza del jugador
  int bonifDestreza; // bonificacion de destreza del jugador
  long exp;          // experiencia
  HashMap *items;    // hashmap de objetos
  HashMap *equipo;   // hashmap de equipo
  int turno;         // turno en el combate
  int armorClass;    // clase de armadura, numero a superar para que le peguen
  int cubierto;      // 1 si esta cubierto, 0 si no
  int ventaja;       // 1 si tiene ventaja, 0 si no
  int desventaja;    // 1 si tiene desventaja, 0 si no
  int desarmadoPrin; // 1 si esta desarmado, 0 si no
  int desarmadoSec;  // 1 si esta desarmado, 0 si no
  short ultAcc;      // Para evitar bucle de cubrirse
} Jugador;

typedef struct {
  char *nombre; // nombre del objeto
  int atk;      // ataque del objeto
  int def;      // defensa del objeto
  int curacion; // curacion del objeto
  // int cantidad; // cantidad de objetos
} Objeto;



//
// FUNCIONES AÑADIDAS EN ORDEN TOPOLOGICO:
// void clear_console()
// bool checkFileForEstado(const char* filename, char* estado)
// void st_combate()
// void st_hoguera()
// void st_mapa()
// void load_map_from_file(char* file_path, Mapa* mapa)
// void display_map(Mapa* mapa, int player_x, int player_y)
// void save_map_to_file(char* file_path, Mapa* mapa)
// void process_actions(char* file_path, Mapa* mapa, int* player_x, int* player_y)
// void PrintArchivo(const char* buffer)
//
//



void clear_console() {
  system("clear");
  system("cls"); // Utilizar "cls" en Windows
}

bool checkFileForEstado(const char* filename,char* estado) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return false;
    }

    char word[100];
    while (fscanf(file, "%s", word) != EOF) {
        if (strcmp(word, estado) == 0) {
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}



void clear_console() {
  system("clear");
  system("cls"); // Utilizar "cls" en Windows
}

bool checkFileForEstado(const char* filename,char* estado) {
    FILE* file = fopen(filename, "r");
    char buffer[999];
    if (file == NULL) {
        sprintf(buffer,"Error opening the file.\n");
      printArchivo(buffer);
        return false;
    }

    char word[100];
    while (fscanf(file, "%s", word) != EOF) {
        if (strcmp(word, estado) == 0) {
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}



void st_combate() {
  FILE *file = fopen("estado.txt", "w");
  char buffer[999];
  if (file == NULL) {
    sprintf(buffer,"Error al abrir el archivo estado.txt.");
    printArchivo(buffer);
    exit(1);
  }

  fprintf(file, "combate");
  fclose(file);
}

void st_hoguera() {
  FILE *file = fopen("estado.txt", "w");
  char buffer[999];
  if (file == NULL) {
    sprintf(buffer, "Error al abrir el archivo estado.txt.");
    printArchivo(buffer);
    exit(1);
  }

  fprintf(file, "bonfire");
  fclose(file);
}
void st_mapa() {
  FILE *file = fopen("estado.txt", "w");
  char buffer[999];
  if (file == NULL) {
    sprintf(buffer, "Error al abrir el archivo estado.txt.");
    printArchivo(buffer);
    exit(1);
  }

  fprintf(file, "mapa");
  fclose(file);
}


void load_map_from_file(char *file_path, Mapa *mapa) {
  FILE *file = fopen(file_path, "r");
  char buffer[999];
  if (file == NULL) {
    sprintf(buffer, "Error al abrir el archivo.");
    printArchivo(buffer);
    exit(1);
  }

  int i, j;
  for (i = 0; i < MAP_SIZE_X; i++) {
    for (j = 0; j < MAP_SIZE_Y; j++) {
      fscanf(file, " %c,", &(mapa->data[i][j]));  // Leer el carácter y la coma
    }
  }

  fclose(file);
}

void display_map(Mapa *mapa, int player_x, int player_y) {
  clear_console();

  int i, j;
  for (i = 0; i < MAP_SIZE_X; i++) {
    for (j = 0; j < MAP_SIZE_Y; j++) {
      if (i == player_y && j == player_x) {
        printf("@");          // Carácter del jugador
        mapa->data[i][j] = '@'; // Guardar la posición del jugador en el mapa
      } else {
        printf("%c", mapa->data[i][j]);
      }
    }
    printf("\n");
  }
}

void save_map_to_file(char *file_path, Mapa *mapa) {
  FILE *file = fopen(file_path, "w");
  char buffer[999];
  if (file == NULL) {
    sprintf(buffer, "Error al abrir el archivo.");
    printArchivo(buffer);
    exit(1);
  }

  int i, j;
  for (i = 0; i < MAP_SIZE_X; i++) {
    for (j = 0; j < MAP_SIZE_Y; j++) {
      fprintf(file, "%c,", mapa->data[i][j]);
    }
    fprintf(file, "\n");
  }

  fclose(file);
}



void process_actions(char *file_path, Mapa *mapa, int *player_x, int *player_y) {
  FILE *file = fopen(file_path, "r");
  char buffer[999];
  if (file == NULL) {
    sprintf(buffer, "Error al abrir el archivo.");
    printArchivo(buffer);
    exit(1);
  }

  char action[5];
  fgets(action, 5, file);

  if (strlen(action) > 0) {
    sprintf(buffer, "Esperando respuesta de archivo %s\n", file_path);
    sprintf(buffer, "Acción hecha: %c\n", action[0]);

    printArchivo(buffer);

    switch (action[0]) {
      case 'W':
      case 'w':
        if (*player_y > 0 && mapa->data[*player_y - 1][*player_x] != '#') {
                        if (mapa->data[*player_y - 1][*player_x] == 'd') {
      // Cambiar al siguiente mapa
      load_map_from_file("mapa2.csv", mapa);
      
      break;
    }
     if (mapa->data[*player_y - 1][*player_x] == 'e') {
      mapa->data[*player_y][*player_x] = '.'; // Restaurar el espacio vacío anterior
          (*player_y)--;
      // combate
      st_combate();
     
      

      break;
    }
    if (mapa->data[*player_y - 1][*player_x] == 'b') {
      mapa->data[*player_y][*player_x] = '.'; // Restaurar el espacio vacío anterior
          (*player_y)--;
      // hoguera
      st_hoguera();
     
      

      break;
    }
          mapa->data[*player_y][*player_x] = '.'; // Restaurar el espacio vacío anterior
          (*player_y)--;
        }
        break;
      case 'A':
      case 'a':
        if (*player_x > 0 && mapa->data[*player_y][*player_x - 1] != '#') {
               if (mapa->data[*player_y][*player_x - 1] == 'd') {
      // Cambiar al siguiente mapa
      load_map_from_file("mapa2.csv", mapa);
 
      break;
    }
     if (mapa->data[*player_y ][*player_x - 1] == 'e') {
       mapa->data[*player_y][*player_x] = '.'; // Restaurar el espacio vacío anterior
          (*player_x)--;
      // combate
      st_combate();
     
    
      break;
    }
     if (mapa->data[*player_y ][*player_x - 1] == 'b') {
       mapa->data[*player_y][*player_x] = '.'; // Restaurar el espacio vacío anterior
          (*player_x)--;
      // hoguera
      st_hoguera();
     
    
      break;
    }
          mapa->data[*player_y][*player_x] = '.'; // Restaurar el espacio vacío anterior
          (*player_x)--;
        }
        break;
      case 'S':
      case 's':
        if (*player_y < MAP_SIZE_Y - 1 && mapa->data[*player_y + 1][*player_x] != '#') {
          if (mapa->data[*player_y+ 1][*player_x] == 'd') {
      // Cambiar al siguiente mapa
      load_map_from_file("mapa2.csv", mapa);
 
      break;
    }
     if (mapa->data[*player_y + 1][*player_x] == 'e') {
       mapa->data[*player_y][*player_x] = '.'; // Restaurar el espacio vacío anterior
          (*player_y)++;
      // combate
      st_combate();
    
    
      break;
    }
     if (mapa->data[*player_y + 1][*player_x] == 'b') {
       mapa->data[*player_y][*player_x] = '.'; // Restaurar el espacio vacío anterior
          (*player_y)++;
      // hoguera
      st_hoguera();
    
    
      break;
    }
          mapa->data[*player_y][*player_x] = '.'; // Restaurar el espacio vacío anterior
          (*player_y)++;
        }
        break;
      case 'D':
      case 'd':
        if (*player_x < MAP_SIZE_X - 1 && mapa->data[*player_y][*player_x + 1] != '#') {
          if (mapa->data[*player_y][*player_x + 1] == 'd') {
      // Cambiar al siguiente mapa
      load_map_from_file("mapa2.csv", mapa);

      break;
    }
     if (mapa->data[*player_y ][*player_x + 1] == 'e') {
       mapa->data[*player_y][*player_x] = '.'; // Restaurar el espacio vacío anterior
          (*player_x)++;
      // combate
      st_combate();
      
    
      break;
    }
    if (mapa->data[*player_y ][*player_x + 1] == 'e') {
       mapa->data[*player_y][*player_x] = '.'; // Restaurar el espacio vacío anterior
          (*player_x)++;
      // hoguera
      st_hoguera();
      
    
      break;
    }
          mapa->data[*player_y][*player_x] = '.'; // Restaurar el espacio vacío anterior
          (*player_x)++;
        }
        break;
      default:
        break;
    }
        
  }

  fclose(file);

  // Vaciar el archivo
  file = fopen(file_path, "w");
  if (file == NULL) {
    sprintf(buffer, "Error al abrir el archivo.");
    printArchivo(buffer);
    exit(1);
  }
  fclose(file);
}

void PrintArchivo(const char* buffer)
{
    // Abrir el archivo en modo de escritura (borrar contenido existente)
    FILE* archivo = fopen("texto.txt", "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    // Guardar el contenido del buffer en el archivo
    fprintf(archivo, "%s", buffer);

    // Cerrar el archivo
    fclose(archivo);

    // Imprimir el contenido del buffer en la consola
    printf("%s", buffer);
    sleep(1.5);
}







// Cambia el equipo del jugador
void cambioEquipo(Jugador *pj, HashMap *objetos) {
  printf("¿Que objeto desea equipar?\n");
  printf("1. Espada principal\n");
  printf("2. Espada secundaria\n");
  printf("3. Escudo\n");
  printf("4. Armadura\n");
  int opc;
  char nombreObj[30];
  int *new;
  Objeto *espSecEquip;
  scanf("%d", &opc);
  getchar();
  switch (opc) {
  case 1:

    printf("¿Que espada desea equipar?\n");
    scanf("%[^\n]", nombreObj);
    getchar();
    new = valueRet(searchMap(pj->items, nombreObj));
    // En caso de no tenerla no la puede equipar
    if (!new) {
      printf("No tienes ese objeto\n");
      getchar();
      break;
    }
    Objeto *itemBase = valueRet(searchMap(objetos, nombreObj));
    // En caso de tenerla pero no tener ataque no la puede equipar
    if (itemBase->atk == 0) {
      printf("No puedes equipar eso\n");
      getchar();
      break;
    } 
    else if ((*new) == 0) {
      printf("Solo tienes el que ya esta equipado\n");
      getchar();
      break;
    }
    // se elimina la espada principal y se equipa la nueva
    Objeto *espEquip = valueRet(searchMap(pj->equipo, "Espada"));
    int *espEquipCant = valueRet(searchMap(pj->items, espEquip->nombre));
    (*espEquipCant)++;
    eraseMap(pj->equipo, "Espada");
    insertMap(pj->equipo, "Espada", itemBase);
    (*new)--;
    printf("Se ha equipado la espada\n");
    getchar();
    break;

  case 2:
    // Comprueba que no tenga equipada una segunda espada
    if (searchMap(pj->equipo, "Espada Secundaria")) {
      printf("Ya tienes equipadas 2 espadas\n");
      printf("Deseas cambiar :\n");
      espSecEquip = valueRet(searchMap(pj->equipo, "Espada Secundaria"));
      printf("%s\nDaño : %d\n", espSecEquip->nombre, espSecEquip->atk);
      printf("1. Si\n");
      printf("2. No\n");
      int opc;
      scanf("%d", &opc);
      getchar();
      if (opc == 2) {
        break;
      }
    }
    // En caso de no tenerla la intenta equipar

    printf("¿Que espada desea equipar?\n");
    scanf("%[^\n]", nombreObj);
    getchar();
    new = valueRet(searchMap(pj->items, nombreObj));

    // Si no tiene la espada en el inventario no la puede equipar
    if (!new) {
      printf("No tienes ese objeto\n");
      getchar();
      break;
    }
    Objeto *nuevaSec = valueRet(searchMap(objetos, nombreObj));
    // En caso de tenerla pero no tener ataque no la puede equipar
    if (nuevaSec->atk == 0) {
      printf("No puedes equipar eso\n");
      getchar();
      break;
    } 
    else if ((*new) == 0) {
      printf("Solo tienes el que ya esta equipado\n");
      getchar();
      break;
    }
    // Si la tiene la equipa y desequipa el escudo
    if (searchMap(pj->equipo, "Escudo")) {
      pj->armorClass -= 2;
      eraseMap(pj->equipo, "Escudo");
      int *escudo = valueRet(searchMap(pj->items, "Escudo"));
      (*escudo)++;
    }
    espSecEquip = valueRet(searchMap(pj->equipo, "Espada Secundaria"));
    if (espSecEquip) {
      int *espSecEquipCant = valueRet(searchMap(pj->items, espSecEquip->nombre));
      (*espSecEquipCant)++;
    }
    eraseMap(pj->equipo, "Espada Secundaria");
    insertMap(pj->equipo, "Espada Secundaria", nuevaSec);
    pj->desarmadoSec = 0;
    (*new)--;

    printf("Se ha equipado la espada\n");

    getchar();
    break;

  case 3:
    // Comprueba que no tenga equipado un escudo
    if (searchMap(pj->equipo, "Escudo")) {
      printf("Ya tienes equipado un escudo\n");
      getchar();
      break;
    }
    // En caso de no tenerlo lo intenta equipar
    else {
      new = valueRet(searchMap(pj->items, "Escudo"));
      // Si no tiene el escudo en el inventario no lo puede equipar
      if (!new) {
        printf("No tienes ese objeto\n");
        getchar();
        break;
      }
      // Si lo tiene lo equipa y desequipa la espada secundaria
      espSecEquip = valueRet(searchMap(pj->equipo, "Espada Secundaria"));
      if (espSecEquip) {
        int *espSecEquipCant = valueRet(searchMap(pj->items, espSecEquip->nombre));
        (*espSecEquipCant)++;
      }
      eraseMap(pj->equipo, "Espada Secundaria");
      insertMap(pj->equipo, "Escudo", valueRet(searchMap(objetos, "Escudo")));
      pj->armorClass += 2;

      printf("Se ha equipado el escudo\n");
    }
    getchar();
    break;

  case 4:
    printf("¿Que armadura desea equipar?\n");
    scanf("%[^\n]", nombreObj);
    getchar();
    new = valueRet(searchMap(pj->items, nombreObj));
    if (!new) {
      printf("No tienes ese objeto\n");
      getchar();
      break;
    }
    Objeto *armor = valueRet(searchMap(objetos, nombreObj));
    if (armor->def == 0 || strcmp("Escudo", nombreObj) == 0) {
      printf("No puedes equipar eso\n");
      getchar();
      break;
    }
    eraseMap(pj->equipo, "Armadura");
    insertMap(pj->equipo, "Armadura", armor);
    if (armor->curacion == 0) {
      pj->armorClass = 10 + armor->def + pj->bonifDestreza;
    }
    else {
      pj->armorClass = 10 + armor->def + pj->bonifFuerza;
    }
    printf("Se ha equipado la armadura\n");
    printf("Tu clase de armadura es %d\n", pj->armorClass);
    getchar();
    break;

  default:
    printf("Opcion no valida\n");
    getchar();
    break;
  }
}


// Imprime los objetos del jugador
void stats(Jugador *pj, HashMap *objetos) {
  // Imprime los stats del jugador
  printf("Nombre : %s\n", pj->nombre);
  printf("Experiencia para el siguiente nivel : %ld\n",
         pj->psMax * 10 - pj->exp);
  printf("Puntos de vida: %ld\n", pj->pH);
  printf("Puntos de armadura: %d\n", pj->armorClass);
  printf("Bonificador de competencia: %d\n", pj->comp);
  printf("Fuerza: %d\n", pj->fuerza);
  printf("Bonificador de fuerza: %d\n", pj->bonifFuerza);
  printf("Destreza: %d\n", pj->destreza);
  printf("Bonificador de destreza: %d\n", pj->bonifDestreza);

  // Imprime los objetos del jugador
  printf("Objetos:\n");
  Pair *aux = firstMap(pj->items);
  while (aux) {
    Objeto *obj = valueRet(searchMap(objetos, aux->key));
    char *copiaName = (char *)malloc(30 * sizeof(char));
    strcpy(copiaName, obj->nombre);
    char *primPala = strtok(copiaName, " ");
    printf("-%s\n", obj->nombre);
    int *cant = aux->value;

    // Si es pocion muestra cuantas tiene
    if (strcmp(primPala, "Pocion") == 0) {
      printf("    Cantidad: %d\n", *cant);
    }
    // Si es espada muestra el ataque
    else if (obj->atk > 0) {
      printf("    Ataque: %d\n", obj->atk);
      printf("    Cantidad sin equipar: %d\n", *cant);
    }
    // Si es armadura muestra el defensa
    else if (strcmp(primPala, "Armadura") == 0) {
      printf("    Defensa: %d\n", obj->def);
    }
    aux = nextMap(pj->items);
  }

  // Imprime el equipo del jugador
  printf("Equipo:\n");
  aux = firstMap(pj->equipo);
  while (aux) {
    Objeto *obj = valueRet(aux);
    char *copiaName = (char *)malloc(30 * sizeof(char));
    strcpy(copiaName, obj->nombre);
    char *primPala = strtok(copiaName, " ");
    printf("-%s\n", obj->nombre);

    // Si es espada muestra el ataque
    if (strcmp(aux->key, "Espada") == 0 ||
        strcmp(aux->key, "Espada Secundaria") == 0) {
      printf("    Ataque: %d\n", obj->atk);
    }
    // Si es armadura muestra el defensa
    else if (strcmp(primPala, "Armadura") == 0) {
      printf("    Defensa: %d\n", obj->def);
    }
    aux = nextMap(pj->equipo);
  }
  getchar();
}


void vaciarArchivo(const char* nombreArchivo) {
  FILE* archivo = fopen(nombreArchivo, "w");
  if (archivo == NULL) {
    printf("Error al abrir el archivo.\n");
    return;  // Error al abrir el archivo
  }
  fclose(archivo);
}
int leerUltimaLineaArchivo(const char* nombreArchivo) {
  FILE* archivo = fopen(nombreArchivo, "r");
  if (archivo == NULL) {
    printf("Error al abrir el archivo.\n");
    return -1;  // Error al abrir el archivo
  }

  int ultimoEntero = 0;
  int enteroActual;
  while (fscanf(archivo, "%d", &enteroActual) == 1) {
    ultimoEntero = enteroActual;
  }
  fclose(archivo);
  return ultimoEntero;
}


int obtenerSeleccion() {
  int seleccion = -1;
  int tiempoEspera = 3000000;  // 3,5 segundos
  vaciarArchivo("accion.txt");
  while (seleccion == -1) {
    usleep(tiempoEspera);
    seleccion = leerUltimaLineaArchivo("accion.txt");
  }

  vaciarArchivo("accion.txt");

  return seleccion;
}

// Imprime las opciones de accion del jugador y devuelve la opcion elegida
int printOp(int objSec) {
  printf("¿Que desea hacer?\n");
  // Si tiene al menos una espada imprime los mensajes correspondientes
  if (objSec != 4) {
    printf("1. Atacar\n");
    if (objSec == 0) {
      printf("2. Accion no disponible\n");
    } else if (objSec == 1) {
      printf("2. Ataque extra\n");
    } else if (objSec == 2) {
      printf("2. Protegerse\n");
    } else if (objSec == 3) {
      printf("2. Recoger una espada\n");
    }
    printf("3. Curarse\n");
    printf("4. Ver stats\n");
    //NUEVAAAAAAAA
    int opc=obtenerSeleccion();
    //Vieja
    //int opc;
    //scanf("%d", &opc);
    //getchar();
    
    return opc;
  }
  // Si no tiene espadas pierde las opciones de atacar
  else {
    printf("1. Recoger una espada\n");
    printf("2. Curarse\n");
    printf("3. Ver stats\n");
    //Nuevaa
    int opc=obtenerSeleccion();
    //Vieja
    //int opc;
    //scanf("%d", &opc);
    //getchar();
    
    // Se modifican las opciones para que se correspondan con las opciones del
    // jugador
    if (opc == 1) {
      return 2;
    } else if (opc == 2) {
      return 3;
    } else {
      return 4;
    }
  }
}


// Realiza la accion de ataque del jugador o enemigo
void ataque(Jugador *pj, Jugador *enemigo, char *objeto) {
  int acc = rand() % 20 + 1;
  
  if (pj->ventaja == 1 && pj->desventaja == 0 ||
      pj->ventaja == 1 && enemigo->cubierto == 0 ||
      enemigo->desventaja && enemigo->cubierto == 0) {
    int acc2 = rand() % 20 + 1;
    printf("Primer dado : %d\n", acc);
    printf("Segundo dado : %d\n", acc2);
    if (acc < acc2) {
      acc = acc2;
    }
    printf("Dado definitivo : %d\n", acc);
  }
  else if (enemigo->cubierto == 1 && pj->ventaja == 0 ||
           pj->desventaja == 1 && pj->ventaja == 0) {
    int acc2 = rand() % 20 + 1;
    printf("Primer dado : %d\n", acc);
    printf("Segundo dado : %d\n", acc2);
    if (acc > acc2) {
      acc = acc2;
    }
    printf("Dado definitivo : %d\n", acc);
  }
  else {
    printf("Lanzamiento del dado : %d\n", acc);
  }

  acc += pj->comp;
  printf("Numero de entrada : %d\n", acc);
  // Ataque exitoso
  if (acc >= enemigo->armorClass) {
    int damage;
    Objeto *espada = valueRet(searchMap(pj->equipo, objeto));
    int dadoArma = rand() % espada->atk + 1;
    // Ataque critico
    if ((acc - pj->comp) == 20) {
      if (strcmp(objeto, "Espada") == 0) {
        damage = espada->atk + dadoArma + pj->bonifFuerza;
      } else {
        damage = espada->atk + dadoArma;
      }
    }
    // Ataque normal
    else {
      if (strcmp(objeto, "Espada") == 0) {
        damage = dadoArma + pj->bonifFuerza;
      } else {
        damage = dadoArma;
      }
    }
    enemigo->pH -= damage;
    // Imprime el resultado del ataque
    if (!pj->prota) {
      if ((acc - pj->comp) == 20) {
        printf("El enemigo te ha hecho %d de daño (critico)\n", damage);
      } else {
        printf("El enemigo te ha hecho %d de daño\n", damage);
      }
    }
    // Imprime el resultado del ataque
    else {
      if (strcmp(objeto, "Espada") == 0) {
        if ((acc - pj->comp) == 20) {
          printf("Has hecho %d (%d + %d + %d) de daño (critico)\n",damage, espada->atk, dadoArma, pj->bonifFuerza);
        } 
        else {
          printf("Has hecho %d (%d + %d) de daño\n", damage, dadoArma, pj->bonifFuerza);
        }
      }
      else {
        if ((acc - pj->comp) == 20) {
          printf("Has hecho %d (%d + %d) de daño (critico)\n", damage, espada->atk, dadoArma);
        } 
        else {
          printf("Has hecho %d de daño\n", dadoArma);
        }
      }
    }
  }
  // Fallo critico, se le cae la espada
  else if ((acc - pj->comp) == 1) {
    if (!pj->prota) {
      printf("El enemigo ha fallado\n");
      pj->desventaja = 1;
      if (strcmp(objeto, "Espada") == 0) {
        pj->desarmadoPrin = 1;
        printf("El enemigo ha perdido su espada principal\n");
      } else {
        pj->desarmadoSec = 1;
        printf("El enemigo ha perdido su espada secundaria\n");
      }
    } else {
      printf("Has fallado\n");
      pj->desventaja = 1;
      if (strcmp(objeto, "Espada") == 0) {
        pj->desarmadoPrin = 1;
        printf("Has perdido tu espada principal\n");
      } else {
        pj->desarmadoSec = 1;
        printf("Has perdido tu espada secundaria\n");
      }
    }
  }
  // Ataque fallido
  else {
    if (enemigo->cubierto) {
      pj->desventaja = 1;
      enemigo->ventaja = 1;
    }
    if (!pj->prota) {
      printf("El enemigo ha fallado\n");
    } else {
      printf("Has fallado\n");
    }
  }
}

// Curarse con pocion durante el combate
int usarPocion(Jugador *pj, HashMap *objetos) {
  // Comprueba que tenga pociones
  int *pocion = valueRet(searchMap(pj->items, "Pocion"));
  if (!pocion) {
    if (pj->prota) {
      printf("No tienes pociones\n");
    }
    return 0;
  }
  // Se cura
  else {
    Objeto *pocionReal = valueRet(searchMap(objetos, "Pocion"));
    int curacion = pj->psMax - pj->pH;
    pj->pH += pocionReal->curacion;
    if (pj->pH > pj->psMax) {
      pj->pH = pj->psMax;
      if (pj->prota) {
        printf("Te has curado %d puntos de vida\n", curacion);
      } else {
        printf("El enemigo se ha curado\n");
      }
    } else {
      if (pj->prota) {
        printf("Te has curado %d puntos de vida\n", pocionReal->curacion);
      } else {
        printf("El enemigo se ha curado\n");
      }
    }

    (*pocion)--;
    if ((*pocion) == 0) {
      eraseMap(pj->items, "Pocion");
      if (pj->prota) {
        printf("Se han acabado las pociones\n");
      }
    }
  }
  return 1;
}

// Modifica la accion extra si el jugador esta desarmado
int detCaida(Jugador *pj, int accExtra) {
  // Si esta desarmado pero tiene 2 espadas
  if (pj->desarmadoPrin == 1 && accExtra == 1) {
    if (pj->prota)
      printf("No puedes atacar con la espada principal\n");
    accExtra = 3;
  }
  // Si esta desarmado pero tiene 2 espadas
  else if (pj->desarmadoSec == 1 && accExtra == 1) {
    if (pj->prota)
      printf("No puedes atacar con la espada secundaria\n");
    accExtra = 3;
  }
  // Si esta desarmado de ambas manos o tiene solo una espada que se le cayo y
  // un escudo
  else if (pj->desarmadoPrin == 1 && accExtra == 0 ||
           pj->desarmadoPrin == 1 && pj->desarmadoSec == 1 ||
           pj->desarmadoPrin == 1 && accExtra == 2) {
    printf("No puedes atacar\n");
    accExtra = 4;
  }
  return accExtra;
}

// Realiza el turno del jugador o enemigo
void turnoPj(Jugador *pj, Jugador *enemigo, HashMap *objetos) {
  int opc;

  // Define que accion extra tiene el personaje
  int accExtra = 0;
  Pair *secundario = searchMap(pj->equipo, "Espada Secundaria");
  if (!secundario || (secundario && secundario->key == NULL)) {
    secundario = searchMap(pj->equipo, "Escudo");
  }
  if (!secundario || (secundario && secundario->key == NULL)) {
    secundario = (Pair *)malloc(sizeof(Pair));
    secundario->key = (char *)malloc(sizeof(char) * 5);
    secundario->key = "Nada";
  }
  if (strcmp(secundario->key, "Espada Secundaria") == 0) {
    accExtra = 1;
  } else if (strcmp(secundario->key, "Escudo") == 0) {
    accExtra = 2;
  }
  // Variable para que el enemigo solo diga una vez su vida
  int mostVidaEne = 0;
  int accOriginal = accExtra;
  // Bucle para que el jugador elija una accion valida
  while (true) {
    accExtra = detCaida(pj, accExtra);

    // Si es jugador pide la accion, si es enemigo la elige aleatoriamente
    if (pj->prota) {
      printf("Salud : %ld\n", pj->pH);
      opc = printOp(accExtra);
    }
    // Da indicios de la vida del enemigo
    else {
      float porcVida = pj->pH * 1.0 / pj->psMax;
      if (!mostVidaEne) {
        mostVidaEne = 1;
        if (porcVida == 1) {
          printf("El enemigo se ve en perfecto estado\n");
        } else if (porcVida >= 0.75) {
          printf("El enemigo no parece tener heridas\n");
        } else if (porcVida >= 0.5) {
          printf("El enemigo se ve adolorido\n");
        } else if (porcVida >= 0.25) {
          printf("El enemigo se ve bastante herido\n");
        } else {
          printf("No sabes como se mantiene en pie\n");
        }
        getchar();
      }
      opc = rand() % 3 + 1;
      if (enemigo->desventaja == 1 || pj->ventaja == 1) {
        if (porcVida >= 0.5) 
          opc = 1;
        else {
          opc = rand() % 3 + 1;
          if (opc == 2)
            opc = 1;
        }
      }
      else if (porcVida <= 0.25) {
        if (valueRet(searchMap(pj->items, "Pocion"))) {
          opc = rand() % 3 + 1;
          if (opc == 2)
            opc = 3;
        }
      }
      // Si sale opcion extra pero no tiene nada equipado elige ataque
      if (opc == 2 && accExtra == 0) {
        opc = 1;
      } 
      else if (opc == 1 && accExtra == 4) {
        opc = 2;
      }
      else if (opc == 3 && pj->pH == pj->psMax) {
        opc = 1;
      }
      if (opc == 1 && accExtra == 1) {
        opc = 2;
      }

      if (pj->ultAcc == 2 && opc == 2 && accExtra == 2)
        pj->ultAcc = 4;
      else if (pj->ultAcc == 4 && opc == 2) {
        while (opc == 2) {
          opc = rand() % 3 + 1;
        }
        pj->ultAcc = opc;
      }
      else {
        pj->ultAcc = opc;
      }
    }

    // Realiza la accion
    switch (opc) {
    case 1:
      if (!pj->desarmadoPrin) {
        ataque(pj, enemigo, "Espada");
      } else if (pj->desarmadoPrin && !pj->desarmadoSec && accOriginal == 1) {
        ataque(pj, enemigo, "Espada Secundaria");
      } else {
        printf("No puedes atacar\n");
        break;
      }
      getchar();
      return;

    case 2:
      // No tiene equipo para realizar acciones extras
      if (accExtra == 0) {
        printf("No disponible\n");
        getchar();
        break;
      }
      // Ataque extra
      else if (accExtra == 1) {
        if (pj->prota == 1) {
          printf("Primer ataque\n");
          ataque(pj, enemigo, "Espada");
          printf("Segundo ataque\n");
          ataque(pj, enemigo, "Espada Secundaria");
        } else {
          ataque(pj, enemigo, "Espada");
          ataque(pj, enemigo, "Espada Secundaria");
        }
      }
      // Protegerse
      else if (accExtra == 2) {
        pj->armorClass += 2;
        pj->cubierto = 1;
        if (pj->prota)
          printf("Te has cubierto\n");
        else
          printf("El enemigo se ha cubierto\n");
      }
      // Recoger una espada
      else {
        if (pj->prota)
          printf("Recoges el arma del suelo\n");
        else
          printf("El enemigo recogio una espada\n");
        if (pj->desarmadoPrin == 1) {
          pj->desarmadoPrin = 0;
        } else {
          pj->desarmadoSec = 0;
        }
      }
      getchar();
      return;

    case 3:
      // No se puede curar
      if (usarPocion(pj, objetos) == 0) {
        break;
      }
      // Se cura
      else {
        getchar();
        return;
      }

    case 4:
      stats(pj, objetos);
      break;

    default:
      printf("Opcion no valida\n");
      break;
    }
  }
}

// Realiza el combate entre el jugador y el enemigo mientras ambos tengan vida
void combate(Jugador *pj, Jugador *enemigo, HashMap *objetos) {
  int iniciativaPj = rand() % 20 + 1;
  int iniciativaEnemigo = rand() % 20 + 1;
  if (iniciativaPj > iniciativaEnemigo) {
    pj->turno = 1;
    enemigo->turno = 0;
  } else {
    pj->turno = 0;
    enemigo->turno = 1;
  }
  while (pj->pH > 0 && enemigo->pH > 0) {
    if (pj->turno == 1) {
      printf("Es tu turno\n");
      
      // Si esta cubierto lo quita
      if (pj->cubierto == 1) {
        pj->cubierto = 0;
        pj->armorClass -= 2;
      }
      turnoPj(pj, enemigo, objetos);
      // Si tiene desventaja la quita
      if (pj->desventaja == 1 && pj->desarmadoPrin == 0 &&
          pj->desarmadoSec == 0) {
        pj->desventaja = 0;
      }
      pj->ventaja = 0;
      pj->turno = 0;
      enemigo->turno = 1;
    } else {
      printf("Turno del enemigo\n");
      
      // Si esta cubierto lo quita
      if (enemigo->cubierto == 1) {
        enemigo->cubierto = 0;
        enemigo->armorClass -= 2;
      }
      turnoPj(enemigo, pj, objetos);
      // Si tiene desventaja la quita
      if (enemigo->desventaja == 1 && enemigo->desarmadoPrin == 0 &&
          enemigo->desarmadoSec == 0) {
        enemigo->desventaja = 0;
      }
      enemigo->ventaja = 0;
      pj->turno = 1;
      enemigo->turno = 0;
    }
  }
}

// Realiza la subida de nivel del jugador
void subidaNivel(Jugador *pj) {
  printf("Has subido de nivel\n");
  pj->exp -= pj->psMax * 10;
  pj->psMax += 5;
  pj->pH = pj->psMax;
  pj->comp = 2 + (pj->psMax - 15) / 5;
  printf("Tus puntos de vida maximos son %ld\n", pj->psMax);
  printf("Tu bonificador de competencia es %d\n", pj->comp);
  getchar();
  Objeto *armadura;
  if (pj->fuerza < 20 && pj->destreza < 20) {
    if (pj->psMax % 2 == 0) {
      printf("Puedes sumarle dos a una caracteristica\n");
      printf("1. Fuerza: actualmente = %d\n", pj->fuerza);
      printf("2. Destreza: actualmente = %d\n", pj->destreza);
      int opc;
      scanf("%d", &opc);
      getchar();

      // Suma 2 a la fuerza
      if (opc == 1) {
        pj->fuerza += 2;
        if (pj->fuerza >= 20) {
          pj->fuerza = 20;
          printf("Has llegado al maximo de fuerza\n");
        }
        pj->bonifFuerza = (pj->fuerza - 10) / 2;
        printf("Tu bonificador de fuerza es %d\n", pj->bonifFuerza);
      }
      // Suma 2 a la destreza
      else {
        pj->destreza += 2;
        if (pj->destreza >= 20) {
          pj->destreza = 20;
          printf("Has llegado al maximo de destreza\n");
        }
        pj->bonifDestreza = (pj->destreza - 10) / 2;
        armadura = valueRet(searchMap(pj->equipo, "Armadura"));
        if (armadura) {
          pj->armorClass = 10 + armadura->def + pj->bonifDestreza;
        } else {
          pj->armorClass = 10 + pj->bonifDestreza;
        }
        printf("Tu bonificador de destreza es %d\n", pj->bonifDestreza);
      }
    }
  } else if (pj->fuerza < 20) {
    printf("Tu destreza esta al maximo\n");
    printf("Se suma automaticamente a la fuerza\n");
    pj->fuerza += 2;
    if (pj->fuerza >= 20) {
      pj->fuerza = 20;
      printf("Has llegado al maximo de fuerza\n");
    }
    pj->bonifFuerza = (pj->fuerza - 10) / 2;
    printf("Tu bonificador de fuerza es %d\n", pj->bonifFuerza);
  } else if (pj->destreza < 20) {
    printf("Tu fuerza esta al maximo\n");
    printf("Se suma automaticamente a la destreza\n");
    pj->destreza += 2;
    if (pj->destreza >= 20) {
      pj->destreza = 20;
      printf("Has llegado al maximo de destreza\n");
    }
    pj->bonifDestreza = (pj->destreza - 10) / 2;
    armadura = valueRet(searchMap(pj->equipo, "Armadura"));
    if (armadura) {
      pj->armorClass = 10 + armadura->def + pj->bonifDestreza;
    } else {
      pj->armorClass = 10 + pj->bonifDestreza;
    }
    printf("Tu bonificador de destreza es %d\n", pj->bonifDestreza);
  } else {
    printf("Tus caracteristicas estan al maximo\n");
  }
}

// Da inicio al combate y luego otorga el loot en caso de ganar
void estructuraCombate(Jugador *pj, Jugador *enemigo, HashMap *objetos) {
  printf("Comienza el combate\n");
  combate(pj, enemigo, objetos);
  if (pj->pH <= 0) {
    printf("Has muerto\n");
    exit(EXIT_SUCCESS);
  } else {
    printf("Has ganado\n");
    printf("Has obtenido %ld de experiencia\n", enemigo->exp);
    pj->exp += enemigo->exp;
    while (pj->exp >= (pj->psMax * 10)) {
      subidaNivel(pj);
      st_mapa();
    }
  }
  if (enemigo->pH <= 0) {
    Pair *aux = firstMap(enemigo->items);
    while (aux) {
      int *itemJug = valueRet(searchMap(pj->items, aux->key));
      int *cantItemEnem = aux->value;
        if (searchMap(enemigo->equipo, "Espada") ||
          searchMap(enemigo->equipo, "Espada Secundaria") ||
          searchMap(enemigo->equipo, "Escudo") ||
          searchMap(enemigo->equipo, "Armadura")) 
          (*cantItemEnem)++;
      
      if (!itemJug) {
        insertMap(pj->items, aux->key, aux->value);
      } 
      else {
        (*itemJug) += *cantItemEnem;
      }
      printf("Has obtenido %s\n", aux->key);
      aux = nextMap(enemigo->items);
    }
  }
  pj->desarmadoPrin = 0;
  pj->desarmadoSec = 0;
  pj->desventaja = 0;
}

// Genera una caracteristica aleatoria
int generadorCaract() {
  int d1 = rand() % 6 + 1;
  int d2 = rand() % 6 + 1;
  int d3 = rand() % 6 + 1;
  int d4 = rand() % 6 + 1;
  printf("Los dados han sido %d, %d, %d, %d\n", d1, d2, d3, d4);
  int min = d1;
  if (d2 < min) {
    min = d2;
  }
  if (d3 < min) {
    min = d3;
  }
  if (d4 < min) {
    min = d4;
  }
  printf("El valor de la caracteristica es %d\n", d1 + d2 + d3 + d4 - min);
  return d1 + d2 + d3 + d4 - min;
}

// Crea un nuevo jugador
Jugador *crearJugador(HashMap *objetos) {
  Jugador *pj = (Jugador *)malloc(sizeof(Jugador));
  printf("¿Cual es tu nombre?\n");
  char *nombre = (char *)malloc(30 * sizeof(char));
  scanf("%[^\n]", nombre);
  getchar();

  // // Solo esta por un tema de testing
  // printf("Es jugador?\n");
  // char resp[3];
  // scanf("%s", resp);
  // getchar();

  pj->nombre = nombre;
  pj->items = createMap(10);
  pj->equipo = createMap(3);
  pj->pH = 10;
  pj->psMax = 10;
  pj->comp = 2;
  pj->exp = 0;
  pj->prota = 1;
  printf("Se realizaran lanzamientos aleatorios para determinar la fuerza y la "
         "destreza del personaje\n");
  printf("Primer lanzamiento\n");
  int caract1 = generadorCaract();
  printf("Segundo lanzamiento\n");
  int caract2 = generadorCaract();

  // Se asignan las caracteristicas
  printf("¿Cual sera tu caracteristica principal?\n");
  printf("1. Fuerza: Bonificador al daño del arma\n");
  printf("2. Destreza: Bonificador a la armadura\n");
  int opc;
  scanf("%d", &opc);
  getchar();
  if (opc == 1) {
    if (caract1 >= caract2) {
      pj->fuerza = caract1 + 2;
      pj->bonifFuerza = (pj->fuerza - 10) / 2;
      pj->destreza = caract2 + 1;
      pj->bonifDestreza = (pj->destreza - 10) / 2;
    } else {
      pj->fuerza = caract2 + 2;
      pj->bonifFuerza = (pj->fuerza - 10) / 2;
      pj->destreza = caract1 + 1;
      pj->bonifDestreza = (pj->destreza - 10) / 2;
    }
  } else {
    if (caract1 >= caract2) {
      pj->fuerza = caract2 + 1;
      pj->bonifFuerza = (pj->fuerza - 10) / 2;
      pj->destreza = caract1 + 2;
      pj->bonifDestreza = (pj->destreza - 10) / 2;
    } else {
      pj->fuerza = caract1 + 1;
      pj->bonifFuerza = (pj->fuerza - 10) / 2;
      pj->destreza = caract2 + 2;
      pj->bonifDestreza = (pj->destreza - 10) / 2;
    }
  }
  printf("Bonificador de fuerza : %d\n", pj->bonifFuerza);
  printf("Bonificador de destreza : %d\n", pj->bonifDestreza);

  // if (strcmp(resp, "si") == 0)
  //   pj->prota = 1;
  // else {
  //   pj->prota = 0;
  //   // Solo test para que los enemigos den exp
  //   printf("¿Cuanta experiencia dara?\n");
  //   int xp = 0;
  //   scanf("%d", &xp);
  //   getchar();
  //   pj->exp = xp;
  // }
  pj->turno = 0;
  pj->armorClass = 10 + pj->bonifDestreza;
  pj->cubierto = 0;
  pj->desventaja = 0;

  // Inserta los objetos iniciales
  Objeto *espada = valueRet(searchMap(objetos, "Palo de madera"));
  int *cantEsp = (int*)malloc(sizeof(int));
  *cantEsp = 0;
  insertMap(pj->items, espada->nombre, cantEsp);
  insertMap(pj->equipo, "Espada", espada);
  pj->desarmadoPrin = 0;
  pj->desarmadoSec = 0;
  pj->ultAcc = 0;

  int *cantPoc = (int*) malloc(sizeof(int));
  *cantPoc = 2;
  insertMap(pj->items, "Pocion", cantPoc);

  // // Test
  // printf("Tendra escudo?\n");
  // printf("1. Si\n");
  // printf("2. No\n");
  // scanf("%d", &opc);
  // getchar();
  // if (opc == 2) {
  //   return pj;
  // }
  // Objeto *escudo = (Objeto *)malloc(sizeof(Objeto));
  // escudo->nombre = "Escudo";
  // escudo->atk = 0;
  // escudo->def = 2;
  // escudo->curacion = 0;
  // // escudo->cantidad = 0;
  // insertMap(pj->items, "Escudo", 0);
  // insertMap(pj->equipo, "Escudo", escudo);
  // pj->armorClass += 2;
  return pj;
}

// Zona de descanso y recupera la vida maxima
void hoguera(Jugador *pj, HashMap *Objetos) {
  pj->pH = pj->psMax;
  printf("Has descansado en la hoguera\n");
  getchar();
  while (true) {
    printf("¿Deseas realizar algo más?\n");
    printf("1. Cambiar equipo\n");
    printf("2. Ver stats\n");
    printf("3. Salir\n");
    int opc;
    scanf("%d", &opc);
    getchar();
    switch (opc) {
    case 1:
      cambioEquipo(pj, Objetos);
      break;

    case 2:
      stats(pj, Objetos);
      break;

    default:
      return;
    }
  }
}

void mostrarObj(HashMap *Objetos) {
  Objeto *aux = valueRet(firstMap(Objetos));
  while (aux) {
    printf("%s, %d, %d, %d\n", aux->nombre, aux->atk, aux->def, aux->curacion);
    aux = valueRet(nextMap(Objetos));
  }
}

// Lectura de enemigos csv
HashMap *lecturaPjs(HashMap *Objetos) {
  FILE *archivo = fopen("Enemigos.csv", "r");
  if (!archivo) {
    printf("No se pudo abrir el archivo\n");
    return NULL;
  }
  HashMap *enemigos = createMap(10);
  char *linea = (char *)malloc(300 * sizeof(char));
  fgets(linea, 300, archivo);
  while (!feof(archivo)) {
    fgets(linea, 300, archivo);
    Jugador* pj = (Jugador*) malloc(sizeof(Jugador));
    char *aux = strtok(linea, ",");
    pj->nombre = (char*) malloc(30*sizeof(char));
    strcpy(pj->nombre, aux);
    aux = strtok(NULL, ",");
    if (strcmp("si", aux) == 0) {
      free(pj);
      fgets(linea, 300, archivo);
      continue;
    }
    pj->prota = 0;
    aux = strtok(NULL, ",");
    pj->pH = atoi(aux);
    aux = strtok(NULL, ",");
    pj->psMax = atoi(aux);
    aux = strtok(NULL, ",");
    pj->comp = atoi(aux);
    aux = strtok(NULL, ",");
    pj->fuerza = atoi(aux);
    aux = strtok(NULL, ",");
    pj->bonifFuerza = (pj->fuerza - 10) / 2;
    aux = strtok(NULL, ",");
    pj->destreza = atoi(aux);
    aux = strtok(NULL, ",");
    pj->bonifDestreza = (pj->destreza - 10) / 2;
    aux = strtok(NULL, ",");
    pj->exp = atoi(aux);
    char *items = (char *)malloc(100 * sizeof(char));
    pj->items = createMap(10);
    items = strtok(NULL, ",");
    char *equipo = (char *)malloc(100 * sizeof(char));
    pj->equipo = createMap(5);
    equipo = strtok(NULL, ",");
    aux = strtok(items, ";");
    while (aux) {
      // if (strcmp(aux, "0") == 0) {
      //   break;
      // }
      
      int *objCant = valueRet(searchMap(pj->items, aux));
      if (objCant) {
        (*objCant)++;
        aux = strtok(NULL, ";");
      }
      else {
        Objeto *obj = valueRet(searchMap(Objetos, aux));
        if (!obj) {
          aux = strtok(NULL, ";");
          continue;
        }
        // obj->cantidad = 1;
        int *base = (int*) malloc(sizeof(int));
        *base = 1;
        insertMap(pj->items, obj->nombre, base);
        aux = strtok(NULL, ";");
      }
    }
    // mostrarObj(pj->items);
    printf("\n");
    aux = strtok(equipo, ";");
    while (aux) {
      if (aux[strlen(aux) - 2] == '\r') {
        aux[strlen(aux) - 2] = '\0';
      }
      int *objCant = valueRet(searchMap(pj->items, aux));
      if (!objCant) {
        aux = strtok(NULL, ";");
      }
      Objeto *obj = valueRet(searchMap(Objetos, aux));
      if (obj->atk > 0 && !searchMap(pj->equipo, "Espada")) {
        insertMap(pj->equipo, "Espada", obj);
      }
      else if (obj->atk > 0 && searchMap(pj->equipo, "Espada")) {
        insertMap(pj->equipo, "Espada Secundaria", obj);
      }
      else if (strcmp(aux, "Escudo") == 0) {
        insertMap(pj->equipo, "Escudo", obj);
      }
      else {
        insertMap(pj->equipo, "Armadura", obj);  
      }
      (*objCant)--;
      aux = strtok(NULL, ";");
    }
    Objeto *armor = valueRet(searchMap(pj->equipo, "Armadura"));
    if (armor) {
      if (armor->curacion == 0)
        pj->armorClass = 10 + armor->def + pj->bonifDestreza;
      else 
        pj->armorClass = 10 + armor->def + pj->bonifFuerza;
    }
    else 
      pj->armorClass = 10 + pj->bonifDestreza;
    
    insertMap(enemigos, pj->nombre, pj);
  }
  fclose(archivo);
  return enemigos;
}

// Lectura de objetos
HashMap *lecturaObjetos() {
  FILE *archivo = fopen("Items.csv", "r");
  if (!archivo) {
    printf("No se pudo abrir el archivo\n");
    return NULL;
  }
  HashMap *objetos = createMap(10);
  char *linea = (char *)malloc(300 * sizeof(char));
  fgets(linea, 300, archivo);
  while (!feof(archivo)) {
    fgets(linea, 300, archivo);
    Objeto *obj = (Objeto *)malloc(sizeof(Objeto));
    char *aux = strtok(linea, ",");
    obj->nombre = (char*) malloc(30*sizeof(char));
    strcpy(obj->nombre, aux);
    aux = strtok(NULL, ",");
    obj->atk = atoi(aux);
    aux = strtok(NULL, ",");
    obj->def = atoi(aux);
    aux = strtok(NULL, ",");
    obj->curacion = atoi(aux);
    insertMap(objetos, obj->nombre, obj);
  }
  fclose(archivo);
  return objetos;
}

void mostrarEn(HashMap *enemigos) {
  Jugador *aux = valueRet(firstMap(enemigos));
  while (aux) {
    printf("%s, %ld, %ld, %d, %d, %d, %ld\n", aux->nombre, aux->pH, aux->psMax,
           aux->comp, aux->fuerza, aux->destreza, aux->exp);
    aux = valueRet(nextMap(enemigos));
  }
}


void limpiarArchivos() {
    // Sobrescribir mapatemp.csv con el contenido de mapa.csv
    remove("mapatemp.csv");
    if (rename("mapa.csv", "mapatemp.csv") != 0) {
        perror("Error al renombrar el archivo");
    }

    // Vaciar action.txt
    FILE *actionFile = fopen("action.txt", "w");
    if (actionFile == NULL) {
        perror("No se pudo abrir action.txt");
        exit(EXIT_FAILURE);
    }
    fclose(actionFile);

    // Escribir 'inicio' en estado.txt
    FILE *estadoFile = fopen("estado.txt", "w");
    if (estadoFile == NULL) {
        perror("No se pudo abrir estado.txt");
        exit(EXIT_FAILURE);
    }
    fprintf(estadoFile, "inicio");
    fclose(estadoFile);
}




int main(){

  // Configuracion
  int desiredIterationsPerSecond = 5;
  int microsecondsPerIteration = 1000000 / desiredIterationsPerSecond;
  const char* fileEstado = "estado.txt";

  // Inicializar estructuras
  srand(time(NULL));

  HashMap *objetos = lecturaObjetos();
  HashMap *enemigos = lecturaPjs(objetos);  

  Jugador *yo = crearJugador(objetos);
  
  Jugador *enemigo = valueRet(searchMap(enemigos, "Duende"));

  while (1) {  // Bucle infinito
    Mapa mapa;
    int player_x = 1;
    int player_y = 1;

    load_map_from_file("mapa.csv", &mapa);

    while (1) {
        display_map(&mapa, player_x, player_y);
        save_map_to_file("mapatemp.csv", &mapa);
        process_actions("accion.txt", &mapa, &player_x, &player_y);

        printf("Ejecutando lógica...\n");
        if(checkFileForEstado(fileEstado,"combate") == true){
            estructuraCombate(yo, enemigo, objetos);
        }
        if(checkFileForEstado(fileEstado,"bonfire") == true){
            hoguera(yo, objetos);
        }



        usleep(microsecondsPerIteration);  // Pausa el programa durante los microsegundos calculados
    }


    limpiarArchivos();  // Llamada a la función de limpieza


    return 0;
}
}

