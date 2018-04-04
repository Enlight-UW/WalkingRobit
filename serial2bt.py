import bluetooth
import serial
import sys
#connect to bluetooth module
addr = "30:14:08:20:30:86"
port = 1
sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
print("Searching for HC06...")
sock.connect((addr,port))
print("Connected.") 

#connect to serial port
ser = serial.Serial(sys.argv[1],timeout=3)
#poll serial port and send every recieved character across bt
while True:
    try:
        if(ser.in_waiting):
            char = int.from_bytes(ser.read(1),'little')
            print("Got int from serial: {}".format(char))
            sock.send(bytes([char]))
            print("Sent to bluetooth.")
    except KeyboardInterrupt:
        print("Exiting")
        sock.close()
        sys.exit(0)
        sock.close()
        ser.close()
