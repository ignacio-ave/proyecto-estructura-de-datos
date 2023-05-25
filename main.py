import pygame
import csv

# Dimensiones de la ventana y los caracteres
WINDOW_WIDTH = 640
WINDOW_HEIGHT = 640
CHAR_SIZE = 64

# Carga el archivo CSV del mapa
def load_map(file_path):
    with open(file_path, newline='') as file:
        reader = csv.reader(file)
        return list(reader)

# Dibuja el mapa en la ventana
def draw_map(map_data):
    window.fill((0, 0, 0))

    for y in range(len(map_data)):
        for x in range(len(map_data[y])):
            cell = map_data[y][x]
            if cell == '#':
                image = wall_image
            elif cell == '@':
                image = jugador
            elif cell == 'i':
                image = door_image
            elif cell == 'd':
                image = door_image
            elif cell == 'p':
                image = door_image
            else:
                image = floor_image

            window.blit(image, (x * CHAR_SIZE, y * CHAR_SIZE))

    pygame.display.flip()

# Guarda la acción en el archivo accion.txt
def save_action(action):
    with open('accion.txt', 'w') as file:
        file.write(action)

# Carga los datos del jugador desde el archivo jugador.csv
def load_player_data():
    with open('jugador.csv', newline='') as file:
        reader = csv.reader(file)
        data = next(reader)  # Lee la primera fila
        return data

# Inicialización de Pygame
pygame.init()
window = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
pygame.display.set_caption('Map Viewer')
clock = pygame.time.Clock()

# Carga las imágenes
floor_image = pygame.image.load('empty.png').convert()
wall_image = pygame.image.load('wall.png').convert()
door_image = pygame.image.load('door.png').convert()
jugador = pygame.image.load('player.png').convert()

# Carga el mapa inicial
map_data = load_map('mapatemp.csv')

running = True

# Bucle principal
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Verifica las teclas presionadas
    keys = pygame.key.get_pressed()
    if keys[pygame.K_w]:
        save_action('w')
    elif keys[pygame.K_a]:
        save_action('a')
    elif keys[pygame.K_s]:
        save_action('s')
    elif keys[pygame.K_d]:
        save_action('d')

    # Carga el mapa actualizado desde el archivo
    map_data = load_map('mapatemp.csv')

    # Carga los datos del jugador desde el archivo
    player_data = load_player_data()

    # Dibuja el mapa en la ventana
    draw_map(map_data)

    # Dibuja los datos del jugador en la pantalla
    font = pygame.font.Font(None, 24)
    text = font.render(f"Nombre: {player_data[0]}", True, (255, 255, 255))
    window.blit(text, (10, 10))
    text = font.render(f"Puntaje: {player_data[1]}", True, (255, 255, 255))
    window.blit(text, (10, 40))
    text = font.render(f"Vida: {player_data[2]}", True, (255, 255, 255))
    window.blit(text, (10, 70))

    pygame.display.flip()

    clock.tick(10)  # Actualiza la pantalla 10 veces por segundo

# Finaliza Pygame
pygame.quit()
