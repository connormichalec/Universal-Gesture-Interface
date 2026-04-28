import hid

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

    # Send  - not rlly configured tho
    msg = b"cock"
    packet = [0x03] + list(msg) + [0x00] * (63 - len(msg))
    dev.write(packet)

    # Receive
    while True:
        data = dev.read(65)
        if data:
            #if wanting raw data just dont decode it.
            payload = bytes(data[1:]).decode('utf-8', errors='ignore').rstrip('\x00')
            print(f"RX: {payload}")