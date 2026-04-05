# pygame template
import pygame

from read import ser, readTick, inputs

pygame.init()

WIDTH = 640
HEIGHT = 480
SIZE = (WIDTH, HEIGHT)

screen = pygame.display.set_mode(SIZE)
clock = pygame.time.Clock()

# ---------------------------
# Initialize global variables

accel_x = 0
accel_y = 0
accel_z = 0

hp_x = 0
hp_y = 0
hp_z = 0

vel_x = 0
vel_y = 0
vel_z = 0

# ---------------------------

running = True
while running:
    # EVENT HANDLING
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # GAME STATE UPDATES
    # All game math and comparisons happen here

    readTick()
    for key in inputs:
        val = inputs[key]
        print(key, val)

        if key == 'accel_x':
            accel_x = int(val)

        if key == 'accel_y':
            accel_y = int(val)

        if key == 'accel_z':
            accel_z = int(val)
        
        if key == 'hp_x':
            hp_x = float(val)

        if key == 'hp_y':
            hp_y = float(val)

        if key == 'hp_z':
            hp_z = float(val)

        if key == 'vel_x':
            vel_x = float(val)

        if key == 'vel_y':
            vel_y = float(val)

        if key == 'vel_z':
            vel_z = float(val)

    inputs.clear()

    # DRAWING
    screen.fill((255, 255, 255))  # always the first drawing command

    pygame.draw.line(screen, (30, 30, 30), (10, HEIGHT / 2), (WIDTH - 10, HEIGHT / 2))
    pygame.draw.circle(screen, (255, 0, 0), (100, 240 + hp_x / 100.), 25)
    pygame.draw.circle(screen, (255, 0, 0), (200, 240 + hp_y / 100.), 25)
    pygame.draw.circle(screen, (255, 0, 0), (300, 240 + hp_z / 100.), 25)
    pygame.draw.circle(screen, (0, 0, 255), (100, 240 + accel_x / 100.), 20)
    pygame.draw.circle(screen, (0, 0, 255), (200, 240 + accel_y / 100.), 20)
    pygame.draw.circle(screen, (0, 0, 255), (300, 240 + accel_z / 100.), 20)
    pygame.draw.circle(screen, (0, 255, 0), (100, 240 + vel_x), 17)
    pygame.draw.circle(screen, (0, 255, 0), (200, 240 + vel_y), 17)
    pygame.draw.circle(screen, (0, 255, 0), (300, 240 + vel_z), 17)

    # Must be the last two lines
    # of the game loop
    pygame.display.flip()
    clock.tick(60)
    #---------------------------

ser.close()
pygame.quit()