import binascii
import json
import os.path
import re
import traceback

def getFileBytes(filename: str, append_zero_byte: bool = False):
    bytes = []
    with open(filename, 'rb') as f:
        for byte in iter(lambda: f.read(1), b''):
            bytes.append('0x' + binascii.hexlify(byte).decode())
    if append_zero_byte: bytes.append('0x00')
    return bytes

def createHeaderFile(file_in: str, file_out, var_name: str, append_zero_byte: bool = False):
    bytes = getFileBytes(file_in, append_zero_byte)
    str_out = 'unsigned char ' + var_name + '_bytes[] = {'
    # The 12 bytes per line is just for nice output.
    bytecount = 12
    for byte in bytes:
        if bytecount == 12:
            str_out += '\n  '
            bytecount = 0
        str_out += byte + ","
        if bytecount != 11: str_out += ' '
        bytecount += 1
    str_out = str_out[:-2]
    str_out += '\n};\n'
    str_out += 'char* ' + var_name + ' = reinterpret_cast<char*>(&' + var_name + '_bytes[0]);'

    with open(file_out, 'w') as f:
        f.write(str_out)

def createWifiConfig(cfg_source, cfg_header):
    try:
        if os.path.isfile(cfg_source):
            with open(cfg_source, 'r') as f:
                contents = f.read()
            config = json.loads(contents)
            ssid = config['ssid']
            password = config['password']
            ip = config['ip']
            is_ap = config['ap']
        else:
            raise Exception('Missing WiFi config.')

        # If static IPv4 is set e.g. 192.168.1.4, a gateway of 192.168.1.1 is assumed.
        gateway = re.sub(r'[0-9]{1,}$', '1', ip).replace('.', ',') if not is_ap else '0,0,0,0'

        with open(cfg_header, 'w') as f:
            f.write('const char* _wifi_ssid = "' + ssid + '";\n')
            f.write('const char* _wifi_password = "' + password + '";\n')
            f.write('const IPAddress _wifi_ip(' + ip.replace('.', ',') + ');\n')
            f.write('const IPAddress _wifi_gateway(' + gateway + ');')

    except Exception:
        traceback.print_exc()
        exit(1)

createHeaderFile('web/index.html', 'src/index_html.h', 'index_html', True)
createHeaderFile('web/favicon.ico', 'src/favicon.h', 'favicon')
createWifiConfig('src/wifi_cfg.json', 'src/wifi_cfg.h')
