#!/user/bin/env python
import serial

port = "/dev/ttyUSB0"
rate = 115200

s1 = serial.Serial(port, rate)

while True:
    s1.write(str.encode("0:0>"))
