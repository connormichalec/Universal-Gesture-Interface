import hid
import struct
import pygame


VID = 0x0483
PID = 0x572B

target_path = None
for d in hid.enumerate(VID, PID):
    if d['usage_page'] == 0xFF00:
        target_path = d['path']
        break

if target_path is None:
    print("Vendor HID interface not found")
else:
    dev = hid.device()
    dev.open_path(target_path)
    dev.set_nonblocking(1)
    print("Opened vendor interface")

gestures = {
    0: "None",
    1: "Thumbs up",
    2: "Thumbs middle",
    3: "Thumbs down",
    4: "Finger gun",
    5: "BANG"
}

## SERIAL READING STUFF ##
FMT = '<H12f3H'

accel_x, accel_y, accel_z, grav_x, grav_y, grav_z, gyro_x, gyro_y, gyro_z, flex_thumb, flex_index, flex_middle, fsr_thumb, fsr_index, fsr_middle = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
gesture = 0

VALID_TOPICS = {0, 1}

buffer = bytearray()
def readTick():
    global accel_x, accel_y, accel_z, grav_x, grav_y, grav_z, gyro_x, gyro_y, gyro_z, flex_thumb, flex_index, flex_middle, fsr_thumb, fsr_index, fsr_middle
    global buffer
    global gesture

    data = dev.read(65)
    if data:
        #if wanting raw data just dont decode it.
        payload = bytes(data[1:]).rstrip('\x00')
        print(f"RX: {payload}")

    """
    while True:
        # Need at least 2 bytes to check topic
        if len(buffer) < 2:
            break

        # Peek at potential topic
        topic = struct.unpack_from('<H', buffer, 0)[0]

        if topic == 0:
            PACKET_SIZE = 56
        else:
            PACKET_SIZE = 4

        if topic not in VALID_TOPICS:
            # Not aligned → discard 1 byte and try again
            buffer.pop(0)
            continue

        # Now check if we have a full packet
        if len(buffer) < PACKET_SIZE:
            break

        # Extract full packet
        packet_bytes = buffer[:PACKET_SIZE]
        del buffer[:PACKET_SIZE]

        # Unpack
        if topic == 0:
            unpacked = struct.unpack(FMT, packet_bytes)

            topic = unpacked[0]
            accel_x, accel_y, accel_z = unpacked[1:4]
            grav_x, grav_y, grav_z = unpacked[4:7]
            gyro_x, gyro_y, gyro_z  = unpacked[7:10]
            flex_thumb, flex_index, flex_middle = unpacked[10:13]
            fsr_thumb, fsr_index, fsr_middle = unpacked[13:16]
        
        elif topic == 1:
            unpacked = struct.unpack('<HH', packet_bytes)

            topic = unpacked[0]
            gesture = unpacked[1]

            print("Gesture:", gesture)
    """


################################################################



pygame.init()

WIDTH = 640
HEIGHT = 480
SIZE = (WIDTH, HEIGHT)

screen = pygame.display.set_mode(SIZE)
clock = pygame.time.Clock()

running = True
while running:
    # EVENT HANDLING
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # GAME STATE UPDATES
    # All game math and comparisons happen here

    readTick()

    # DRAWING
    screen.fill((255, 255, 255))  # always the first drawing command

    pygame.draw.line(screen, (30, 30, 30), (10, HEIGHT / 2), (WIDTH - 10, HEIGHT / 2))

    # Accel
    pygame.draw.circle(screen, (0, 0, 255), (50, 240 - accel_x * 1000.), 20)
    pygame.draw.circle(screen, (0, 0, 255), (100, 240 - accel_y * 1000.), 20)
    pygame.draw.circle(screen, (0, 0, 255), (150, 240 - accel_z * 1000.), 20)
    pygame.draw.circle(screen, (0, 155, 0), (50, 240 - grav_x * 100.), 10)
    pygame.draw.circle(screen, (0, 155, 0), (100, 240 - grav_y * 100.), 10)
    pygame.draw.circle(screen, (0, 155, 0), (150, 240 - grav_z * 100.), 10)

    # Gyro
    pygame.draw.circle(screen, (255, 0, 0), (250, 240 - gyro_x * 10), 25)
    pygame.draw.circle(screen, (255, 0, 0), (300, 240 - gyro_y * 10), 25)
    pygame.draw.circle(screen, (255, 0, 0), (350, 240 - gyro_z * 10), 25)

    # Flex/FSR

    pygame.draw.circle(screen, (255, 155, 0), (450, 240 - fsr_thumb / 20), 5)
    pygame.draw.circle(screen, (255, 155, 0), (460, 240 - fsr_index / 20), 5)
    pygame.draw.circle(screen, (255, 155, 0), (470, 240 - fsr_middle / 20), 5)
    pygame.draw.circle(screen, (50, 30, 100), (500, 240 - flex_thumb * 200), 5)
    pygame.draw.circle(screen, (50, 30, 100), (510, 240 - flex_index * 200), 5)
    pygame.draw.circle(screen, (50, 30, 100), (520, 240 - flex_middle * 200), 5)

    # Must be the last two lines
    # of the game loop
    pygame.display.flip()
    clock.tick(100)
    #---------------------------

ser.close()
pygame.quit()