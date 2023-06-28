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

