#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAP_SIZE_X 10
#define MAP_SIZE_Y 10

// Estructura para el jugador
typedef struct {
  char nombre[50];
  char inventario[2][50]; // Dos slots de inventario
  int puntaje;
  int vida;
} Jugador;

// Estructura para el mapa
typedef struct {
  char data[MAP_SIZE_X][MAP_SIZE_Y];
} Mapa;

void clear_console() {
  system("clear"); // Utilizar "cls" en Windows
}

void load_map_from_file(char *file_path, Mapa *mapa) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    printf("Error al abrir el archivo.");
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
  if (file == NULL) {
    printf("Error al abrir el archivo.");
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

void process_actions(char *file_path, Mapa *mapa, int *player_x, int *player_y, Jugador *jugador) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    printf("Error al abrir el archivo.");
    exit(1);
  }

  char action[5];
  fgets(action, 5, file);

  if (strlen(action) > 0) {
    switch (action[0]) {
      case 'W':
      case 'w':
        if (*player_y > 0 && mapa->data[*player_y - 1][*player_x] != '#') {
          mapa->data[*player_y][*player_x] = '.'; // Restaurar el espacio vacío anterior
          (*player_y)--;
        }
        break;
      case 'A':
      case 'a':
        if (*player_x > 0 && mapa->data[*player_y][*player_x - 1] != '#') {
          mapa->data[*player_y][*player_x] = '.'; // Restaurar el espacio vacío anterior
          (*player_x)--;
        }
        break;
      case 'S':
      case 's':
        if (*player_y < MAP_SIZE_Y - 1 && mapa->data[*player_y + 1][*player_x] != '#') {
          mapa->data[*player_y][*player_x] = '.'; // Restaurar el espacio vacío anterior
          (*player_y)++;
        }
        break;
      case 'D':
      case 'd':
        if (*player_x < MAP_SIZE_X - 1 && mapa->data[*player_y][*player_x + 1] != '#') {
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
    printf("Error al abrir el archivo.");
    exit(1);
  }
  fclose(file);
}

void get_player_name(Jugador *jugador) {
  printf("Ingrese el nombre del jugador: ");
  fgets(jugador->nombre, sizeof(jugador->nombre), stdin);

  // Eliminar el salto de línea final
  jugador->nombre[strcspn(jugador->nombre, "\n")] = '\0';
}

void initialize_player(Jugador *jugador) {
  memset(jugador->inventario, 0, sizeof(jugador->inventario)); // Inicializar inventario vacío
  jugador->puntaje = 0;
  jugador->vida = 100;
}

void save_player_info(Jugador *jugador) {
  FILE *file = fopen("jugador.csv", "w");
  if (file == NULL) {
    printf("Error al abrir el archivo.");
    exit(1);
  }

  fprintf(file, "%s,%d,%d\n", jugador->nombre, jugador->puntaje, jugador->vida);

  fclose(file);
}

void load_player_info(Jugador *jugador) {
  FILE *file = fopen("jugador.csv", "r");
  if (file == NULL) {
    printf("Error al abrir el archivo.");
    exit(1);
  }

  fscanf(file, "%[^,],%d,%d\n", jugador->nombre, &(jugador->puntaje), &(jugador->vida));

  fclose(file);
}

void display_player_info(Jugador *jugador) {
  printf("Nombre del jugador: %s\n", jugador->nombre);
  printf("Puntaje: %d\n", jugador->puntaje);
  printf("Vida: %d\n", jugador->vida);
}

int main() {
  Jugador jugador;
  Mapa mapa;
  int player_x = 0;
  int player_y = 0;

  get_player_name(&jugador);
  initialize_player(&jugador);

  load_player_info(&jugador);

  load_map_from_file("mapa.csv", &mapa);

  while (1) {
    display_map(&mapa, player_x, player_y);
    save_map_to_file("mapatemp.csv", &mapa);

    process_actions("accion.txt", &mapa, &player_x, &player_y, &jugador);

    if (mapa.data[player_y][player_x] == 'p') {
      // Cambiar al siguiente mapa
      load_map_from_file("mapa2.csv", &mapa);
      player_x = 0;
      player_y = 0;
    }

    save_player_info(&jugador);
    display_player_info(&jugador);

    usleep(200000); // Esperar 0.2 segundos (actualizar 5 veces por segundo)
  }

  return 0;
}
