# Handles everything related to reading data

import serial

ser = serial.Serial('COM7', 115200, timeout=1)

yaw, roll, pitch = 0, 0, 0

inputs = {}

def readTick():
    global yaw, roll, pitch

    line = ser.readline().decode('utf-8').strip()

    if line:
        key, val = line.split(':')
        inputs.update({key.strip(): val.strip()})