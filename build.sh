#!/bin/bash

# The contents of wifi.cfg should be:
# ssid
# password
head -n 1 src/wifi.cfg | sed "s/\(.*\)/const char* ssid = \"\0\";/" > src/ssid.h
tail -n 1 src/wifi.cfg | sed "s/\(.*\)/const char* password = \"\0\";/" > src/password.h

xxd -i src/index.html | sed 's/\([0-9a-f]\)$/\0, 0x00/' > src/index.h

pio run -t upload
