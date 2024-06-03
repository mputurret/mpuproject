import serial
import time

ser = serial.Serial('/dev/ttyACM0',9600,timeout=1)
time.sleep(2)

send_str = "1235.264412, 321.46536"
ser.write((send_str + '\n').encode()) #'utf-8'
print('sent:',send_str)
