import serial
import requests
import time

ser = serial.Serial('COM11', 9600, timeout=1)  # Ajuste 'COM11' conforme necessário
time.sleep(2)

while True:
    try:
        line = ser.readline().decode('utf-8').strip()
        if line:
            print(line)
            if 'TEMP' in line and 'HUMID' in line and 'FLAME' in line:
                # Processa dados de sensores
                parts = line.split(',')
                temp = parts[0].split(':')[1]
                humid = parts[1].split(':')[1]
                flame = parts[2].split(':')[1]
                data = {
                    'temperature': temp,
                    'humidity': humid,
                    'flame': flame
                }
                response = requests.post('http://localhost/api/save_data.php', data=data)
                print(response.text)
            elif 'EVENT:DOOR' in line:
                # Processa eventos de porta
                parts = line.split(',')
                event = parts[0].split(':')[2]
                fingerprint = parts[1].split(':')[1]
                data = {
                    'event': event,
                    'fingerprint_id': fingerprint
                }
                response = requests.post('http://localhost/api/log_access.php', data=data)
                print(response.text)
    except Exception as e:
        print(e)
    time.sleep(1)
