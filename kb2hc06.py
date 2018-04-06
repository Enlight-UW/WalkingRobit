import bluetooth
import time
addr = "30:14:08:20:30:86"
port = 1
sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
print("Searching for HC06...")
sock.connect((addr,port))
print("enter keys")
while True:
    try:
        sock.send(input())
    except KeyboardInterrupt:
        sock.close()
        print("Disconnected...")
        break

