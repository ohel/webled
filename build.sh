#!/bin/bash

# The contents of $wifi_cfg_in should be these three lines:
#    ssid
#    password
#    static IPv4 address (use 0.0.0.0 for dynamic IP)
# If static IPv4 is set e.g. 192.168.1.4, a gateway of 192.168.1.1 is assumed.

# The contents of $ap_cfg_in should be these three lines:
#    ssid
#    password
#    IPv4 gateway address (use 0.0.0.0 for default)

piodev="1A86:7523"

wifi_cfg_in="src/wifi.cfg"
wifi_cfg_out="src/wificonfig.h"
ap_cfg_in="src/ap.cfg"
ap_cfg_out="src/apconfig.h"

wifi_ssid=$(head -n 1 $wifi_cfg_in 2>/dev/null)
wifi_pass=$(head -n 2 $wifi_cfg_in 2>/dev/null | tail -n 1)
wifi_ip=$(tail -n 1 $wifi_cfg_in 2>/dev/null)
echo ${wifi_ssid:-"wlan"} | sed "s/\(.*\)/const char* ssid = \"\0\";/" > $wifi_cfg_out
echo ${wifi_pass:-"password"} | sed "s/\(.*\)/const char* password = \"\0\";/" >> $wifi_cfg_out
echo ${wifi_ip:-"0.0.0.0"} | sed "s/\./, /g" | sed "s/\(.*\)/IPAddress ip(\0);/" >> $wifi_cfg_out
echo ${wifi_ip:-"0.0.0.0"} | sed "s/\./, /g" | sed "s/[0-9]\{1,\}$/1/" | sed "s/\(.*\)/IPAddress gateway(\0);/" >> $wifi_cfg_out

ap_ssid=$(head -n 1 $ap_cfg_in 2>/dev/null)
ap_pass=$(head -n 2 $ap_cfg_in 2>/dev/null | tail -n 1)
ap_ip=$(tail -n 1 $ap_cfg_in 2>/dev/null)
echo ${ap_ssid:-"ArduinoAP"} | sed "s/\(.*\)/const char* soft_ap_ssid = \"\0\";/" > $ap_cfg_out
echo ${ap_pass:-""} | sed "s/\(.*\)/const char* soft_ap_password = \"\0\";/" >> $ap_cfg_out
echo ${ap_ip:-"0.0.0.0"} | sed "s/\./, /g" | sed "s/\(.*\)/IPAddress soft_ap_ip(\0);/" >> $ap_cfg_out

xxd -i src/index.html | sed 's/\([0-9a-f]\)$/\0, 0x00/' > src/index.h
xxd -i src/favicon.ico > src/favicon.h

port=$(pio device list | grep -B 2 "VID:PID=$piodev" | head -n 1)
pio run -t upload --upload-port $port
