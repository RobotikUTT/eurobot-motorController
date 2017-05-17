# !/usr/bin/env python
# -*- coding: utf-8 -*-

import serial
import sys

DT    = 0.05 # s
DUREE = 5 # s

# A changer selon l'OS
ser = serial.Serial('/dev/tty.wchusbserial1420', 115200)

lines = 0
while True:
    message = ser.readline()

    if message.find("Setup") > -1:
        ser.write('avanceG\n');
    else:
        sys.stdout.write(message);
        lines += 1

        if lines >= int(DUREE/DT):
            ser.write('stop\n');
            ser.close();
            sys.exit()

