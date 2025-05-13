import socket
import struct
import time

print(f"Size of float in Python: {struct.calcsize('f')}")
print(f"Size of int in Python: {struct.calcsize('i')}")
print(f"Size of uint in Python: {struct.calcsize('I')}")

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
host = "127.0.0.1"
port = 20000

trajectory_data = [
    [(10.0, 20.0), (15.0, 40.0), (20.0, 30.0), (25.0, 50.0), (30.0, 60.0)],
    [(5.0, 10.0), (12.0, 25.0), (18.0, 35.0), (22.0, 45.0), (28.0, 55.0)],
    [(1.0, 2.0), (3.0, 5.0), (6.0, 8.0), (9.0, 12.0), (12.0, 15.0)],
    [(30.0, 5.0), (35.0, 15.0), (40.0, 25.0), (45.0, 35.0), (50.0, 45.0)],
    [(50.0, 10.0), (55.0, 20.0), (60.0, 30.0), (65.0, 40.0), (70.0, 50.0)]
]

def send_packet(points):
    count_points = len(points)
    mark = b"GloS"
    packet_type = 456
    data_size = count_points * 2 * 4
    point_count = count_points

    fmt = f'<4sIIi{count_points * 2}f'
    args = [mark, packet_type, data_size, point_count]
    for p in points:
        args.extend(p)

    payload = struct.pack(fmt, *args)
    sock.sendto(payload, (host, port))
    print(f"UDP Packet sent ({len(payload)} bytes): {points} to {host}:{port}")

while True:
    for points in trajectory_data:
        send_packet(points)
        time.sleep(2)