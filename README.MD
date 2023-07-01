# Nombre del Proyecto

Descripción breve del proyecto.

## Requisitos

Antes de empezar, asegúrate de tener los siguientes requisitos:

- Python
- Pip
- Compilador de C (GCC)

Puedes verificar si tienes estos componentes instalados al abrir una terminal e introducir los siguientes comandos:

```bash
python --version
pip --version
gcc --version
```

Si falta algún componente, deberás instalarlo antes de continuar.

## Instalación

### Instalación de Pygame

Para instalar Pygame, ejecuta el siguiente comando en tu terminal:

```python
pip install pygame
```

### Compilación del Código C

Para compilar los archivos del código fuente de C, usa los siguientes comandos en la terminal:

```bash
gcc main.c hashmap.c -o juegoMazmorra
```

Esto generará un ejecutable llamado 'juego'.

Si se prefiere, este puede ser creado como uno .exe

```bash
gcc main.c hashmap.c -o juegoMazmorra.exe
```

## Ejecución del Juego

Una vez que hayas compilado el código con éxito, podrás correr el juego. En la terminal, ejecuta:

```bash
./juego
```

Para correr el script de Python, abre una nueva terminal en el directorio del proyecto y ejecuta:

```bash
python main.py
```

## Descripción de la Solución

El juego interactivo propuesto incorpora una diversidad de Tipos de Datos Abstractos (TDA) y estructuras de datos, como HashMap para almacenar elementos clave: jugador, enemigos e ítems, garantizando un acceso rápido y rendimiento óptimo. Los TDA encapsulan la lógica del juego, simplificando su diseño, implementación y potenciales modificaciones futuras. El juego, programado en C, incluye un archivo .py que interpreta visualmente la actividad en memoria y la lógica del juego, esencial para su correcto funcionamiento. La meticulosa elección de TDA y estructuras de datos asegura una solución eficaz, escalable y una experiencia de juego robusta.

El juego hace uso de una serie de archivos:

- **mapa.csv**: Describe el mapa inicial del juego.
- **Mapa1.csv** a **MapaN.csv**: Representan los mapas utilizados en distintas etapas del juego.
- **Mapatemp.csv**: Es una versión en tiempo real del mapa actual, reflejando la posición del jugador.
- **Estado.txt**: Muestra uno de los cuatro posibles estados del juego, es actualizado por el código en C y es leído por el programa en Python.
- **Jugador.csv**: Almacena todos los datos relevantes del jugador, incluyendo su inventario. Los cambios se guardan en el disco.
- **action.txt**: Registra las acciones llevadas a cabo durante el juego.
- **Enemigo actual.csv**: Mantiene una copia de los datos en memoria del enemigo que el jugador está enfrentando en el momento.
- **Mensaje.txt**: Utilizado para mostrar mensajes o textos al jugador durante el juego. Los contenidos son leídos y desplegados por el programa en Python.

**Funciones Python para la implementación gráfica:**

- `load_map(file_path)`: Carga un mapa desde un archivo CSV.
- `draw_map(map_data)`: Dibuja el mapa en la ventana del juego.
- `save_action(action)`: Guarda la acción en el archivo "accion.txt".
- `read_text()`: Lee el contenido del archivo "texto.txt".
- `read_estado()`: Lee el contenido del archivo "estado.txt".
- `estado`: Variable que almacena el estado actual del juego.

**Bucle principal del juego en Python:**
Administra eventos de salida, actualiza el estado del juego, gestiona los movimientos y acciones del jugador, muestra diferentes pantallas (mapa, combate, hoguera, entrada de nombre), y muestra mensajes en la parte inferior de la pantalla.

**Funciones C para la implementación gráfica y el mapa:**

- `void clear_console()`: Limpia la consola.
- `void load_map_from_file(char *file_path, Mapa *mapa)`: Carga un mapa desde un archivo.
- `void display_map(Mapa *mapa, int player_x, int player_y)`: Muestra el mapa y la posición del jugador.
- `void save_map_to_file(char *file_path, Mapa *mapa)`: Guarda el mapa en un archivo.
- `void process_actions(char *file_path, Mapa *mapa, int *player_x, int *player_y)`: Procesa acciones y actualiza el mapa y la posición del jugador.
- `void PrintArchivo(const char* buffer)`: Guarda e imprime contenido de un buffer en "texto.txt".

**Funciones de C relacionadas al combate:**

- `cambioEquipo(Jugador *pj)`: Permite al jugador cambiar su equipo.
- `stats(Jugador *pj)`: Muestra las estadísticas del jugador.
- `printOp(int objSec)`: Imprime las opciones de acción durante un combate.
- `ataque(Jugador *pj, Jugador *enemigo, char *objeto)`: Realiza el ataque del jugador o enemigo.
- `usarPocion(Jugador *pj)`: Permite al jugador usar una poción para curarse.
- `detCaida(Jugador *pj, int accExtra)`: Modifica la opción de acción extra si el jugador está desarmado.
- `turnoPj(Jugador *pj, Jugador *enemigo)`: Maneja el turno del jugador.
- `combate(Jugador *pj, Jugador *enemigo)`: Realiza el combate.
- `subidaNivel(Jugador *pj)`: Realiza la subida de nivel del jugador.
- `estructuraCombate(Jugador *pj, Jugador *enemigo)`: Inicia el combate.
- `generadorCaract()`: Genera una característica aleatoria para un nuevo jugador.

**Complejidad temporal:**

Tabla 1 : Complejidad temporal de la estructura de datos Hashmap

| Operación | Complejidad de tiempo promedio |
|-----------|-------------------------------|
| createMap | O(1) |
| insertMap | O(1) |
| eraseMap | O(1) |
| searchMap | O(1) |
| firstMap | O(1) |
| nextMap | O(1) |
| enlarge | O(n) |
| sizeMap | O(1) |
| valueRet | O(1) |

Tabla 2 : Complejidad temporal de las funciones relacionadas al mapa 

| Operación | Complejidad Temporal |
|-----------|----------------------|
| process_actions | O(A) |
| display_map | O(M * N) |
| load_map_from_file | O(M * N) |
| clear_console | O(1) |
| save_map_to_file | O(M * N) |
| get_player_name | O(1) |
| initialize_player | O(1) |
| save_player_info | O(1) |
| load_player_info | O(1) |
| display_player_info | O(1) |

Tabla 3 : Complejidad temporal de las funciones de combate 

| Operación | Complejidad Temporal Promedio |
|-----------|-------------------------------|
| cambioEquipo | O(1) |
| stats | O(n) |
| printOp | O(1) |
| ataque | O(1) |
| usarPocion | O(1) |
| detCaida | O(1) |
| turnoPj | O(1) |
| combate | O(n) |
| subidaNivel | O(1) |
| estructuraCombate | O(n) |
| generadorCaract | O(1) |

---
## Situación del Grupo

En el transcurso del proyecto, hemos experimentado dificultades significativas en términos de la contribución de los miembros del equipo. En particular, el desempeño de Antonia ha sido motivo de preocupación y ha generado varios obstáculos para la finalización exitosa de nuestro trabajo. A continuación, se proporciona un desglose detallado de los eventos.

Durante una llamada de Discord, diseñada para coordinar los esfuerzos de integración de código por parte de Felipe y Carlos, se dividieron las tareas y se asignaron responsabilidades. Aunque Antonia mostró disposición inicial para trabajar, su compromiso no se tradujo en una contribución efectiva.

La comunicación fue uno de los principales problemas. Antonia asistió a la reunión de Discord más tarde de lo acordado y permaneció en silencio durante la mayor parte de la reunión, limitándose a responder mediante texto. A pesar de su llegada tardía, se le proporcionó un audio detallado con todas las tareas a realizar y se le asignó la tarea de modificar ciertas funciones printf por la función printarchivo.

Desafortunadamente, hubo un malentendido con las instrucciones. Antonia modificó las funciones incorrectas, lo que resultó en un retroceso de trabajo y requirió que otro miembro deshiciera manualmente sus cambios. Esto puede verificarse a través de las estadísticas del repositorio donde se realizó el merge.

A Antonia también se le asignó la tarea de revisar la compatibilidad del programa con Windows, siendo ella la única con este sistema operativo a esa hora. Sin embargo, esta tarea tampoco fue completada por Antonia y tuvo que ser realizada por otro miembro del equipo.

Al día siguiente, a pesar de los esfuerzos continuos del resto del equipo, Antonia se mantuvo inactiva. Para agravar la situación, se le observó jugando League of Legends a las 2 de la mañana mientras los demás miembros estaban trabajando en el proyecto. Al ser confrontada sobre esta situación, Antonia optó por salir del grupo.

En resumen, la participación de Antonia ha sido insuficiente y ha representado un desafío para el grupo. A pesar de su aparente disposición a colaborar, la realidad ha demostrado una falta de compromiso y una interpretación incorrecta de las tareas asignadas, resultando en retrasos y una carga de trabajo adicional para los demás miembros. Este hecho está claramente documentado en el repositorio del proyecto y en nuestras interacciones de comunicación.

Esperamos que esta explicación proporcione una visión clara de los desafíos que hemos enfrentado como equipo.
