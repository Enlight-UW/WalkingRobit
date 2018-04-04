import bluetooth
import time
addr = "30:14:08:20:30:86"
port = 1
sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
print("Searching for HC06...")
sock.connect((addr,port))
print("Connected, sleeping for 5 seconds...")
sock.send("Hello, World!")
time.sleep(5)
sock.close()
print("Disconnected...")
