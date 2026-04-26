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

gyro_x = 0
gyro_y = 0
gyro_z = 0

fsr1 = 0
fsr2 = 0
fsr3 = 0
flex1 = 0
flex2 = 0
flex3 = 0

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
            accel_x = float(val)

        if key == 'accel_y':
            accel_y = float(val)

        if key == 'accel_z':
            accel_z = float(val)
        
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

        if key == 'fsr1':
            fsr1 = int(val)

        if key == 'fsr2':
            fsr2 = int(val)

        if key == 'fsr3':
            fsr3 = int(val)

        if key == 'flex1':
            flex1 = float(val)

        if key == 'flex2':
            flex2 = float(val)

        if key == 'flex3':
            flex3 = float(val)

        if key == 'gyro_x':
            gyro_x = float(val)

        if key == 'gyro_y':
            gyro_y = float(val)

        if key == 'gyro_z':
            gyro_z = float(val)


    inputs.clear()

    # DRAWING
    screen.fill((255, 255, 255))  # always the first drawing command

    # Accel
    pygame.draw.line(screen, (30, 30, 30), (10, HEIGHT / 2), (WIDTH - 10, HEIGHT / 2))
    pygame.draw.circle(screen, (255, 0, 0), (50, 240 + hp_x / 100.), 25)
    pygame.draw.circle(screen, (255, 0, 0), (100, 240 + hp_y / 100.), 25)
    pygame.draw.circle(screen, (255, 0, 0), (150, 240 + hp_z / 100.), 25)
    pygame.draw.circle(screen, (0, 0, 255), (50, 240 + accel_x / 100.), 20)
    pygame.draw.circle(screen, (0, 0, 255), (100, 240 + accel_y / 100.), 20)
    pygame.draw.circle(screen, (0, 0, 255), (150, 240 + accel_z / 100.), 20)
    pygame.draw.circle(screen, (0, 255, 0), (50, 240 + vel_x), 17)
    pygame.draw.circle(screen, (0, 255, 0), (100, 240 + vel_y), 17)
    pygame.draw.circle(screen, (0, 255, 0), (150, 240 + vel_z), 17)

    # Gyro
    pygame.draw.circle(screen, (255, 0, 0), (250, 240 + gyro_x / 100.), 25)
    pygame.draw.circle(screen, (255, 0, 0), (300, 240 + gyro_y / 100.), 25)
    pygame.draw.circle(screen, (255, 0, 0), (350, 240 + gyro_z / 100.), 25)

    # Flex/FSR

    pygame.draw.circle(screen, (255, 155, 0), (450, 240 + fsr1 / 20), 5)
    pygame.draw.circle(screen, (255, 155, 0), (460, 240 + fsr2 / 20), 5)
    pygame.draw.circle(screen, (255, 155, 0), (470, 240 + fsr3 / 20), 5)
    pygame.draw.circle(screen, (50, 30, 100), (500, 240 + flex1 * 200), 5)
    pygame.draw.circle(screen, (50, 30, 100), (510, 240 + flex2 * 200), 5)
    pygame.draw.circle(screen, (50, 30, 100), (520, 240 + flex3 * 200), 5)

    # Must be the last two lines
    # of the game loop
    pygame.display.flip()
    clock.tick(60)
    #---------------------------

ser.close()
pygame.quit()