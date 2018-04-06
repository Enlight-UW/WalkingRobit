import serial
import sys
import time
from datetime import datetime
#connect to serial port
ser_src = serial.Serial(sys.argv[1],timeout=None)
ser_dst = serial.Serial(sys.argv[2],timeout=None,write_timeout=None)


while True:
    try:
        char = ser_src.read(1)
        print(char)
        ser_dst.write(char)
        '''
        try:
        except:
            ser_dst.close()
            ser_dst = serial.Serial(sys.argv[2],timeout=None,write_timeout=0.1)
        '''
    except KeyboardInterrupt:
        print("Exiting")
        ser_src.close()
        ser_dst.close()
        sys.exit(0)
