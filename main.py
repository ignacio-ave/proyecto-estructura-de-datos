
import pygame
import csv
import os
import time

# Dimensiones de la ventana y los caracteres
WINDOW_WIDTH = 640
WINDOW_HEIGHT = 740

# Inicialización de Pygame
pygame.init()
window = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
pygame.display.set_caption('Juego estructura de datos')

# Carga las fuent
# de texto
font_title = pygame.font.Font(None, 36)
font_subtitle = pygame.font.Font(None, 24)
font_text = pygame.font.Font(None, 18)
# Carga las imágenes
fondo_image = pygame.image.load('sprites/fondofinal.png').convert()
enemy_image_full = pygame.image.load('sprites/enemigo.png').convert()
wall_image = pygame.image.load('sprites/wall.png').convert()
player_images = [pygame.image.load('sprites/pp1.png').convert_alpha(),
                 pygame.image.load('sprites/pp2.png').convert_alpha(),
                 pygame.image.load('sprites/pp3.png').convert_alpha()]  # Replace with your actual frames
bonfire_images = [pygame.image.load('sprites/bf1.png').convert_alpha(),
                 pygame.image.load('sprites/bf2.png').convert_alpha(),
                 pygame.image.load('sprites/bf3.png').convert_alpha()]  # Replace with your actual frames
door_image = pygame.image.load('sprites/door.png').convert()
item_image = pygame.image.load('sprites/cofre.png').convert()
enemy_images = [pygame.image.load('sprites/e1.png').convert_alpha(),
                 pygame.image.load('sprites/e2.png').convert_alpha(),
                 pygame.image.load('sprites/e3.png').convert_alpha()]  # Replace with your actual frames
lava_images = [pygame.image.load('sprites/l1.png').convert_alpha(),
                 pygame.image.load('sprites/l2.png').convert_alpha(),
                 pygame.image.load('sprites/l3.png').convert_alpha()]  # Replace with your actual frames
floor_image = pygame.image.load('sprites/empty.png').convert()
cbg = pygame.image.load('sprites/bg_combat.png').convert()
bonfire_image_full = pygame.image.load('sprites/bonfire.png').convert()

# Carga el mapa desde el archivo CSV
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
                image = player_images[frame_index]  # Use current frame image
            elif cell == 'i':
                image = item_image
            elif cell == 'l':
                image = lava_images[frame_index]
            elif cell == 'e':
                image = enemy_images[frame_index]
            elif cell == 'd':
                image = door_image
            elif cell == 'b':
                image = bonfire_images[frame_index]
            else:
                image = floor_image

            window.blit(image, (x * 32, y * 32))

    pygame.display.flip()

# Guarda la acción en el archivo accion.txt
def save_action(action):
    with open('accion.txt', 'a') as file:
        file.write(action + "\n")

# Lee el contenido del archivo "texto.txt"
def read_text():
    with open("texto.txt", "r") as file:
        texto = file.read().strip()
    return texto

# Lee el contenido del archivo "estado.txt"
def read_estado():
    with open("estado.txt", "r") as file:
        estado = file.read().strip()
    return estado

# Estado inicial
estado = "nombre"
running = True

# Animation variables
frame_index = 0
animation_delay = 100  # Delay between animation frames in milliseconds
last_frame_time = pygame.time.get_ticks()

# Texto
texto_rect = pygame.Rect(20, 640, WINDOW_WIDTH - 200, 30)
text = ""
last_modification_time = 0
read_file_interval = 1000  # Read file interval in milliseconds
last_read_time = pygame.time.get_ticks()

# Music
pygame.mixer.music.load('sounds/music1.mp3')
pygame.mixer.music.set_volume(0.2)
pygame.mixer.music.play(-1)  # Play in a loop

# Bucle principal
while running:
    clock = pygame.time.Clock()
    current_time = pygame.time.get_ticks()  # Current time

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
    
    # Leer el archivo "estado.txt" en cualquier estado
    new_estado = read_estado()
    if new_estado != estado:
        estado = new_estado

    if estado == "mapa":
        # Mostrar pantalla del mapa
        map_data = load_map("mapatemp.csv")
        draw_map(map_data)

        # Animation frame timing
        if current_time - last_frame_time >= animation_delay:
            frame_index = (frame_index + 1) % len(player_images)
            last_frame_time = current_time

            

        # Leer el archivo "estado.txt" cada 5 segundos
        if current_time % 5000 == 0:
            estado = open("estado.txt").read().strip()

        # Verifica las teclas presionadas para los movimientos
        keys = pygame.key.get_pressed()
        if keys[pygame.K_w]:
            save_action('w')
        elif keys[pygame.K_a]:
            save_action('a')
        elif keys[pygame.K_s]:
            save_action('s')
        elif keys[pygame.K_d]:
            save_action('d')

        # Leer el contenido del archivo "texto.txt" una vez por segundo or when it changes
        if current_time - last_read_time >= read_file_interval:
            modification_time = os.path.getmtime("texto.txt")
            if modification_time != last_modification_time:
                text = "TEXTO: " + read_text()
                last_modification_time = modification_time
            last_read_time = current_time
    elif estado == "combate":
        combate_framerate = 60 
        clock.tick(combate_framerate)
        # Mostrar pantalla de combate
        
        window.blit(cbg, (0, 0))

        combate_text = font_title.render("Estás en combate!", True, (255, 255, 255))
        window.blit(combate_text, (10, 10))

        enemy_rect = enemy_image_full.get_rect(center=(WINDOW_WIDTH // 2, WINDOW_HEIGHT // 4))
        window.blit(enemy_image_full, enemy_rect)

        button_font = pygame.font.Font(None, 24)
        button_labels = ["1", "2", "3", "4"]
        button_positions = [(220, 400), (380, 400), (220, 450), (380, 450)]
        button_width = 80
        button_height = 40
        button_padding = 10

        for label, pos in zip(button_labels, button_positions):
            button_rect = pygame.Rect(pos[0], pos[1], button_width, button_height)
            pygame.draw.rect(window, (255, 255, 255), button_rect)
            pygame.draw.rect(window, (0, 0, 0), button_rect.inflate(-2, -2))
            button_text = button_font.render(label, True, (255, 255, 255))
            text_rect = button_text.get_rect(center=button_rect.center)
            window.blit(button_text, text_rect)

 # Evento de los botones
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 1:
                    mouse_pos = pygame.mouse.get_pos()
                    for label, pos in zip(button_labels, button_positions):
                        button_rect = pygame.Rect(pos[0], pos[1], button_width, button_height)
                        if button_rect.collidepoint(mouse_pos):
                            save_action(label)
                            print("Botón presionado:", label)

        # Leer el contenido del archivo "texto.txt" una vez por segundo or when it changes
        if current_time - last_read_time >= read_file_interval:
            modification_time = os.path.getmtime("texto.txt")
            if modification_time != last_modification_time:
                text = "TEXTO: " + read_text()
                last_modification_time = modification_time
            last_read_time = current_time


       

    elif estado == "bonfire":
       
        # Mostrar pantalla de combate
        
        window.blit(cbg, (0, 0))

        combate_text = font_title.render("Estás en la hoguera.", True, (255, 255, 255))
        window.blit(combate_text, (10, 10))

        enemy_rect = bonfire_image_full.get_rect(center=(WINDOW_WIDTH // 2, WINDOW_HEIGHT // 4))
        window.blit(bonfire_image_full, enemy_rect)

        button_font = pygame.font.Font(None, 24)
        button_labels = ["1", "2", "3", "4"]
        button_positions = [(220, 400), (380, 400), (220, 450), (380, 450)]
        button_width = 80
        button_height = 40
        button_padding = 10

        for label, pos in zip(button_labels, button_positions):
            button_rect = pygame.Rect(pos[0], pos[1], button_width, button_height)
            pygame.draw.rect(window, (255, 255, 255), button_rect)
            pygame.draw.rect(window, (0, 0, 0), button_rect.inflate(-2, -2))
            button_text = button_font.render(label, True, (255, 255, 255))
            text_rect = button_text.get_rect(center=button_rect.center)
            window.blit(button_text, text_rect)

        # Leer el contenido del archivo "texto.txt" una vez por segundo or when it changes
        if current_time - last_read_time >= read_file_interval:
            modification_time = os.path.getmtime("texto.txt")
            if modification_time != last_modification_time:
                text = "TEXTO: " + read_text()
                last_modification_time = modification_time
            last_read_time = current_time


        # Evento de los botones
        for event in pygame.event.get():
            if event.type == pygame.MOUSEBUTTONDOWN:
                if event.button == 1:
                    mouse_pos = pygame.mouse.get_pos()
                    for label, pos in zip(button_labels, button_positions):
                        button_rect = pygame.Rect(pos[0], pos[1], button_width, button_height)
                        if button_rect.collidepoint(mouse_pos):
                            save_action(label)
                            print("Botón presionado:", label)


    elif estado == "nombre":
        # Mostrar pantalla para ingresar el nombre del jugador
        window.blit(fondo_image, (0, 0))

        title_text = font_title.render("Juego estructura de datos", True, (255, 255, 255))
        window.blit(title_text, (50, 50))

        subtitle_text = font_subtitle.render("Ingresa el nombre del jugador", True, (255, 255, 255))
        window.blit(subtitle_text, (50, 100))

        pygame.draw.rect(window, (255, 255, 255), pygame.Rect(50, 150, 200, 30))
        pygame.draw.rect(window, (0, 0, 0), pygame.Rect(52, 152, 196, 26))

        input_text = ""
        while estado == "nombre":
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                    estado = "salir"
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_RETURN:
                        estado = "mapa"
                        pygame.mixer.music.load('sounds/music2.mp3')
                        pygame.mixer.music.set_volume(0.2)
                        pygame.mixer.music.play(-1)  # Play in a loop
                        with open("nombre.txt", "w") as file:
                            file.write(input_text)
                        with open("estado.txt", "w") as file:
                            file.write("mapa")
                    elif event.key == pygame.K_BACKSPACE:
                        input_text = input_text[:-1]
                    else:
                        input_text += event.unicode

            text_surface = font_text.render(input_text, True, (255, 255, 255))
            window.blit(text_surface, (55, 155))

            pygame.display.flip()

# Leer el contenido del archivo "texto.txt" una vez por segundo or when it changes
    if current_time - last_read_time >= read_file_interval:
        modification_time = os.path.getmtime("texto.txt")
        if modification_time != last_modification_time:
            text = "TEXTO:\n" + read_text()  # Added newline character
            last_modification_time = modification_time
        last_read_time = current_time

    # Mostrar el texto en la parte inferior de la pantalla
    window.fill((0, 0, 0), texto_rect)
    text_lines = text.split('\n')  # Split text into lines
    for i, line in enumerate(text_lines):
        text_surface = font_text.render(line, True, (255, 255, 255))
        window.blit(text_surface, (texto_rect.x, texto_rect.y + i * 20))  # Adjusted vertical position for each line


    pygame.display.flip()

# Detener la música al finalizar
pygame.mixer.music.stop()

# Finaliza Pygame
pygame.quit()