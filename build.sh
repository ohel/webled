#!/bin/bash

# The contents of $input should be:
# ssid
# password
# static IPv4 address

input="src/wifi.cfg"
output="src/wificonfig.h"
head -n 1 $input | sed "s/\(.*\)/const char* ssid = \"\0\";/" > $output
head -n 2 $input | tail -n 1 | sed "s/\(.*\)/const char* password = \"\0\";/" >> $output
tail -n 1 $input | sed "s/\./, /g" | sed "s/\(.*\)/IPAddress ip(\0);/" >> $output
tail -n 1 $input | sed "s/\./, /g" | sed "s/[0-9]\{1,\}$/1/" | sed "s/\(.*\)/IPAddress gateway(\0);/" >> $output
echo "IPAddress subnet(255, 255, 255, 0);" >> $output

xxd -i src/index.html | sed 's/\([0-9a-f]\)$/\0, 0x00/' > src/index.h

pio run -t upload
