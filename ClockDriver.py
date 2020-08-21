# ClockDriver. py   Author: Robert Walker
# Dependencies Installation
# ls /dev/i2c* /dev/spi*
# /dev/i2c-1 /dev/spidev0.0 /dev/spidev0.1
# dtoverlay=spi1-3cs in /boot/config.txt
# pip3 install RPI.GPIO
# pip3 install adafruit-blinka
# sudo pip3 install rpi_ws281x adafruit-circuitpython-neopixel
# sudo python3 -m pip install --force-reinstall adafruit-blinka 


import board
import neopixel
import datetime




pixels = neopixel.NeoPixel(board.D18, 30)

mappings = [[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,25,26,27,28,29,30,31],
[0,1,2,3,4,5,6,7,8,9],
[5,6,7,8,9,10,11,12,13,14,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34],
[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,20,21,22,23,24,30,31,32,33,34],
[0,1,2,3,4,5,6,7,8,9,15,16,17,18,19,20,21,22,23,24],
[0,1,2,3,4,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,30,31,32,33,34],
[0,1,2,3,4,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34],
[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14],
[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31],
[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24]]

current_time = datetime.datetime.now()
print(current_time)
hour = current_time.hour
minute = current_time.minute
if(current_time.hour > 12):
    print(current_time.hour- 12)


for i in range(0, len(mappings[int(str(hour[0]))])):
    pixels[mappings[int(str(hour[0]))][i + 179]] = (255,0,0)

for i in range(0, len(mappings[int(str(hour[1]))])):
    pixels[mappings[int(str(hour[1]))][i + 144]] = (255,0,0)

for i in range(0, len(mappings[int(str(minute[0]))])):
    pixels[mappings[int(str(minute[0]))][i + 35] = (255,0,0)

for i in range(0, len(mappings[int(str(minute[1]))])):
    pixels[mappings[int(str(minute[1]))][i]] = (255,0,0)







