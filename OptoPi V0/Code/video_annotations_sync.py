import tkinter as tk
import picamera
import time
import datetime as dt
import serial

#USB_PORT = "/dev/ttyACM0"
USB_PORT = "/dev/ttyACM1"
parent_directory = '/home/pi/Documents/dmel'
date='2021_06_11'
exp='1DR_test'
lighting = 'red'
lighting_paradigm = 'OFF_ON'
gal4 = 'NA'
uas = 'NA'
experiment_name = parent_directory + '/' + date + '_' + gal4 + '_' + uas + '_' + lighting + '_' + lighting_paradigm + '_'+ exp


experiment_duration=20

try:
   ser = serial.Serial(USB_PORT, 9600, timeout=1)
   ser.flush()
except:
   print("ERROR - Could not open USB serial port.  Please check your port name and permissions.")
   print("Exiting program.")
   exit()
ser.write(b'wait')
#file=open(experiment_name + '_log.csv','a')

 # quality 1 is maintain highest quality and 40 for lowest quality- reasonabletrade-ff between bandwidth and quality- 20-25

with picamera.PiCamera() as camera:
        camera.resolution = (1000, 1200)
        camera.color_effects=(128,128)
        camera.framerate = 25
        camera.annotate_background = picamera.Color('black')
        camera.start_preview(alpha=250)
        file = open("basin_log5.csv","a")
        input('Press ENTER to start recording')
        ser.write(b'begin')
        start = dt.datetime.now()
        camera.start_recording(experiment_name + ".h264")
        while not (dt.datetime.now() - start).total_seconds() > experiment_duration:
            if ser.in_waiting > 0:
                file=open(experiment_name + '_log.csv','a')
                line = ser.readline().decode('utf-8').rstrip()
                file.write('\n'+line + str((dt.datetime.now()-start).total_seconds()))
                #file.write("\n"+line +"\n"+ str((dt.datetime.now()-start).total_seconds()))
                file.close()
                #file.write("\n"+str((dt.datetime.now()-start).total_seconds())+","+line)
                #
            camera.annotate_text = str((dt.datetime.now()-start).total_seconds())
        camera.stop_recording()
        
        ser.flush()
        ser.write(b'wait')
ser.flush()
ser.write(b'wait')
